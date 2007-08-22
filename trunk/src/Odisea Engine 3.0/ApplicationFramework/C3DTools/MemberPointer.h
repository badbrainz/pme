#ifndef MEMBERPOINTER
#define MEMBERPOINTER

template <class T>
class MemberPointer
{
  public:
    MemberPointer(T *p = 0) : m_pObject(p)
    {}
    
    ~MemberPointer()
    {
      delete m_pObject;
    }
    
    T& operator * ()
    { 
      return *m_pObject; 
    }
    
    T* operator -> ()
    { 
      return m_pObject; 
    }
    
    T* Get()
    {
      return m_pObject;
    }
    
    T* Release()
    {
      T *old = m_pObject;
      m_pObject = 0;
      return old;
    }
    
    void Reset(T *p = 0)
    {
      delete m_pObject;
      m_pObject = p;
    }
  
  protected:
    void operator = (const MemberPointer<T>& right)
    {}
    
    MemberPointer(const MemberPointer<T>& right)
    {}
  
  protected:
    T* m_pObject;
};

#endif