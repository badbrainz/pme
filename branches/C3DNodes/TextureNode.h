#ifndef TEXTURENODE
#define TEXTURENODE

#include "Node.h"

class TextureNode : public Node
{
  public:
     TextureNode();
    ~TextureNode();
    
    TextureNode(const TextureNode &copy);
    TextureNode& operator = (const TextureNode &right);
    bool operator == (const TextureNode &right);
    
    virtual void Accept(TileGraphVisitor* visitor);
    
    void SetTextureID(unsigned int id);
    unsigned int GetTextureID();
  
  private:
    unsigned int textureID;
};

#endif