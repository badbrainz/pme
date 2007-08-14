#ifndef TEXTURECOORDSNODE
#define TEXTURECOORDSNODE

#include "GraphNode.h"
#include "Math/MathUtils.h"

class TextureCoordsNode : public GraphNode
{
  public:
     TextureCoordsNode();
    ~TextureCoordsNode();
    
    virtual void Accept(TileGraphVisitor* visitor);
    
    void SetTextureCoords(Tuple2f *coords);
    Tuple2f* GetTextureCoords();
  //int id;
  private:
    Tuple2f *texCoords;
};

#endif