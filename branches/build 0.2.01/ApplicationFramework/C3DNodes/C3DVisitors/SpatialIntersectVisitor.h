#ifndef SPATIALINTERSECTVISITOR
#define SPATIALINTERSECTVISITOR

#include "TileGraphVisitor.h"

class Ray3D;
class SpatialIndexNode;
class SpatialIndexCell;

class SpatialIntersectVisitor : public TileGraphVisitor
{
  public:
    void SetRay(Ray3D *ray);
    
    void Visit(SpatialIndexNode  *node);
    void Visit(SpatialIndexCell  *cell);
  
  private:
    bool Intersect(BoundsDescriptor *bounds);
  
  private:
    Ray3D *m_pRay;
};

#endif