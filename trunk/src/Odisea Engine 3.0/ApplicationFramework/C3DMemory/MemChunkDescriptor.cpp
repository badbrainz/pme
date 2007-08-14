#include "MemChunkDescriptor.h"

MemChunkDescriptor::MemChunkDescriptor()
{
	Reset();
}

void MemChunkDescriptor::Reset()
{
	m_uiSegment 	= 0;
	m_uiOffset		= 0;
	m_pData				= 0;
	m_pNextChunk	= 0;
	m_bBaseChunk	= false;
}

void MemChunkDescriptor::BaseChunk(bool value)
{	
	m_bBaseChunk = value;
}

bool MemChunkDescriptor::BaseChunk()
{
	return m_bBaseChunk;
}

void MemChunkDescriptor::NextChunk(MemChunkDescriptor* chunk)
{
	m_pNextChunk = chunk;
}

MemChunkDescriptor* MemChunkDescriptor::NextChunk()
{
	return m_pNextChunk;
}

void MemChunkDescriptor::Data(unsigned char* address)
{
	m_pData = address;
}

unsigned char* MemChunkDescriptor::Data()
{
	return m_pData;
}

void MemChunkDescriptor::Segment(unsigned int position)
{
	m_uiSegment = position;
}

unsigned int MemChunkDescriptor::Segment()
{
	return m_uiSegment;
}

void MemChunkDescriptor::Offset(unsigned int offset)
{
	m_uiOffset = offset;
}

unsigned int MemChunkDescriptor::Offset()
{
	return m_uiOffset;
}

MemChunkDescriptor::~MemChunkDescriptor()
{}