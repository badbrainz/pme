#ifndef VIRTUALMEMORY
#define VIRTUALMEMORY

#include "Tools/String.h"

class MemChunkDescriptor;
//class MemoryDefragmentor;

class VirtualMemory
{
	public:
		 VirtualMemory(unsigned int initialSize,
									 unsigned int chunkSize,
									 unsigned int allocChunks,
									 bool 				zeroMem);
		 VirtualMemory();
		~VirtualMemory();
	
	public:
		void 								AllocateMemory(unsigned int  bytes	 	 = 1024,
																			 bool 				 zeroMem 	 = true,
																			 unsigned char fillValue = 0x2e);
		
		void 								FreeRange(void *memory);
		void*								GetRange(unsigned int bytes);
		
		void 								SetChunkSize(unsigned int bytes);
		void 								SetMinAllocSize(unsigned int bytes);

		void 								PrintMemoryBlock(const String &fileName);
		void 								PrintMemoryProfile(const String &fileName);
	
	private:		
		void 								Destroy();
		void								DeallocateMemory();
		void								DeallocateChunks();
		void 								FreeRegion(MemChunkDescriptor* allocator);
		
		void								MoveToNextRegion();

		unsigned int				CalculateRealUsedBytes(void);
		unsigned int 				CalculateMaxBytes(unsigned int bytes);
		unsigned int 				CalculateMaxChunks(unsigned int bytes);
		
		MemChunkDescriptor* FindMappedAllocator(void* memory);
		MemChunkDescriptor* FindSuitableAllocator(unsigned int bytes);

		void 								MapChunks(MemChunkDescriptor* chunks, unsigned int count, unsigned char* memory);
		void								ResetChunkOffsets(MemChunkDescriptor* allocator, unsigned int chunkCount, unsigned int byteCount);
	
	private:
    MemChunkDescriptor *m_pChunkCursor,
											 *m_pFirstChunk,
											 *m_pLastChunk;
		
		unsigned int 				m_uiMinAllocSize,
												m_uiTotalChunks,
												m_uiChunkSize;
		
		unsigned int				m_uiTotalBytes,
												m_uiUsedBytes;
};

//class MemoryDefragmentor
//{
//  public:
//    MemoryDefragmentor()
//    {
//      //m_DefragMemory.SetDefragmentation(false);
//    }
//    void AllocateMemory(unsigned int bytes, unsigned int chunkSize, unsigned int minAlloc)
//    {
//      m_DefragMemory.SetMinAllocSize(minAlloc);
//      m_DefragMemory.SetChunkSize(chunkSize);
//      m_DefragMemory.AllocateMemory(bytes);
//    }
//private:
//    VirtualMemory m_DefragMemory;
//};

#endif