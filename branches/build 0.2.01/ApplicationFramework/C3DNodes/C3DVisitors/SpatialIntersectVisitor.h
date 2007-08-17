#ifndef SPATIALINTERSECTVISITOR
#define SPATIALINTERSECTVISITOR

//#include "TileGraphVisitor.h"
#include "SpatialIndexVisitor.h"

class Ray3D;
class SpatialIndexBaseNode;
class SpatialIndexNode;
class SpatialIndexCell;
class BoundsDescriptor;
class TerrainDatabase;

class SpatialIntersectVisitor : public SpatialIndexVisitor
{
  public:
    SpatialIntersectVisitor();
    void SetRay(Ray3D *ray);
    void SetTerrain(TerrainDatabase *terrain);
    
    void Visit(SpatialIndexBaseNode  *base);
    void Visit(SpatialIndexNode  *node);
    void Visit(SpatialIndexCell  *cell);
  
  private:
    bool Intersect(BoundsDescriptor *bounds);
  
  private:
    Ray3D *m_pRay;
    TerrainDatabase *m_pTerrainDatabase;
};

#endif