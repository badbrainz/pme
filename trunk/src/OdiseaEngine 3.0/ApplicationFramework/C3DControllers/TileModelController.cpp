#include "TileModelController.h"
#include "Math/MathUtils.h"
#include "../C3DTerrain/Tile.h"
#include "../C3DNodes/TileModelNode.h"

void TileModelController::SetModel(Tile *tile)
{
  m_pTileModel = tile;
}

unsigned short TileModelController::GetModelFlags()
{
  return m_pTileModel->GetFlags();
}

void TileModelController::SetVertices(Tuple3f *vertices)
{
  m_Vertices = vertices;
}

Tuple3f* TileModelController::GetVertices()
{
  return m_Vertices;
}

void TileModelController::SetColors(Tuple4ub *colors)
{
  m_Colors = colors;
}

Tuple4ub* TileModelController::GetColors()
{
  return m_Colors;
}

void TileModelController::SetVisible(bool value)
{
  unsigned int length = m_ModelNodes.length();
  if (value)
  {
    for (unsigned int i = 0; i < length; i++)
    m_ModelNodes[i]->Reveal();
    //m_pNode->Reveal();
  }
  else
  {
    for (unsigned int j = 0; j < length; j++)
    m_ModelNodes[j]->Hide();
    //m_pNode->Hide();
  }
}

void TileModelController::SetTileModelNode(TileModelNode *node)
{
  m_pNode = node;
}

TileModelNode *TileModelController::GetTileModelNode()
{
  return m_pNode;
}

Tile* TileModelController::GetModel()
{
  return m_pTileModel;
}

void TileModelController::AddTileModelNode(TileModelNode *node)
{
  m_ModelNodes.append(node);
}

TileModelNode *TileModelController::GetTileModelNode(unsigned int index)
{
  return m_ModelNodes[index];
}
