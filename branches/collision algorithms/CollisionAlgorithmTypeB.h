#ifndef COLLISIONALGORITHMTYPEB_H
#define COLLISIONALGORITHMTYPEB_H

/**
 * author: kwantum
 */
 
#include "CollisionAlgorithm.h"

class CollisionAlgorithmTypeB : public CollisionAlgorithm
{
  public:
     CollisionAlgorithmTypeB();
    ~CollisionAlgorithmTypeB();

  public:
    Tuple4f ComputeRayTriangleIntersection(const Ray &ray, const Tuple3f &p0, const Tuple3f &p1, const Tuple3f &p2);

  private:
};

#endif
