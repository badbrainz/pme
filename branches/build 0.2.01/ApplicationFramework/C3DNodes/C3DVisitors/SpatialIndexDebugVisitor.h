#ifndef SPATIALINDEXDEBUGVISITOR
#define SPATIALINDEXDEBUGVISITOR

//#include "TileGraphVisitor.h"
#include "SpatialIndexVisitor.h"

//class SpatialIndexBaseNode;
//class SpatialIndexNode;
//class SpatialIndexCell;

class SpatialIndexDebugVisitor : public SpatialIndexVisitor
{
  public:
    void Visit(SpatialIndexBaseNode  *base);
    void Visit(SpatialIndexNode  *node);
    void Visit(SpatialIndexCell  *cell);
};

#endif