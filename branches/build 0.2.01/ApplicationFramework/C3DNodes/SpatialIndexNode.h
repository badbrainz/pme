#ifndef SPATIALINDEXNODE
#define SPATIALINDEXNODE

#include "SpatialIndexBaseNode.h"

class SpatialIndexNode : public SpatialIndexBaseNode
{
  public:
     SpatialIndexNode();
    ~SpatialIndexNode();
    
    void Accept(SpatialIndexVisitor* visitor);
};

#endif