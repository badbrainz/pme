#ifndef SPATIALINDEXVISITOR
#define SPATIALINDEXVISITOR

#include "TileGraphVisitor.h"

class SpatialIndexNode;
class SpatialIndexCell;

class SpatialIndexVisitor : public TileGraphVisitor
{
  public:
    virtual void Visit(SpatialIndexNode  *node);
    virtual void Visit(SpatialIndexCell  *cell);
};

#endif