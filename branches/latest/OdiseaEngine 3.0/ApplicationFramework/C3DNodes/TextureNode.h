#ifndef TEXTURENODE
#define TEXTURENODE

#include "GraphNode.h"

class TextureNode : public GraphNode
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