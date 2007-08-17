#ifndef SPATIALINDEXVISITOR
#define SPATIALINDEXVISITOR

//#include "Visitor.h"

class GraphNode;
class SpatialIndexNode;
class SpatialIndexCell;
class SpatialIndexBaseNode;

class SpatialIndexVisitor// : public Visitor
{
  public:
    virtual void Visit(GraphNode* node){}
    virtual void Visit(SpatialIndexNode *node){}
    virtual void Visit(SpatialIndexCell *cell){}
    virtual void Visit(SpatialIndexBaseNode *node){}
};

#endif