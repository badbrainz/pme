#include "TileGraph.h"
#include "../C3DTerrain/Tile.h"
#include "../C3DNodes/TemplatedNodeIterator.h"

unsigned int nodeIndices[10]  = {4,3,0,1,2,5,8,7,6,3};
unsigned int nodeIndices1[5]  = {6,0,2,8,6};
unsigned int nodeIndices2[6]  = {4,6,0,2,8,6};
unsigned int nodeIndices3[10] = {4,3,6,7,8,5,2,1,0,3};

TileGraph::TileGraph()
{
};

void TileGraph::Initialize(unsigned int levels)
{
  IndicesNode *indicesNode = 0;
  Node        *graphNode   = 0;

  for (unsigned int i = 0; i < levels; i++)
  {
    graphNode   = m_ManagedGraphNodes.Create();
    indicesNode = m_ManagedIndicesNodes.Create();
    
    m_Levels.append(graphNode);
    indicesNode->SetIndices(nodeIndices);
    indicesNode->Attach(graphNode);
    
    AttachmentPoint ap;
    m_AttatchmentPoints.append(ap);
  }
}

void TileGraph::Trim()
{
  m_ManagedTileModelControllers.FlushUnusedResources();
  m_ManagedTextureCoordsNodes.FlushUnusedResources();
  m_ManagedTileModelNodes.FlushUnusedResources();
  m_ManagedIndicesNodes.FlushUnusedResources();
  m_ManagedTextureNodes.FlushUnusedResources();
  m_ManagedGraphNodes.FlushUnusedResources();
}

void TileGraph::Render(unsigned int level, TileGraphVisitor *visitor)
{
  m_Levels[level]->Accept(visitor);
}

unsigned int TileGraph::Integrate(unsigned int level, Tuple2f *coords, unsigned int texID)
{
  Node* start = m_Levels[level]->GetFirstChild()->GetFirstChild();
  TextureNode *textureNode = (TextureNode*) CheckForRepeat(start, texID);
  
  if (!textureNode)
  {
    textureNode = m_ManagedTextureNodes.Create();
    textureNode->SetTextureID(texID);
    textureNode->Attach(m_Levels[level]->GetFirstChild());
  }
  
  TextureCoordsNode *texCoordsNode = m_ManagedTextureCoordsNodes.Create();
  texCoordsNode->SetTextureCoords(coords);
  texCoordsNode->Attach(textureNode);
  texCoordsNode->Hide();

  m_AttatchmentPoints[level].m_AggregateNodes.append(texCoordsNode);

  return m_AttatchmentPoints[level].m_AggregateNodes.length()-1;
}

TileModelNode* TileGraph::ModelGraph(unsigned int level, unsigned int index)
{
  TileModelNode       *tileModelNode = m_ManagedTileModelNodes.Create();
  TextureCoordsNode   *texCoordsNode = m_AttatchmentPoints[level].m_AggregateNodes[index];
  
  tileModelNode->Attach(texCoordsNode);
  texCoordsNode->Reveal();
  
  return tileModelNode;
}

Node* TileGraph::CheckForRepeat(Node* node, unsigned int identifier)
{
  TemplatedNodeIterator <TextureNode> iter((TextureNode*)node);
  while (!iter.End())
  {
    if (iter.Current()->GetTextureID() == identifier)
      return iter.Current();
    iter++;
  }
  return 0;
}

TileGraph::~TileGraph()
{
};
