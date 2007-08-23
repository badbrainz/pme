#pragma once

//#include "TileGraphVisitor.h"
#include "SpatialIndexVisitor.h"

class SpatialIndexBaseNode;
class SpatialIndexNode;
class SpatialIndexCell;
class TerrainDatabase;
class Frustum;

class SpatialVisibilityVisitor : public SpatialIndexVisitor
{
  public:
    SpatialVisibilityVisitor(){}
    void SetFrustum(Frustum *frustumpointer);
    void SetTerrain(TerrainDatabase *terrain);
    
    void Visit(SpatialIndexBaseNode  *base);
    void Visit(SpatialIndexNode  *node);
    void Visit(SpatialIndexCell  *cell);

  private:
    Frustum         *m_pFrustum;
    TerrainDatabase *m_pTerrainDatabase;
};