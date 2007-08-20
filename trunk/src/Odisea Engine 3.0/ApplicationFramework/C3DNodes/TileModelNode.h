#ifndef TILEMODELNODE
#define TILEMODELNODE

#include "GraphNode.h"

class TileModelController;

class TileModelNode : public GraphNode
{
  public:
     TileModelNode();
    ~TileModelNode();
    
    virtual void Accept(TileGraphVisitor* visitor);
  
  public:
    void SetController(TileModelController *controller);
    TileModelController* GetController();
  
  private:
    TileModelController* m_pController;
};

#endif