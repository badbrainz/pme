#include "Archive.h"
#include "../zlib/zlib.h"
#include "../Math/MathUtils.h"
#include "../Tools/Logger.h"

#ifndef ZUNP_WIN_SIZE
#define ZUNP_WIN_SIZE 32 * 1024
#endif

namespace ZipState
{
	enum  Enum
	{
		ENCRYPTED,
		NOT_ENCRYPTED,
		NOT_RECOGNIZED
	};
}

Archive::Archive(const char* name)
{
	archivename = name;
}

bool Archive::initialize(const char* path)
{
	ZipDirectory  zd;
	ZipEntry      zde;
	ArchivedFile  cendir;
	ArchivedFile  dirent;
	unsigned int  sig;
	unsigned char entryname[256];
	
	if (!archiveMap.createMapping(path))
		return false;
		
	//CENTRAL DIRECTORY
	archiveMap.createView("cendir", archiveMap.getFileSize() - 22, 22, &cendir);
	
	cendir.read(&sig, 4);
	
	if (ZipState::NOT_RECOGNIZED == (state = verifyZip(sig)))
	{
		archiveMap.close();
		return false;
	}
	
	cendir.seek(4, SEEKD);
	
	cendir.read(&zd.zipentrcnt, 2);
	cendir.read(&zd.zipentrtot, 2);
	cendir.read(&zd.zipdirsize, 4);
	cendir.read(&zd.zipdirdata, 4);
	
	if (ZipState::ENCRYPTED == state)
	{
		decryptulong(zd.zipdirsize);
		decryptulong(zd.zipdirdata);
		decryptushort(zd.zipentrcnt);
		decryptushort(zd.zipentrtot);
	}
	
	if (zd.zipentrcnt == 0)
	{
		archiveMap.close();
		return false;
	}
	
	archiveMap.unmapView(&cendir);
	
	//DIRECTORY ENTRIES
	archiveMap.createView("zipdirent", zd.zipdirdata, zd.zipdirsize, &dirent);
	
	for (int i = 0; i < zd.zipentrcnt; i++)
	{
		dirent.seek(10, SEEKD);
		dirent.read(&zde.zipcomp, 2);
		dirent.read(&zde.ziptime, 2);
		dirent.read(&zde.zipdate, 2);
		dirent.seek(4, SEEKD);
		dirent.read(&zde.zipsize, 4);
		dirent.read(&zde.zipuncmp, 4);
		dirent.read(&zde.zipfnln, 2);
		dirent.read(&zde.zipxtraln, 2);
		dirent.seek(6, SEEKD);
		dirent.read(&zde.zipattr, 4);
		dirent.read(&zde.zipdata, 4);
		
		if (ZipState::ENCRYPTED == state)
		{
			decryptulong(zde.zipsize);
			decryptulong(zde.zipdata);
			decryptulong(zde.zipattr);
			decryptulong(zde.zipuncmp);
			decryptushort(zde.zipcomp);
			decryptushort(zde.ziptime);
			decryptushort(zde.zipdate);
			decryptushort(zde.zipfnln);
			decryptushort(zde.zipxtraln);
		}
		
		dirent.read(entryname, zde.zipfnln);
		
		if (ZipState::ENCRYPTED == state)
			for (int j = 0; j < zde.zipfnln; j++)
				decryptuchar(entryname[j]);
				
		entryname[zde.zipfnln] = '\0';
		
		zde.zipname = ICString((char*)entryname);
		
		if ((zde.zipattr & ZipEntry::FILE) && !(zde.zipattr & ZipEntry::DIRECTORY))
			entries.insertKeyAndValue(zde.zipname, zde);
			
		if (zde.zipattr & ZipEntry::DIRECTORY)
			directories.append(zde.zipname);
			
		dirent.seek(zde.zipxtraln, SEEKD);
	}
	
	archiveMap.unmapView(&dirent);
	
	return true;
}

bool Archive::findEntry(const char* name, ZipEntry* entry)
{
	if (!name || !entry)
		return false;
		
	if (entries.findValue(name, *entry))
		return true;
		
	for (unsigned int i = 0; i < directories.length(); i++)
		if (entries.findValue(directories(i) + name, *entry))
			return true;
			
	ICString::size_type i;
	AVLTreeTIterator <ICString, ZipEntry, 4> iter(entries);
	
	while (iter.next())
	{
		i = iter.key().find(name);
		
		if (i != ICString::npos)
			if (iter.key().substr(i) == name)
			{
				*entry = iter.value();
				return true;
			}
	}
	
	return false;
}

bool Archive::containsEntry(const char* name)
{
	if (!name)
		return false;
		
	if (entries.contains(name))
		return true;
		
	for (unsigned int i = 0; i < directories.length(); i++)
		if (entries.contains(directories(i) + name))
			return true;
			
	ICString::size_type i;
	AVLTreeTIterator <ICString, ZipEntry, 4> iter(entries);
	
	while (iter.next())
	{
		i = iter.key().find(name);
		
		if (i != ICString::npos)
			if (iter.key().substr(i) == name)
				return true;
	}
	
	return false;
}

unsigned int Archive::verifyZip(unsigned int sig)
{
	return (sig == 0x06054b50) ? ZipState::NOT_ENCRYPTED
				 : ((sig ^ 0xabababab) == 0x06054b50) ? ZipState::ENCRYPTED
				 : ZipState::NOT_RECOGNIZED;
}

bool Archive::unpackEntry(ZipEntry* entry, unsigned char* out)
{
	if (!entry || !out)
		return false;
		
	bool success = false;
	unsigned short xtra;
	int ret, dec, counter = 0;
	unsigned char input[ZUNP_WIN_SIZE], output[ZUNP_WIN_SIZE];
	z_stream strm;
	ArchivedFile ent;
	
	//begin hack
	archiveMap.createView(entry->zipname.c_str(), entry->zipdata, entry->zipsize + 30 + entry->zipfnln + entry->zipxtraln, &ent);
	
	ent.seek(28, SEEKD);
	ent.read(&xtra, 2);
	
	if (ZipState::ENCRYPTED == state)
		decryptushort(xtra);
		
	ent.seek(entry->zipfnln + xtra, SEEKD);
	//end hack
	
	if (entry->zipcomp == 0x0000)
	{
		ent.read(out, entry->zipsize);
		
		if (ZipState::ENCRYPTED == state)
			for (unsigned int j = 0; j < entry->zipsize; j++)// <-- ?
				decryptuchar(out[j]);
				
		success = true;
	}
	else if (entry->zipcomp == 0x0008)
	{
		strm.zalloc		= Z_NULL;
		strm.zfree		= Z_NULL;
		strm.opaque		= Z_NULL;
		strm.next_in	= Z_NULL;
		strm.avail_in = 0;
		ret = inflateInit2(&strm, -MAX_WBITS);
		
		do
		{
			if ((strm.avail_in = ent.read(input, ZUNP_WIN_SIZE)) == 0)
				break;
				
			if (ZipState::ENCRYPTED == state)
				for (unsigned int j = 0; j < strm.avail_in; j++)
					decryptuchar(input[j]);
					
			strm.next_in = input;
			
			do
			{
				strm.avail_out = ZUNP_WIN_SIZE;
				strm.next_out = output;
				ret = inflate(&strm, Z_NO_FLUSH);
				dec = ZUNP_WIN_SIZE - strm.avail_out;
				
				if (dec > 0)
				{
					memcpy(out + counter, output, dec);
					counter += dec;
				}
			}
			while (strm.avail_out == 0);
		}
		while (ret != Z_STREAM_END);
		
		inflateEnd(&strm);
		
		success = true;
	}
	
	archiveMap.unmapView(&ent);
	
	return success;
}

void Archive::decryptulong(unsigned int& v)
{
	v ^= 0xabababab;
}

void Archive::decryptuchar(unsigned char& v)
{
	v ^= 0xab;
}

void Archive::decryptushort(unsigned short& v)
{
	v ^= 0xabab;
}

void Archive::printAllPaths()
{
	if (!archivename.getLength())
		return;
		
	for (unsigned int i = 0; i < directories.length(); i++)
		Logger::writeInfoLog(directories(i).c_str());
}

void Archive::printAllEntries()
{
	if (!archivename.getLength())
		return;
		
	AVLTreeTIterator <ICString, ZipEntry, 4> iter(entries);
	
	while (iter.next())
		Logger::writeInfoLog(iter.key().c_str());
}

void Archive::close()
{
	archiveMap.close();
}

Archive::~Archive()
{
	close();
}
