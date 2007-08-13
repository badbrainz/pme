#ifndef COLLISIONALGORITHMTYPEA_H
#define COLLISIONALGORITHMTYPEA_H

/**
 * author: kwantum
 */
 
#include "CollisionAlgorithm.h"

class CollisionAlgorithmTypeA : public CollisionAlgorithm
{
  public:
     CollisionAlgorithmTypeA();
    ~CollisionAlgorithmTypeA();

  public:
    Tuple4f ComputeRayTriangleIntersection(const Ray &ray, const Tuple3f &p0, const Tuple3f &p1, const Tuple3f &p2);
};

/*
typedef unsigned int uint32;
#define in(a) ((uint32&) a)
bool CheckPointInTriangle(const VECTOR& point, const VECTOR& pa, const VECTOR& pb, const VECTOR& pc)
{
    VECTOR e10=pb-pa;
    VECTOR e20=pc-pa;
    float a,b,c,ac_bb;

    a = e10.dot(e10);
    b = e10.dot(e20);
    c = e20.dot(e20);
    ac_bb=(a*c)-(b*b);

    VECTOR vp(point.x-pa.x, point.y-pa.y, point.z-pa.z);

    float d = vp.dot(e10);
    float e = vp.dot(e20);

    float x = (d*c)-(e*b);
    float y = (e*a)-(d*b);
    float z = x+y-ac_bb;

    return (( in(z)& ~(in(x)|in(y)) ) & 0x80000000);
}
*/

#endif
