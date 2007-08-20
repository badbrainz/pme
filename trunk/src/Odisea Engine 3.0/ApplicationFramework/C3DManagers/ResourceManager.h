#ifndef RESOURCEMANAGER
#define RESOURCEMANAGER

#include "../C3DTools/VectorMemberPointer.h"
#include "../C3DMemory/GenericPool.h"

template <class T, int size = 1e2, int min = 1e2> class ResourceManager///fuck
{
  public:
    ResourceManager(void) : m_Aquired(size), m_Cursor(0)
    {}
    
    T* Create(void)
    {
      T* object = m_Pool.allocate();
      AddResource(object);
      return object;
    }
    
    T* Clone(T* copy)
    {
      T* object = new T(*copy);
      AddResource(object);
      return object;
    }
    
    void FlushUnusedResources(void)
    {
      m_Pool.flush();
    }
    
    void AddResource(T* object)
    {
      if (m_Cursor >= m_Aquired.Size())
        m_Aquired.Resize(m_Aquired.Size()*2);
      m_Aquired[m_Cursor++].Reset(object);
    }
  
  private:
    VectorMemberPointer <T>            m_Aquired;
    GenericPool         <T, size, min> m_Pool;
    unsigned int                       m_Cursor;
};

#endif