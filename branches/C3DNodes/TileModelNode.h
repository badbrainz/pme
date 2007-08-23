#ifndef TILEMODELNODE
#define TILEMODELNODE

#include "Node.h"

class TileModelController;

class TileModelNode : public Node
{
  public:
     TileModelNode();
    ~TileModelNode();
    
    TileModelNode(const TileModelNode &copy);
    TileModelNode& operator = (const TileModelNode &right);
    bool operator == (const TileModelNode &right);
    
    virtual void Accept(TileGraphVisitor* visitor);
    
    void Destroy(){m_pController=0;}
  
  public:
    void SetController(TileModelController *controller);
    TileModelController* GetController();
  
  private:
    TileModelController* m_pController;
};

#endif