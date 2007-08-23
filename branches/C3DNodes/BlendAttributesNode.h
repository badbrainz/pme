#ifndef BLENDATTRIBUTESNODE
#define BLENDATTRIBUTESNODE

#include "Node.h"

class BlendAttributesNode : public Node
{
  public:
     BlendAttributesNode();
    ~BlendAttributesNode();
    
    virtual void Accept(TileGraphVisitor* visitor);
    
    void SetAttributes();
};

#endif