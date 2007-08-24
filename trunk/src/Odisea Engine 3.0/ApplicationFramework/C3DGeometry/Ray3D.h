#ifndef RAY3D
#define RAY3D

#include "Math/MathUtils.h"

class Ray3D
{
  public:
     Ray3D();
     Ray3D(const Tuple3f &Po, const Tuple3f &Pd);
    ~Ray3D();

  public:
    void            SetOrigin(const Tuple3f &Po);
    void            SetDestination(const Tuple3f &Pd);
    void            Set(const Tuple3f &Po, const Tuple3f &Pd);
    
    float           GetLength(void);
    const Tuple3f  &GetOrigin(void) const;
    const Tuple3f  &GetDirection(void) const;
    const Tuple3f  &GetDestination(void) const;
    
    void            NormalizeDirection(void);
  
  private:
    Tuple3f origin,
            destination;
};

#endif