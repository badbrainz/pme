#ifndef INDICESNODE
#define INDICESNODE

#include "GraphNode.h"

class IndicesNode : public GraphNode
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