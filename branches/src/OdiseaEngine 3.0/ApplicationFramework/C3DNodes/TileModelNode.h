#ifndef TILEMODELNODE
#define TILEMODELNODE

#include "GraphNode.h"
//#include "../C3DTerrain/Tile.h"

class TileModelController;

class TileModelNode : public GraphNode//, public Tile
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