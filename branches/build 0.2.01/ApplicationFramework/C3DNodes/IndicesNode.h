#ifndef INDICESNODE
#define INDICESNODE

#include "Node.h"

class IndicesNode : public Node
{
  public:
     IndicesNode();
    ~IndicesNode();
    
    virtual void Accept(TileGraphVisitor* visitor);
    
    void SetIndices(unsigned int *indices);
    unsigned int *GetIndices();
  
  private:
    unsigned int *m_Indices;
};

#endif