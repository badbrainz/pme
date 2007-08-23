#ifndef RESOURCEMANAGER
#define RESOURCEMANAGER

#include "../C3DMemory/GenericPool.h"
#include "../OC/ocarray.h"
#include <iostream>

class ManagedResource
{
  public:
    virtual void Destroy() = 0;
};

template <class T, int size = 1e2, int min = 1e2> class ResourceManager///fuck
{
  public:
    ResourceManager(void) : m_Aquired(size)
    {}
    
    ~ResourceManager(void)
    {
      FlushAllResources();
      m_Pool.flush();
    }
    
    T* Create(void)
    {
      T* object = m_Pool.allocate();
      AddResource(object);
      return object;
    }
    
    void Return(T* object)
    {
      if (object)
      {
        //object->Destroy();
        m_Pool.release(object);
      }
    }
    
    T* Clone(T* copy)
    {
      T* object = new T(*copy);
      AddResource(object);
      return object;
    }
    
    void FlushAllResources()
    {
      std::cout << "---------------------------" << std::endl;
      std::cout << m_Aquired.length() << std::endl;
      for (unsigned int i = 0; i < m_Aquired.length(); i++)
        Return(m_Aquired[i]);
      m_Aquired.clear();
    }
    
    void FlushUnusedResources(void)
    {
      m_Pool.flush();
    }
    
    void AddResource(T* object)
    {
      m_Aquired.append(object);
    }
  
  private:
    ArrayPtr    <T>            m_Aquired;
    GenericPool <T, size, min> m_Pool;
};

#endif