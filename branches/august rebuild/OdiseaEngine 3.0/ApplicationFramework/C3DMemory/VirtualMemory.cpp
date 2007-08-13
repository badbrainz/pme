#include "VirtualMemory.h"
#include "MemChunkDescriptor.h"
#include "Tools/Logger.h"
#include <fstream>
#include <math.h>
#include <stack>

/*template <class T>
int ceilPowTwo(T x)
{
  return (int) pow((T)2, ceil(log10(x)/log10((T)2)));
}*/

VirtualMemory::VirtualMemory() :
m_pLastChunk(0),
m_pFirstChunk(0),
m_pChunkCursor(0),
m_uiUsedBytes(0),
m_uiTotalBytes(0),
m_uiTotalChunks(0),
m_uiChunkSize(128),
m_uiMinAllocSize(256)
{
}

VirtualMemory::VirtualMemory(unsigned int initialSize,
														 unsigned int chunkSize,
														 unsigned int allocChunks,
														 bool 				zeroMem) :
m_pLastChunk(0),
m_pFirstChunk(0),
m_pChunkCursor(0),
m_uiUsedBytes(0),
m_uiTotalBytes(0),
m_uiTotalChunks(0),
m_uiChunkSize(chunkSize),
m_uiMinAllocSize(chunkSize*allocChunks)
{
}

void VirtualMemory::SetChunkSize(unsigned int bytes)
{
	m_uiChunkSize = bytes;
}

void VirtualMemory::SetMinAllocSize(unsigned int bytes)
{
	m_uiMinAllocSize = bytes;
}

void VirtualMemory::AllocateMemory(unsigned int bytes, bool zeroMem, unsigned char fillValue)
{
	MemChunkDescriptor *pMemDescriptors = 0,
										 *pChunk 					= 0;
	unsigned int 				iChunkCount			= 0,
											iByteCount			= 0;
	unsigned char 		 *pMemRegion 			= 0;
	
	if (m_uiChunkSize >= bytes)//added '=' 8/2/07
		bytes = m_uiMinAllocSize;//changed 'm_uiMinAllocSize'

	iChunkCount = CalculateMaxChunks(bytes);
	iByteCount	= iChunkCount*m_uiChunkSize;

	m_uiTotalBytes  += iByteCount;
  m_uiTotalChunks += iChunkCount;
	
	pMemDescriptors = new MemChunkDescriptor[iChunkCount];
	pMemRegion			= new unsigned char[iByteCount];
	
	if (zeroMem)
		std::fill(pMemRegion, pMemRegion+iByteCount, fillValue);
	
	if (!m_pFirstChunk)
	{
		m_pFirstChunk  = pMemDescriptors;
		m_pLastChunk   = m_pFirstChunk;
		m_pChunkCursor = m_pFirstChunk;
	}
	
	MapChunks(pMemDescriptors, iChunkCount, pMemRegion);
	
	ResetChunkOffsets(pMemDescriptors, iChunkCount, iByteCount);
	
	m_pChunkCursor = pMemDescriptors;
}

void VirtualMemory::MapChunks(MemChunkDescriptor* chunks, unsigned int count, unsigned char* memory)
{
	MemChunkDescriptor *pChunk = 0;
	
	for (unsigned int i = 0; i < count; i++)
	{
		pChunk = &chunks[i];
		pChunk->Data(&memory[i*m_uiChunkSize]);
		
		if (i == 0)
			pChunk->BaseChunk(true);
		
		m_pLastChunk->NextChunk(pChunk);
		m_pLastChunk = pChunk;
	}
}

void VirtualMemory::ResetChunkOffsets(MemChunkDescriptor* allocator, unsigned int count, unsigned int bytes)
{
	MemChunkDescriptor *pChunk = allocator;
	
  for (unsigned int i = 0; i < count; i++)
  {
		pChunk->Segment(bytes-(i*m_uiChunkSize));
		pChunk = pChunk->NextChunk();
  }
}

void* VirtualMemory::GetRange(unsigned int bytes)
{
	unsigned int 				allocSize 	 		= 0;
	MemChunkDescriptor *pAvailableChunk	= 0;
	
	while (!pAvailableChunk)
	{
		pAvailableChunk = FindSuitableAllocator(bytes);
		
		if (!pAvailableChunk)
		{
			allocSize = bytes < m_uiMinAllocSize ? m_uiMinAllocSize : bytes;
			AllocateMemory(bytes, true, 0x9f);
		}
	}
	
  return (void*) pAvailableChunk->Data();
}

MemChunkDescriptor* VirtualMemory::FindSuitableAllocator(unsigned int bytes)
{
	if (bytes > m_uiTotalBytes-m_uiUsedBytes)
		return 0;

	MemChunkDescriptor *allocator = 0,
										 *marker		= 0;
	unsigned int 				range			= 0;
	
	for (unsigned int i = 0; i < m_uiTotalChunks; i++)
	{
		if (!m_pChunkCursor)
			m_pChunkCursor = m_pFirstChunk;

		if (bytes <= m_pChunkCursor->Segment())
		{
			marker = m_pChunkCursor;
			range  = m_pChunkCursor->Segment();
			
			while (marker)
			{
				if (marker->Offset() > 0)
					break;

				range -= m_uiChunkSize;
				marker = marker->NextChunk();
			}

			if (bytes <= m_pChunkCursor->Segment()-range)
			{
				allocator = m_pChunkCursor;
				allocator->Offset(bytes);
				
				m_uiUsedBytes += CalculateMaxBytes(bytes);
				
				MoveToNextRegion();
				break;
			}
		}
		
		MoveToNextRegion();
	}
	
	return allocator;
}

void VirtualMemory::MoveToNextRegion()
{
	unsigned int chunksToSkip = CalculateMaxChunks(m_pChunkCursor->Offset());
	
	if (chunksToSkip == 0)
		chunksToSkip = 1;
	
	for (unsigned int i = 0; i < chunksToSkip; i++)
		m_pChunkCursor = m_pChunkCursor->NextChunk();
}

void VirtualMemory::FreeRange(void* memory)
{
	MemChunkDescriptor *pChunk = FindMappedAllocator(memory);
	
  if (pChunk)
	  FreeRegion(pChunk);

	m_pChunkCursor = pChunk;
}

void VirtualMemory::FreeRegion(MemChunkDescriptor* allocator)
{
  MemChunkDescriptor *pChunk		= allocator;
  unsigned int	 			linkCount = CalculateMaxChunks(pChunk->Offset());
	
	std::fill(pChunk->Data(), pChunk->Data()+m_uiChunkSize*linkCount, 0x23);
	m_uiUsedBytes -= m_uiChunkSize*linkCount;

	pChunk->Offset(0);
}

MemChunkDescriptor* VirtualMemory::FindMappedAllocator(void* memory)
{
	MemChunkDescriptor *pChunk = m_pFirstChunk;
	
	while (pChunk)
	{
		if (pChunk->Data() == ((unsigned char*)memory))
			break;
		pChunk = pChunk->NextChunk();
	}
	
	return pChunk;
}

unsigned int VirtualMemory::CalculateMaxChunks(unsigned int bytes)
{
	return (unsigned int) ceil((float)bytes/m_uiChunkSize);
}

unsigned int VirtualMemory::CalculateMaxBytes(unsigned int bytes)
{
  return CalculateMaxChunks(bytes)*m_uiChunkSize;
}

unsigned int VirtualMemory::CalculateRealUsedBytes(void)
{
	MemChunkDescriptor *pChunk = m_pFirstChunk;
	unsigned int usedBytes = 0;
	while (pChunk)
	{
		usedBytes += pChunk->Offset();
		pChunk = pChunk->NextChunk();
	}
	return usedBytes;
}

void VirtualMemory::PrintMemoryBlock(const String &fileName)
{
	std::ofstream outputFile(fileName, std::ofstream::out|std::ofstream::binary);
	MemChunkDescriptor *pChunk = m_pFirstChunk;
	
  while (pChunk)
  {
    if (outputFile.good())
			outputFile.write((char*)pChunk->Data(), (std::streamsize)m_uiChunkSize);
    pChunk = pChunk->NextChunk();
  }
	
  outputFile.close();
}

void VirtualMemory::PrintMemoryProfile(const String &fileName)
{
	std::ofstream outputFile(fileName, std::ofstream::out);
	if (outputFile.good())
	{
		MemChunkDescriptor *pChunk = m_pFirstChunk;
		unsigned int usedBytes = CalculateRealUsedBytes();
		unsigned int memBlocks = 0;
		while (pChunk)
		{
			if (pChunk->BaseChunk())
				memBlocks++;
			pChunk = pChunk->NextChunk();
		}
		outputFile 	<< "Memory Profile\n\n"
								<< "total memory blocks: " 		<< memBlocks << "\n"
								<< "total allocated memory: " << m_uiTotalBytes << "\n"
								<< "total used memory: " 			<< usedBytes << "\n"
								<< "total unused memory: " 		<< m_uiTotalBytes-usedBytes << "\n"
								<< "total chunks: " 					<< m_uiTotalChunks << "\n"
								<< "chunk size: " 						<< m_uiChunkSize << "\n"
								<< "chunk footprint: "				<< sizeof(MemChunkDescriptor) << "\n"
								<< "total chunk footprint: "	<< sizeof(MemChunkDescriptor)*m_uiTotalChunks << "\n\n"
								<< "Segment_Offset \n\n";
		pChunk = m_pFirstChunk;
		while (pChunk)
		{
			outputFile  << pChunk->Segment() << "_" << pChunk->Offset() << "\n";
			pChunk = pChunk->NextChunk();
		}
	}
  outputFile.close();
}

void VirtualMemory::DeallocateMemory()
{
	MemChunkDescriptor *pChunk = 0;
	unsigned char 		 *pData	 = 0;
	std::stack <unsigned char*> memBlocks;
	
	pChunk = m_pFirstChunk;
	
	while (pChunk)
	{
		if (pChunk->BaseChunk())
			memBlocks.push(pChunk->Data());
		pChunk = pChunk->NextChunk();
	}

	while (!memBlocks.empty())
	{
		pData = memBlocks.top();
		memBlocks.pop();
		delete [] pData;
	}
}

void VirtualMemory::DeallocateChunks()
{
	MemChunkDescriptor *pChunk		 = 0,
										 *pBaseChunk = 0;
	std::stack <MemChunkDescriptor*> baseChunks;
	
	pChunk = m_pFirstChunk;
	
	while (pChunk)
  {
		if (pChunk->BaseChunk())
			baseChunks.push(pChunk);
		pChunk = pChunk->NextChunk();
  }
	
	while (!baseChunks.empty())
	{
		pBaseChunk = baseChunks.top();
		baseChunks.pop();
		delete [] pBaseChunk;
	}
}

void VirtualMemory::Destroy()
{
	DeallocateMemory();
	DeallocateChunks();
}

VirtualMemory::~VirtualMemory()
{
	Destroy();
}