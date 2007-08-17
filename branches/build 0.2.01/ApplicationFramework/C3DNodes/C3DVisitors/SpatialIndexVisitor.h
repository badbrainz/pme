#ifndef SPATIALINDEXVISITOR
#define SPATIALINDEXVISITOR

#include "TileGraphVisitor.h"

class SpatialIndexBaseNode;
class SpatialIndexNode;
class SpatialIndexCell;

class SpatialIndexVisitor : public TileGraphVisitor
{
  public:
    void Visit(SpatialIndexBaseNode  *base);
    void Visit(SpatialIndexNode  *node);
    void Visit(SpatialIndexCell  *cell);
};

#endif