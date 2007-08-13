#ifndef TILEGRAPHRENDERERVISITOR
#define TILEGRAPHRENDERERVISITOR

#include "TileGraphVisitor.h"

class TileGraphRendererVisitor : public TileGraphVisitor
{
  public:
    TileGraphRendererVisitor();
    
    void Visit(GraphNode* node);
    void Visit(TextureNode* node);
    void Visit(IndicesNode* node);
    void Visit(TileModelNode* node);
    void Visit(TextureCoordsNode* node);
    void Visit(BlendAttributesNode* node);
    
    void EnableBlend(bool enable);
  
  private:
    void TransformTextureMatrix(unsigned short flags);
    void UnTransformTextureMatrix();
  
  private:
    unsigned int *m_pIndices;
    bool          enableBlend;
};

#endif