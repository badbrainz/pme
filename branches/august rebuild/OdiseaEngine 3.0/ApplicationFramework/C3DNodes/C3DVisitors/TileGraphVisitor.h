#ifndef TILEGRAPHVISITOR
#define TILEGRAPHVISITOR

#include "Visitor.h"

class GraphNode;
class IndicesNode;
class TextureNode;
class TileModelNode;
class TextureCoordsNode;
class BlendAttributesNode;

class SpatialIndexNode;
class SpatialIndexCell;

class TileGraphVisitor
{
  public:
    /*virtual void Visit(GraphNode* node)           = 0;
    virtual void Visit(TextureNode* node)         = 0;
    virtual void Visit(IndicesNode* node)         = 0;
    virtual void Visit(TileModelNode* node)       = 0;
    virtual void Visit(TextureCoordsNode* node)   = 0;
    virtual void Visit(BlendAttributesNode* node) = 0;*/
    
    virtual void Visit(GraphNode* node){}
    virtual void Visit(TextureNode* node){}
    virtual void Visit(IndicesNode* node){}
    virtual void Visit(TileModelNode* node){}
    virtual void Visit(TextureCoordsNode* node){}
    virtual void Visit(BlendAttributesNode* node){}
    
    virtual void Visit(SpatialIndexNode  *node){}
    virtual void Visit(SpatialIndexCell  *cell){}
};

#endif