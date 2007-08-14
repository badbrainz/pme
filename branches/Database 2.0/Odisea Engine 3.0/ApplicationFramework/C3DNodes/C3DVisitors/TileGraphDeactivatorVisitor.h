#ifndef TILEGRAPHDEACTIVATORVISITOR
#define TILEGRAPHDEACTIVATORVISITOR

#include "TileGraphVisitor.h"

class TileGraphDeactivatorVisitor : public TileGraphVisitor
{
  public:
    void Visit(GraphNode* node);
    void Visit(TextureNode* node);
    void Visit(IndicesNode* node);
    void Visit(TileModelNode* node);
    void Visit(TextureCoordsNode* node);
    void Visit(BlendAttributesNode* node);
};

#endif