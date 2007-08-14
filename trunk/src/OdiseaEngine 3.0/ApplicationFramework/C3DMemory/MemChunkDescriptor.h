#ifndef MEMCHUNKDESCRIPTOR
#define MEMCHUNKDESCRIPTOR

class MemChunkDescriptor
{
	public:
		 MemChunkDescriptor();
		~MemChunkDescriptor();
	
	public:
		unsigned char* 			Data();
		void 								Data(unsigned char* address);
		
		MemChunkDescriptor*	NextChunk();
		void 								NextChunk(MemChunkDescriptor* chunk);
		
		unsigned int				Segment();
		void 								Segment(unsigned int segment);
		
		unsigned int				Offset();
		void 								Offset(unsigned int offset);
		
		bool 								BaseChunk();
		void 								BaseChunk(bool value);
		
		void 								Reset();
	
	private:
		unsigned int				m_uiSegment;
		unsigned int				m_uiOffset;
		
		bool					 			m_bBaseChunk;
		MemChunkDescriptor* m_pNextChunk;
		
		unsigned char* 			m_pData;
};

#endif