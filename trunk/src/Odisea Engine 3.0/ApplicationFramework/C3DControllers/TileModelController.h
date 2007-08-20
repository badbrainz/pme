#ifndef TILEMODELCONTROLLER
#define TILEMODELCONTROLLER

#include "Math/MathUtils.h"
#include "../OC/ocarray.h"

class Tile;
class TileModelNode;

class TileModelController
{
  public:
    void  SetModel(Tile *tile);
    Tile* GetModel();
    
    unsigned short GetModelFlags();
    
    void SetVertices(Tuple3f *vertices);
    Tuple3f* GetVertices();
    
    void SetColors(Tuple4ub *colors);
    Tuple4ub* GetColors();
    
    void SetIndices(unsigned int *indices){}
    
    void SetVisible(bool value);
    
    void SetTileModelNode(TileModelNode *node);
    TileModelNode *GetTileModelNode();
    
    void AddTileModelNode(TileModelNode *node);
    TileModelNode *GetTileModelNode(unsigned int index);
    
    ///
    void SwitchTemp(Tuple4ub *temp);
    void SwitchBack();
  
  private:
    TileModelNode *m_pNode;
    Tile          *m_pTileModel;
    Tuple3f       *m_Vertices;
    Tuple4ub      *m_Colors;
    Tuple4ub      *m_TempColors;
    
    ArrayPtr <TileModelNode> m_ModelNodes;
};

#endif