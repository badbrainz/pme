#ifndef RESOURCEMANAGER
#define RESOURCEMANAGER

#include "../C3DTools/VectorMemberPointer.h"

template <class T>
class ResourceManager
{
	public:
		ResourceManager(int size = 100) : m_Resources(size), m_Cursor(0)
		{}
		
		T* Create()
		{
			T* object = new T;
			AddResource(object);
			return object;
		}
		
		T* Clone(T* copy)
		{
			T* object = new T(*copy);
			AddResource(object);
			return object;
		}
		
		void AddResource(T* controller)
		{
      if (m_Cursor >= m_Resources.Size())
        m_Resources.Resize(m_Resources.Size()*2);
      
			m_Resources[m_Cursor++].Reset(controller);
		}
	
	private:
		VectorMemberPointer <T> m_Resources;
		unsigned int m_Cursor;
};

#endif