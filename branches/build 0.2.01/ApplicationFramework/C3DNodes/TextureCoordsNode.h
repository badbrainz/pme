#ifndef TEXTURECOORDSNODE
#define TEXTURECOORDSNODE

#include "Node.h"
#include "Math/MathUtils.h"

class TextureCoordsNode : public Node
{
  public:
     TextureCoordsNode();
    ~TextureCoordsNode();
    
    virtual void Accept(TileGraphVisitor* visitor);
    
    void SetTextureCoords(Tuple2f *coords);
    Tuple2f* GetTextureCoords();
  
  private:
    Tuple2f *texCoords;
};

#endif