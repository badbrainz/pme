#include "TileModelController.h"
#include "Math/MathUtils.h"
#include "../C3DTerrain/Tile.h"
#include "../C3DNodes/TileModelNode.h"

TileModelController::TileModelController()
{
  m_pTileModel = 0;
  m_TempColors = 0;
  m_Vertices   = 0;
  m_Colors     = 0;
  m_pNode      = 0;
}

TileModelController::TileModelController(const TileModelController& copy)
{
  m_pTileModel = copy.m_pTileModel;
  m_TempColors = copy.m_TempColors;
  m_Vertices   = copy.m_Vertices;
  m_Colors     = copy.m_Colors;
  m_pNode      = copy.m_pNode;

  m_ModelNodes.clear();
  
  for (unsigned int i = 0; i < m_ModelNodes.length(); i++)
    m_ModelNodes.append(copy.m_ModelNodes[i]);
}

void TileModelController::SwitchTemp(Tuple4ub *temp)
{
  m_TempColors = m_Colors;
  m_Colors = temp;
}

void TileModelController::SwitchBack()
{
  m_Colors = m_TempColors;
}

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
  m_TempColors = m_Colors;
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
  }
  else
  {
    for (unsigned int j = 0; j < length; j++)
    m_ModelNodes[j]->Hide();
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

void TileModelController::Destroy()
{
  m_pNode       = 0;
  m_pTileModel  = 0;
  m_Vertices    = 0;
  m_Colors      = 0;
  m_TempColors  = 0;
  m_ModelNodes.clearFast();
} 

TileModelController::~TileModelController()
{
}
