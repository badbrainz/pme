#ifndef COLLISIONALGORITHM_H
#define COLLISIONALGORITHM_H

/**
 * author: kwantum
 */

#include "../Engine/Geometry/Plane.h"
#include "Ray.h"

enum CollisionState
{
  NOT_COLLIDED = 0,
  COLLIDED
};

enum Sign
{
  NEGATIVE = -1,
  POSITIVE =  1
};

class CollisionAlgorithm
{
  public:
     CollisionAlgorithm();
    ~CollisionAlgorithm();

  public:
    virtual Tuple4f ComputeRayTriangleIntersection(const Ray &ray, const Tuple3f &p0, const Tuple3f &p1, const Tuple3f &p2);
    virtual Tuple3f ComputeCollisionPoint(const Ray &ray, const Planef &plane, const Tuple3f &point);
    virtual int     CheckAgainstPlaneNormal(const Tuple3f &vector, const Tuple3f &Pn);
    virtual int     CheckAgainstTriangleBounds(const Tuple4f &cp, const Tuple3f &p0, const Tuple3f &p1, const Tuple3f &p2);
    virtual int     CheckAgainstTriangleBounds(const Tuple3f &cp, const Tuple3f &p0, const Tuple3f &p1, const Tuple3f &p2);

  private:
    double pi;
};

#endif
