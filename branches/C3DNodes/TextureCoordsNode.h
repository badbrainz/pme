#ifndef TEXTURECOORDSNODE
#define TEXTURECOORDSNODE

#include "Node.h"
#include "Math/MathUtils.h"

class TextureCoordsNode : public Node
{
  public:
     TextureCoordsNode();
    ~TextureCoordsNode();
    
    TextureCoordsNode(const TextureCoordsNode &copy);
    TextureCoordsNode& operator = (const TextureCoordsNode &right);
    bool operator == (const TextureCoordsNode &right);
    
    virtual void Accept(TileGraphVisitor* visitor);
    
    void SetTextureCoords(Tuple2f *coords);
    Tuple2f* GetTextureCoords();
    
    void Destroy(){texCoords=0;}
  
  private:
    Tuple2f *texCoords;
};

#endif