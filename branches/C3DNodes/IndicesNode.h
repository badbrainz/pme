#ifndef INDICESNODE
#define INDICESNODE

#include "Node.h"

class IndicesNode : public Node
{
  public:
     IndicesNode();
    ~IndicesNode();
    
    IndicesNode(const IndicesNode &copy);
    IndicesNode& operator = (const IndicesNode &right);
    bool operator == (const IndicesNode &right);
    
    virtual void Accept(TileGraphVisitor* visitor);
    
    void SetIndices(unsigned int *indices);
    unsigned int *GetIndices();
    
    void Destroy(){m_Indices=0;}
  
  private:
    unsigned int *m_Indices;
};

#endif