#ifndef SPATIALINDEXVISITOR
#define SPATIALINDEXVISITOR

//#include "Visitor.h"

class Node;
class SpatialIndexNode;
class SpatialIndexCell;
class SpatialIndexBaseNode;

class SpatialIndexVisitor// : public Visitor
{
  public:
    virtual void Visit(Node* node){}
    virtual void Visit(SpatialIndexNode *node){}
    virtual void Visit(SpatialIndexCell *cell){}
    virtual void Visit(SpatialIndexBaseNode *node){}
};

#endif