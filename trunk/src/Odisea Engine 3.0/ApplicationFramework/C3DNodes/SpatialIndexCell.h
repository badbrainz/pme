#ifndef SPATIALINDEXCELL
#define SPATIALINDEXCELL

#include "SpatialIndexNode.h"

class SpatialIndexCell : public SpatialIndexNode
{
  public:
    ~SpatialIndexCell(){}
    virtual void Accept(TileGraphVisitor* visitor);
};

#endif