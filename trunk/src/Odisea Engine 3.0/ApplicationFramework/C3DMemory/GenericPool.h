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
      std::list <T*>::iterator iter = freeList.begin();
      while (iter != freeList.end())
      {
        delete (*iter);
        iter = freeList.erase(iter);
      }
    }
    
    void flush(void)
    {
      std::list <T*>::iterator iter = freeList.begin();
      while (iter != freeList.end())
      {
        delete (*iter);
        iter = freeList.erase(iter);
      }
    }
    
    T* allocate(void)
    {
      if (freeList.size() > 0)
      {
        T* resource = freeList.front(); 
        freeList.pop_front(); 
        return resource;
      }
      else
      {
        grow();
        return allocate();
      }
    }
    
    void grow()
    {
      for (int j = 0; j < min; j++)
        freeList.push_back(new T);
    }
    
    void release(T* resource)
    {
      freeList.push_back(resource);
    }
  
  private:
    std::list <T*> freeList;
};

#endif