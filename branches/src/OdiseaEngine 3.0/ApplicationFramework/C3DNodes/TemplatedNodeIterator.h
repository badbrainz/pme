#ifndef TEMPLATEDNODEITERATOR
#define TEMPLATEDNODEITERATOR

#include "GraphNode.h"

template <class T>
class TemplatedNodeIterator
{
	public:
		TemplatedNodeIterator() : m_pNode(0)
		{}
		
		TemplatedNodeIterator(T* node)
		{
			m_pNode = node;
		}
	
	public:
		TemplatedNodeIterator& operator ++ ()
		{
			m_pNode = m_pNode->GetNextSibling();
			return *this;
		}
		
		TemplatedNodeIterator	operator ++ (int)
		{
			TemplatedNodeIterator temp = *this;
			m_pNode = (T*)m_pNode->GetNextSibling();
			return temp;
		}
		
		bool End()
		{
			return (m_pNode == 0);
		}
		
		T* Current()
		{
			return m_pNode;
		}
	
	protected:
		T* m_pNode;
};

#endif