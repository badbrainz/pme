#include "TerrainDatabase.h"

#include "../C3DNodes/TemplatedNodeIterator.h"
#include "../C3DTerrain/TileDescriptor.h"
#include <iostream>
#include <stack>

const int cellSize = 8;

template <class T>
inline int ceilPowerOfTwo(T x)
{
  return (int)pow((T)2, ceil(log10(x)/log10((T)2)));
}

template <class T>
inline int ceilLogBaseTwo(T x)
{
  return (int)(ceil(log10(x)/log10(2.0f)));
}

void TerrainDatabase::Draw(unsigned int level, TileGraphVisitor *visitor)
{
  m_TileGraph.Render(level, visitor);
}

void TerrainDatabase::Cull(TileGraphVisitor *visitor)
{
  visitor->Visit(m_pTrunk);
}

void TerrainDatabase::LoadGameData(const GameFileDescriptor &descriptor)
{
  m_PveObject.LoadFromFile(descriptor.pvePath);
  m_PteObject.LoadFromFile(descriptor.ptePath);
  
  SetupGraphStructure();
  SetupCellDataStructures();ComputeCellBoundaries();
  SetupSpatialIndexStructure();ComputeTreeBoundaries();
}

void TerrainDatabase::SetupGraphStructure(void)
{
  TileModelNode *node             = 0;
  Tuple2f       *coords           = 0;
  Tile          *tile             = 0;
  
  unsigned int  width             = m_PveObject.GetTilesPerX(),
                height            = m_PveObject.GetTilesPerY(),
                texPageIndex      = 0,
                texCoordsIndex    = 0,
                index             = 0,
                texPageID         = 0;
  
  int           primaryTexture    = 0,
                secondaryTexture  = 0;
  
  m_TileGraph.Initialize(2);
  
  for (unsigned int i = 0; i < m_PteObject.GetTexturePageCount(); i++)
  for (unsigned int j = 0; j < 16; j++)
  {
    coords    = m_PteObject.CopySubTextureCoords(j).GetTextureCoords();
    texPageID = m_PteObject.GetTexturePageID(i);
    m_TileGraph.Integrate(0, coords, texPageID);
    m_TileGraph.Integrate(1, coords, texPageID);
  }
  
  for (unsigned int y = 0; y < height; y++)
  for (unsigned int x = 0; x <  width; x++)
  {
    index = y*width+x;
    
    tile             = m_PveObject.GetTile(index);
    primaryTexture   = tile->GetPrimaryTextureID();
    secondaryTexture = tile->GetSecondaryTextureID();
    
    if(-1 == primaryTexture)
			primaryTexture = secondaryTexture;
		if(-1 == secondaryTexture)
			secondaryTexture = primaryTexture;
    
    TileModelController *controller = m_ManagedTileModelControllers.Create();
    controller->SetVertices(m_PveObject.GetTileVertexChunk(index));
    controller->SetColors(m_PveObject.GetTileColorChunk(index));
    controller->SetModel(tile);
    
    texPageIndex     = (int) floor((float)primaryTexture/16);
		texCoordsIndex   = primaryTexture-(texPageIndex*16);
    node = m_TileGraph.ModelGraph(0, texPageIndex*16+texCoordsIndex);
    node->SetController(controller);
    controller->AddTileModelNode(node);
    
    texPageIndex     = (int) floor((float)secondaryTexture/16);
		texCoordsIndex   = secondaryTexture-(texPageIndex*16);
    node = m_TileGraph.ModelGraph(1, texPageIndex*16+texCoordsIndex);
    node->SetController(controller);
    controller->AddTileModelNode(node);
    
    m_Controllers.append(controller);
  }
}

void TerrainDatabase::SetupCellDataStructures(void)
{
  unsigned int  xOff, yOff, zOff, wOff,
                tilesPerX = m_PveObject.GetTilesPerX(),
                tilesPerY = m_PveObject.GetTilesPerY(),
                ratioX    = (int) ceil((float)tilesPerX/cellSize),
                ratioY    = (int) ceil((float)tilesPerY/cellSize);

  for (unsigned int y = 0; y < ratioY; y++)
  for (unsigned int x = 0; x < ratioX; x++)
  {
    xOff = cellSize*x;
    yOff = cellSize*y;
    zOff = clamp(int(cellSize*x+(cellSize-1)), 0, int(tilesPerX-1));
    wOff = clamp(int(cellSize*y+(cellSize-1)), 0, int(tilesPerY-1));
    
    SpatialIndexNode *index = m_ManagedCells.Create();
    index->SetRange(Tuple4i(xOff,yOff,zOff,wOff));
    m_SpatialIndexCells.append(index);
  }
}

void TerrainDatabase::ComputeCellBoundaries(void)
{
  unsigned int    tilesPerX = m_PveObject.GetTilesPerX(),
                  tilesPerY = m_PveObject.GetTilesPerY();

  unsigned int    index,
                  height, width;

  float           maxHeight, minHeight;

  Tuple4i         tileRange;
  Tuple3f         minEnd, maxEnd;

  const Tuple3f *trianglefanVertices;

  for(unsigned int i = 0; i < m_SpatialIndexCells.length(); i++)
  {
    tileRange = m_SpatialIndexCells[i]->GetRange();
    width     = tileRange.z - tileRange.x;
    height    = tileRange.w - tileRange.y;
    maxHeight = 0;
    minHeight = 0;
    
    index               = tileRange.y * tilesPerX + tileRange.x;
    trianglefanVertices = m_PveObject.GetVertexBufferChunk(index);
    minEnd.x            = trianglefanVertices[0].x;
    minEnd.z            = trianglefanVertices[0].z;
    minEnd.y            = 1e3;
    
    index               = tileRange.w * tilesPerX + tileRange.x + width;
    trianglefanVertices = m_PveObject.GetVertexBufferChunk(index);
    maxEnd.x            = trianglefanVertices[8].x;
    maxEnd.z            = trianglefanVertices[8].z;
    maxEnd.y            = -1e3;
    
    for(unsigned int y = 0; y < height+1; y++)
    for(unsigned int x = 0; x < width+1; x++)
    {
      index     = (tileRange.y + y) * tilesPerX + tileRange.x + x;
      maxHeight = m_PveObject.ComputeMaxTileHeight(index);
      minHeight = m_PveObject.ComputeMinTileHeight(index);
      maxEnd.y  = maxHeight > maxEnd.y ? maxHeight : maxEnd.y;
      minEnd.y  = minHeight > minEnd.y ? minEnd.y : minHeight;
    }
    
    BoundsDescriptor descriptor;
    descriptor.computeBounds(minEnd, maxEnd);
    m_SpatialIndexCells[i]->SetBoundsDescriptor(descriptor);
  }
}


void TerrainDatabase::SetupSpatialIndexStructure(void)
{
  SpatialIndexNode   *pointer   = 0,
                     *branch    = 0;
  SpatialIndexNode   *cell      = 0;

  unsigned int        logarithm = 0,
                      boundary  = 0,
                      level     = 0,
                      power     = 0;

  unsigned int        tilesPerX = 0,
                      tilesPerY = 0,
                      cellsPerX = 0,
                      cellsPerY = 0;

  int                 maxBranchSize = 2,
                      offset        = 0;

  Tuple4i branchRange,
          terrainRange;

  std::stack <SpatialIndexNode*> treestack;

  tilesPerX = m_PveObject.GetTilesPerX();
  tilesPerY = m_PveObject.GetTilesPerY();
  cellsPerX = (int) ceil((float)tilesPerX/cellSize);
  cellsPerY = (int) ceil((float)tilesPerY/cellSize);

  terrainRange.set(0, 0, cellsPerX-1, cellsPerY-1);

  boundary  = terrainRange.z > terrainRange.w ? terrainRange.z : terrainRange.w;
  power     = ceilLogBaseTwo((float)boundary+1);
  logarithm = ceilPowerOfTwo((float)boundary+1);

  m_pTrunk = m_ManagedBranches.Create();
  m_pTrunk->SetRange(terrainRange);
  
  BoundsDescriptor descriptor;
  descriptor.computeBounds(Tuple3f(1e3,1e3,1e3),Tuple3f(-1e3,-1e3,-1e3));
  
  for (int y = terrainRange.y; y <= terrainRange.w; y++)
  for (int x = terrainRange.x; x <= terrainRange.z; x++)
    descriptor += m_SpatialIndexCells[y * cellsPerX + x]->GetBoundsDescriptor();
  
  m_pTrunk->UpdateBounds(descriptor);///same as setboundsdescriptor
  treestack.push(m_pTrunk);

  while (!treestack.empty())
  {
    pointer     = treestack.top();
    level       = pointer->GetLevel();
    branchRange = pointer->GetRange();
    
    treestack.pop();
    
    if (level < power-ceilLogBaseTwo((float)maxBranchSize))
    {
      offset = logarithm >> (level+1);
      for (unsigned int i = 0; i < 4; i++)
      {
        Tuple4i point;
        point.y = branchRange.y + (i > 1 ? offset : 0);
        point.x = branchRange.x + (i & 1 ? offset : 0);
        
        if (point.x > terrainRange.z || point.y > terrainRange.w) continue;
        
        point.z = clamp(point.x+offset-1, point.x, terrainRange.z);
        point.w = clamp(point.y+offset-1, point.y, terrainRange.w);
        
        branch = m_ManagedBranches.Create();///create a "branch" node instead
        branch->SetLevel(level+1);
        branch->SetRange(point);
        branch->Attach(pointer);
        treestack.push(branch);
        m_SpatialIndexBranches.append(branch);
      }
    }
    else
    {
      for (int y = branchRange.y; y <= branchRange.w; y++)
      for (int x = branchRange.x; x <= branchRange.z; x++)
      {
        cell = m_SpatialIndexCells[y * cellsPerX + x];
        cell->SetLevel(level+1);
        cell->Attach(pointer);
      }
    }
  }
}

void TerrainDatabase::ComputeTreeBoundaries(void)
{
  unsigned int tilesPerX = m_PveObject.GetTilesPerX();
  unsigned int cellsPerX = (int) ceil((float)tilesPerX/cellSize);

  SpatialIndexNode     *branchpointer = 0;
  Tuple4i               branchrange;
  BoundsDescriptor      descriptor;
  
  std::stack <SpatialIndexNode*> treestack;
  treestack.push(m_pTrunk);

  while (!treestack.empty())
  {
    branchpointer = treestack.top();
    branchrange   = branchpointer->GetRange();
    treestack.pop();
    
    descriptor.reset();
    descriptor.computeBounds(Tuple3f(1e3,1e3,1e3),Tuple3f(-1e3,-1e3,-1e3));
    
    TemplatedNodeIterator <SpatialIndexNode> iter((SpatialIndexNode*)branchpointer->GetFirstChild());
    while (!iter.End())
    {
      for (int y = branchrange.y; y <= branchrange.w; y++)
      for (int x = branchrange.x; x <= branchrange.z; x++)
      {
        SpatialIndexNode *cell = m_SpatialIndexCells[y * cellsPerX + x];
        descriptor += cell->GetBoundsDescriptor();
      }
      iter.Current()->UpdateBounds(descriptor);
      treestack.push(iter.Current());
      iter++;
    }
  }
}

void TerrainDatabase::ControllerVisibility(const Tuple4i &range, bool visible)
{
  unsigned int tilesPerX = m_PveObject.GetTilesPerX();
  for (int y = range.y; y <= range.w; y++)
  for (int x = range.x; x <= range.z; x++)
    m_Controllers[y*tilesPerX+x]->SetVisible(visible);
}

TerrainDatabase::~TerrainDatabase()
{
}
