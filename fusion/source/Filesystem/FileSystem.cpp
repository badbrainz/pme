#include "FileSystem.h"
#include "DirectoryScanner.h"
#include "Archive.h"
#include "../Managers/ArchivedItemManager.h"
#include "../Managers/MediaPathManager.h"
#include "../Kernel/Gateway.h"

Array <OCString> FileSystem::registeredNames;
ArrayPtr <Archive> FileSystem::archives;
GenericPool <ArchivedFile, 20, 10> FileSystem::openedFiles;

bool FileSystem::initialize()
{
	DirectoryScanner    dir;
	DirectoryItem       item;
	const char*         path;
	OCString            ext;
	OCString::size_type idx;
	HKEY                hkey;
	DWORD               buffersize;
	char*               regval;
	
	LONG success = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Pyro Studios\\Praetorians Game\\Paths", 0, KEY_READ, &hkey);
	
	if (ERROR_SUCCESS != success)
		success = RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Pyro Studios\\Praetorians Game\\Paths", 0, KEY_READ, &hkey);
		
	if (ERROR_SUCCESS == success)
	{
		RegQueryValueEx(hkey, "Raiz", 0, 0, 0, &buffersize);
		regval = new char[buffersize];
		RegQueryValueEx(hkey, "Raiz", 0, 0, (LPBYTE)regval, &buffersize);
		String strkey = String(regval) + "DATA\\";
		strkey.convertChars('\\', '/');
		MediaPathManager::registerPath(strkey);
		deleteArray(regval);
		RegCloseKey(hkey);
	}
	
	for (int i = 0; i < MediaPathManager::getPathCount(); i++)
	{
		path = MediaPathManager::getPath(i);
		dir.set(path);
		
		while (dir.next())
		{
			item = dir.currentItem();
			
			if (DIR_FILE == item.type)
			{
				if (OCString::npos == (idx = item.name.rfind(".")))
					continue;
					
				ext = item.name.substr(idx + 1);
				
				if (ext == "PAK" || ext == "pak" || ext == "ZIP" || ext == "zip" || ext == "PAT" || ext == "pat")
					if (!FileSystem::registerArchive(OCString(path + item.name).c_str()))
						Logger::writeWarningLog(String("Could not register archive -> ") + item.name.c_str());
			}
		}
	}
	
	return true;
}

bool FileSystem::registerArchive(const char* path)
{
	if (!strlen(path))
		return false;
		
	OCString tpath(path);
	
	tpath = tpath.substr(tpath.rfind('/') + 1);
	
	if (FileSystem::checkForRepeat(tpath.c_str()))
		return true;
		
	Archive* archive = new Archive(tpath.substr(0, tpath.rfind('.')).c_str());
	
	if (!archive->initialize(path))
	{
		deleteObject(archive);
		return false;
	}
	
	registeredNames.append(tpath);
	archives.append(archive);
	
	return true;
}

ArchivedFile* FileSystem::checkOut(const char* name, unsigned int order)
{
	ArchivedFile* p = openedFiles.allocate();
	p->reset();
	bool success = false;
	
	if (!strlen(name) || archives.isEmpty())
	{
		Logger::writeWarningLog(String("FileSystem::checkOut -> ") + name);
		return p;
	}
	
	switch (order)
	{
		case ArchFileOrder::FIRST_FOUND:
			success = checkOutItem(name, p);
			break;
			
		case ArchFileOrder::NEWEST_MODIFIED:
			success = checkOutNewestItem(name, p);
			break;
			
		case ArchFileOrder::OLDEST_MODIFIED:
			success = checkOutOldestItem(name, p);
			break;
			
		default:
			break;
	}
	
	if (!success)
	{
		openedFiles.recycle(p);
		return 0;
	}
	
	return p;
}

bool FileSystem::checkOutItem(const char* name, ArchivedFile* file)
{
	ZipEntry entry;
	unsigned char* buf = 0;
	ArchiveInfo* info;
	
	for (unsigned int i = 0; i < archives.length(); i++)
	{
		if (archives(i)->findEntry(name, &entry))
		{
			info = ArchivedItemManager::getItemInfo(entry.zipname.c_str());
			
			if (info)
			{
				info->increaseUserCount();
				buf = info->getMedia();
			}
			else
			{
				buf = (unsigned char*) Gateway::aquireFilebufferMemory(entry.zipuncmp);
				
				if (!archives(i)->unpackEntry(&entry, buf))
				{
					Gateway::releaseFilebufferMemory(buf);
					return Logger::writeErrorLog(String("Could not unpack file -> ") + name);
				}
				
				ArchivedItemManager::addItemInfo(new ArchiveInfo(entry.zipname.c_str(), buf));
			}
			
			file->size = entry.zipuncmp;
			file->setName(entry.zipname.c_str());
			file->data = buf;
			
			return true;
		}
	}
	
	return false;
}

bool FileSystem::checkOutNewestItem(const char* name, ArchivedFile* file)
{
	unsigned char* buf = 0;
	ArchiveInfo* info;
	Archive *pArch;
	unsigned int entrydatetime, comparedatetime;
	ZipEntry entry, newestEntry;
	ArrayPtr <Archive> flaggedArchives;
	
	for (unsigned int i = 0; i < archives.length(); i++)
		if (archives(i)->containsEntry(name))
			flaggedArchives.append(archives(i));
			
	//only one exist
	if (flaggedArchives.length() == 1)
	{
		flaggedArchives(0)->findEntry(name, &entry);
		info = ArchivedItemManager::getItemInfo(entry.zipname.c_str());
		
		if (info)
		{
			info->increaseUserCount();
			buf = info->getMedia();
		}
		else
		{
			buf = (unsigned char*) Gateway::aquireFilebufferMemory(entry.zipuncmp);
			
			if (!flaggedArchives(0)->unpackEntry(&entry, buf))
			{
				Gateway::releaseFilebufferMemory(buf);
				return Logger::writeErrorLog(String("Could not unpack file -> ") + name);
			}
			
			ArchivedItemManager::addItemInfo(new ArchiveInfo(entry.zipname.c_str(), buf));
		}
		
		file->size = entry.zipuncmp;
		file->setName(entry.zipname.c_str());
		file->data = buf;
		
		return true;
	}
	
	//compare all datetime values
	pArch = flaggedArchives(0);
	pArch->findEntry(name, &entry);
	comparedatetime = (entry.zipdate << 16) | entry.ziptime;
	
	for (unsigned int i = 1; i < flaggedArchives.length(); i++)
	{
		flaggedArchives(i)->findEntry(name, &entry);
		entrydatetime = (entry.zipdate << 16) | entry.ziptime;
		
		if (entrydatetime > comparedatetime)
		{
			comparedatetime = entrydatetime;
			pArch = flaggedArchives(i);
			newestEntry = entry;
		}
	}
	
	info = ArchivedItemManager::getItemInfo(newestEntry.zipname.c_str());
	
	if (info)
	{
		info->increaseUserCount();
		buf = info->getMedia();
	}
	else
	{
		buf = (unsigned char*) Gateway::aquireFilebufferMemory(newestEntry.zipuncmp);
		
		if (!pArch->unpackEntry(&newestEntry, buf))
		{
			Gateway::releaseFilebufferMemory(buf);
			return Logger::writeErrorLog(String("Could not unpack file -> ") + name);
		}
		
		ArchivedItemManager::addItemInfo(new ArchiveInfo(newestEntry.zipname.c_str(), buf));
	}
	
	file->size = newestEntry.zipuncmp;
	file->setName(newestEntry.zipname.c_str());
	file->data = buf;
	
	return true;
}

bool FileSystem::checkOutOldestItem(const char* name, ArchivedFile* file)
{
	unsigned char* buf = 0;
	ArchiveInfo* info;
	Archive *pArch;
	unsigned int entrydatetime, comparedatetime;
	ZipEntry entry, oldestEntry;
	ArrayPtr <Archive> flaggedArchives;
	
	for (unsigned int i = 0; i < archives.length(); i++)
		if (archives(i)->containsEntry(name))
			flaggedArchives.append(archives(i));
			
	//only one exist
	if (flaggedArchives.length() == 1)
	{
		flaggedArchives(0)->findEntry(name, &entry);
		info = ArchivedItemManager::getItemInfo(entry.zipname.c_str());
		
		if (info)
		{
			info->increaseUserCount();
			buf = info->getMedia();
		}
		else
		{
			buf = (unsigned char*) Gateway::aquireFilebufferMemory(entry.zipuncmp);
			
			if (!flaggedArchives(0)->unpackEntry(&entry, buf))
			{
				Gateway::releaseFilebufferMemory(buf);
				return Logger::writeErrorLog(String("Could not unpack file -> ") + name);
			}
			
			ArchivedItemManager::addItemInfo(new ArchiveInfo(entry.zipname.c_str(), buf));
		}
		
		file->size = entry.zipuncmp;
		file->setName(entry.zipname.c_str());
		file->data = buf;
		
		return true;
	}
	
	//compare all datetime values
	pArch = flaggedArchives(0);
	pArch->findEntry(name, &entry);
	comparedatetime = (entry.zipdate << 16) | entry.ziptime;
	
	for (unsigned int i = 1; i < flaggedArchives.length(); i++)
	{
		flaggedArchives(i)->findEntry(name, &entry);
		entrydatetime = (entry.zipdate << 16) | entry.ziptime;
		
		if (entrydatetime < comparedatetime)
		{
			comparedatetime = entrydatetime;
			pArch = flaggedArchives(i);
			oldestEntry = entry;
		}
	}
	
	info = ArchivedItemManager::getItemInfo(oldestEntry.zipname.c_str());
	
	if (info)
	{
		info->increaseUserCount();
		buf = info->getMedia();
	}
	else
	{
		buf = (unsigned char*) Gateway::aquireFilebufferMemory(oldestEntry.zipuncmp);
		
		if (!pArch->unpackEntry(&oldestEntry, buf))
		{
			Gateway::releaseFilebufferMemory(buf);
			return Logger::writeErrorLog(String("Could not unpack file -> ") + name);
		}
		
		ArchivedItemManager::addItemInfo(new ArchiveInfo(oldestEntry.zipname.c_str(), buf));
	}
	
	file->size = oldestEntry.zipuncmp;
	file->setName(oldestEntry.zipname.c_str());
	file->data = buf;
	
	return true;
}

bool FileSystem::fileExist(const char* name)
{
	if (!strlen(name) || archives.isEmpty())
		return false;
		
	for (unsigned int i = 0; i < archives.length(); i++)
		if (archives(i)->containsEntry(name))
			return true;
			
	return false;
}

void FileSystem::checkIn(ArchivedFile* file)
{
	if (!file || !file->data)
		return;
		
	ArchiveInfo* info = ArchivedItemManager::getItemInfo(file->getName());
	
	if (info)
	{
		info->decreaseUserCount();
		
		if (info->getUserCount() == 0)
			Gateway::releaseFilebufferMemory(info->getMedia());
			
		ArchivedItemManager::flushUnusedItems();
		file->reset();
	}
	
	openedFiles.recycle(file);
}

bool FileSystem::checkForRepeat(const char* path)
{
	return registeredNames.contains(path);
}

void FileSystem::printAllRegisteredNames()
{
	for (unsigned int i = 0; i < registeredNames.length(); i++)
		Logger::writeInfoLog(String("Registered  archive -> ") + registeredNames(i).c_str());
}

void FileSystem::printAllPaths()
{
	for (unsigned int i = 0; i < archives.length(); i++)
		archives(i)->printAllPaths();
}

void FileSystem::printAllEntries()
{
	for (unsigned int i = 0; i < archives.length(); i++)
		archives(i)->printAllEntries();
}

void FileSystem::reset()
{
	//ArchivedItemManager::flushAllItems();
	ArchiveInfo* info;
	
	for (unsigned int i = 0; i < ArchivedItemManager::getInfoCount(); i++)
	{
		info = ArchivedItemManager::getItemInfo(i);
		
		if (info->getUserCount() == 0)
			Gateway::releaseFilebufferMemory(info->getMedia());
	}
	
	ArchivedItemManager::flushUnusedItems();
}

void FileSystem::destroy()
{
	//for (size_t i = 0; i < archives.length(); i++)
	//  archives(i)->close();
	ArchiveInfo* info;
	
	for (unsigned int i = 0; i < ArchivedItemManager::getInfoCount(); i++)
	{
		info = ArchivedItemManager::getItemInfo(i);
		
		if (info->getUserCount() == 0)
			Gateway::releaseFilebufferMemory(info->getMedia());
	}
	
	ArchivedItemManager::flushAllItems();
	
	archives.clearAndDestroy();
	registeredNames.clear();
}
