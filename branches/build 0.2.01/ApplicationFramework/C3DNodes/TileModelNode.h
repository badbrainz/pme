#ifndef TILEMODELNODE
#define TILEMODELNODE

#include "Node.h"

class TileModelController;

class TileModelNode : public Node
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