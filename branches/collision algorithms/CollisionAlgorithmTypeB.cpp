#include "CollisionAlgorithmTypeB.h"

CollisionAlgorithmTypeB::CollisionAlgorithmTypeB()
{
}

// http://www.cs.princeton.edu/courses/archive/fall00/cs426/lectures/raycast/sld018.htm
Tuple4f CollisionAlgorithmTypeB::ComputeRayTriangleIntersection(const Ray &ray, const Tuple3f &p0, const Tuple3f &p1, const Tuple3f &p2)
{
  //Planef    plane;
  //
  //Tuple3f   Ro,
  //          CpRo,
  //          planeN[3];
  //
  //Tuple3f   Cp;

  //Ro = ray.GetOrigin();
  //plane.set(p0,p1,p2);

  //if(-1 == CollisionAlgorithm::CheckAgainstPlaneNormal(Ro-p0,plane.getNormal()))
  //  return Tuple4f(0,0,0,-1);

  //Cp = CollisionAlgorithm::ComputeCollisionPoint(ray,plane,p0);
  //CpRo = Cp-Ro;
  //
  //planeN[0].crossProduct(p0-Ro,p1-Ro).normalize();
  //planeN[1].crossProduct(p1-Ro,p2-Ro).normalize();
  //planeN[2].crossProduct(p2-Ro,p0-Ro).normalize();

  //tanSign = -1 == CollisionAlgorithm::CheckAgainstPlaneNormal(CpRo,planeN[0]) ? -1/*fail*/:
  //          -1 == CollisionAlgorithm::CheckAgainstPlaneNormal(CpRo,planeN[1]) ? -1/*fail*/:
  //          -1 == CollisionAlgorithm::CheckAgainstPlaneNormal(CpRo,planeN[2]) ? -1/*fail*/:
  //           1/*pass*/;

  //return Tuple4f(Cp,tanSign);
  Planef    trianglePlane;
  Tuple3f   Ro,
            vect,
            collisionPoint,
            edge[3],
            planeN[3];
  float     result;
  int       planeDirection;

  Ro = ray.GetOrigin();

  trianglePlane.set(p0, p1, p2);

  planeDirection = CollisionAlgorithm::CheckAgainstPlaneNormal(Ro-p0, trianglePlane.getNormal());

  if(-1 == planeDirection)
    return Tuple4f(0,0,0,-1);

  edge[0] = p0;
  edge[1] = p1;
  edge[2] = p2;

  edge[0] -= Ro;
  edge[1] -= Ro;
  edge[2] -= Ro;

  planeN[0].crossProduct(edge[0], edge[1]).normalize();
  planeN[1].crossProduct(edge[1], edge[2]).normalize();
  planeN[2].crossProduct(edge[2], edge[0]).normalize();

  collisionPoint = CollisionAlgorithm::ComputeCollisionPoint(ray, trianglePlane, p0);

  vect = collisionPoint - Ro;

  result = 0.0 < CollisionAlgorithm::CheckAgainstPlaneNormal(vect, planeN[0]) ? -1/*fail*/:
           0.0 < CollisionAlgorithm::CheckAgainstPlaneNormal(vect, planeN[1]) ? -1/*fail*/:
           0.0 < CollisionAlgorithm::CheckAgainstPlaneNormal(vect, planeN[2]) ? -1/*fail*/:
           1/*pass*/;

  return Tuple4f(collisionPoint, result);
}

CollisionAlgorithmTypeB::~CollisionAlgorithmTypeB()
{
}
