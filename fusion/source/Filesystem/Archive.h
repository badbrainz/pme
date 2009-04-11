#ifndef ARCHIVE
#define ARCHIVE

#include "ArchiveMapping.h"
#include "../Tools/ICString.h"

#pragma pack(2)

struct ZipDirectory
{
	unsigned short zipentrcnt;
	unsigned short zipentrtot;
	unsigned int zipdirsize;
	unsigned int zipdirdata;
};

struct ZipEntry
{
	enum
	{
		FILE = 0x00000020,
		DIRECTORY = 0x00000010
	};
	
	unsigned short zipcomp;
	unsigned short zipdate;
	unsigned short ziptime;
	unsigned short zipfnln;
	unsigned short zipxtraln;
	unsigned int zipsize;
	unsigned int zipuncmp;
	unsigned int zipattr;
	unsigned int zipdata;
	ICString zipname;
};

#pragma pack()

class Archive
{
	public:
		Archive(const char* name = "");
		~Archive();
		
	public:
		bool initialize(const char* path);
		
		bool unpackEntry(ZipEntry* entry, unsigned char* out);
		bool findEntry(const char* name, ZipEntry* entry);
		bool containsEntry(const char* name);
		
		void printAllPaths();
		void printAllEntries();
		
	private:
		void close();
		unsigned int verifyZip(unsigned int sig);
		void decryptulong(unsigned int& v);
		void decryptuchar(unsigned char& v);
		void decryptushort(unsigned short& v);
		
	private:
		Array <ICString> directories;
		AVLTreeT <ICString, ZipEntry, 4> entries;
		
		unsigned int state;
		
		ArchiveMapping archiveMap;
		
		String archivename;
};

#endif
