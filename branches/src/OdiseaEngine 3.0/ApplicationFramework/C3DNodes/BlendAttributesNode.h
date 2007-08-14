#ifndef BLENDATTRIBUTESNODE
#define BLENDATTRIBUTESNODE

#include "GraphNode.h"

class BlendAttributesNode : public GraphNode
{
  public:
     BlendAttributesNode();
    ~BlendAttributesNode();
    
    virtual void Accept(TileGraphVisitor* visitor);
    
    void SetAttributes();
};

#endif