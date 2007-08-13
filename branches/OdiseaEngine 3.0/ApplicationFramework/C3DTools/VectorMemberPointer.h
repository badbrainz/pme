#ifndef VECTORMEMBERPOINTER
#define VECTORMEMBERPOINTER

#include "MemberPointer.h"

template <class T>
class VectorMemberPointer
{
	public:
		VectorMemberPointer(unsigned int size = 0) : m_Size(size)
		{
			m_Members = new MemberPointer<T>[m_Size];
		}
		
		~VectorMemberPointer()
		{
			delete [] m_Members;
		}

	public:
		MemberPointer<T>& operator [] (unsigned int index)
		{
			return m_Members[index];
		}
	
	protected:	
		void operator = (const VectorMemberPointer<T> &right)
		{}
	
	public:
		unsigned int Size()
		{
			return m_Size;
		}
		
		void Resize(unsigned int size)
		{
			MemberPointer<T> *newArray = new MemberPointer<T>[size];
			
			for (unsigned int i = 0; i < (m_Size<size ?  m_Size : size); i++)
				newArray[i].Reset(m_Members[i].Release());
			
			delete [] m_Members;
			m_Size = size;
			m_Members = newArray;
		}
	
	protected:	
		VectorMemberPointer(const VectorMemberPointer<T> &copy)
		{}

	private:
		unsigned int 				m_Size;
		MemberPointer<T>*		m_Members;
};

#endif