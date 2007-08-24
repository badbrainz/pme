#ifndef TEXTURENODE
#define TEXTURENODE

#include "Node.h"

class TextureNode : public Node
{
  public:
     TextureNode();
    ~TextureNode();
    
    virtual void Accept(TileGraphVisitor* visitor);
    
    void SetTextureID(unsigned int id);
    unsigned int GetTextureID();
  
  private:
    unsigned int textureID;
};

#endif