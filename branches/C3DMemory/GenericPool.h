#ifndef GENERICPOOL
#define GENERICPOOL

#include <list>

template <class T, int count, int min> class GenericPool
{
  public:
    GenericPool(void)
    {
      for (int j = 0; j < count; j++)
        freeList.push_back(new T);
    }
    
    ~GenericPool(void)
    {
      flush();
    }
    
    void flush(void)
    {
      /*std::list <T*>::iterator iter = freeList.begin();
      while (iter != freeList.end())
      {
        delete (*iter);
        iter = freeList.erase(iter);
      }*/
      while (!freeList.empty())
      {
        T* resource = freeList.front(); 
        freeList.pop_front(); 
        delete resource;
      }
    }
    
    T* allocate(void)
    {
      if (freeList.empty())
        grow();
        
      T* resource = freeList.front(); 
      freeList.pop_front();
      if (!resource) resource = new T;
      return resource;
    }
    
    void grow()
    {
      for (int j = 0; j < min; j++)
        freeList.push_front(new T);
    }
    
    void release(T* resource)
    {
      if (!resource) return;
      freeList.push_back(resource);
    }
  
  private:
    std::list <T*> freeList;
};

#endif