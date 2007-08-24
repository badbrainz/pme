#include "PveObject.h"
#include "Managers/MediaPathManager.h"
#include <algorithm>

PveObject::PveObject()
{
  m_pUncompressedVerts = 0;
  m_pCompressedYBuffer = 0;
  m_pTileVertexBlock   = 0;
  m_pColorBuffer       = 0;
  m_pTileIndexSet      = 0;
  m_pTileSet           = 0;
  
  m_uiTilesPerX        = 0;
  m_uiTilesPerY        = 0;
  m_fHeightRatio       = 1.0f;
  m_fMaxHeight         = 0.0f;
  m_fMinHeight         = 0.0f;
  m_fElevate           = 1.0f;
}

void PveObject::Flush()
{
  if (m_pUncompressedVerts)
  delete [] m_pUncompressedVerts;
  
  if (m_pCompressedYBuffer)
  delete [] m_pCompressedYBuffer;
  
  if (m_pTileVertexBlock)
  delete [] m_pTileVertexBlock;
  
  if (m_pTileIndexSet)
  delete [] m_pTileIndexSet;
  
  if (m_pColorBuffer)
  delete [] m_pColorBuffer;
  
  if (m_pTileSet)
  delete [] m_pTileSet;
  
  m_pUncompressedVerts = 0;
  m_pCompressedYBuffer = 0;
  m_pTileVertexBlock   = 0;
  m_pColorBuffer       = 0;
  m_pTileIndexSet      = 0;
  m_pTileSet           = 0;
  
  m_uiTilesPerX        = 0;
  m_uiTilesPerY        = 0;
  m_fHeightRatio       = 1.0f;
  m_fMaxHeight         = 0.0f;
  m_fMinHeight         = 0.0f;
  m_fElevate           = 1.0f;
}

///buffers
const Tuple3f *PveObject::GetVertexBuffer(void)
{
  return m_pUncompressedVerts;
}

const Tuple3f *PveObject::GetVertexBufferChunk(int tileIndex)
{
  unsigned int index;
  unsigned int size = 9*sizeof(float);
  
  for(int b = 0; b < 9; b++)
  {
    index = m_pTileIndexSet[ tileIndex ].indices[ b ];
    m_TempVertexBuffer[ b ].z = m_pUncompressedVerts[ index ].x;
    m_TempVertexBuffer[ b ].y = m_pUncompressedVerts[ index ].y * m_fElevate;
    m_TempVertexBuffer[ b ].x = m_pUncompressedVerts[ index ].z;
  }

  return m_TempVertexBuffer;
}

const Tuple4ub *PveObject::GetColorBuffer(void)
{
  return m_pColorBuffer;
}

const Tuple4ub *PveObject::GetColorBufferChunk(int tileIndex)
{
  unsigned int index;
  for(int b = 0; b < 9; b++)
  {
    index = m_pTileIndexSet[ tileIndex ].indices[ b ];
    m_TempColorBuffer[ b ] = m_pColorBuffer[ index ];
  }

  return m_TempColorBuffer;
}

///tiles
Tile *PveObject::GetTile(int tileIndex)
{
  return &m_pTileSet[tileIndex];
}

const unsigned int *PveObject::GetTileVertexIndices(int tileIndex)
{
  return m_pTileIndexSet[tileIndex].indices;
}

Tuple3f *PveObject::GetTileVertexChunk(int tileIndex)
{
  return m_pTileVertexBlock[tileIndex].vertices;
}

Tuple4ub *PveObject::GetTileColorChunk(int tileIndex)
{
  return m_pTileVertexBlock[tileIndex].colors;
}

///accessors
float PveObject::GetMaxHeight(void)
{
  return m_fMaxHeight;
}

float PveObject::GetMinHeight(void)
{
  return m_fMinHeight;
}

float PveObject::ComputeMaxTileHeight(unsigned int index)
{
  float max = -100,
        element;

  for(int i = 0; i < 9; i++)
  {
    element = m_pUncompressedVerts[m_pTileIndexSet[index].indices[i]].y;
    max     =  max > element ? max : element;
  }

  return max;
}

float PveObject::ComputeMinTileHeight(unsigned int index)
{
  float min = 100,
        element;

  for(int i = 0; i < 9; i++)
  {
    element = m_pUncompressedVerts[m_pTileIndexSet[index].indices[i]].y;
    min     =  min > element ? element : min;
  }

  return min;
}

void PveObject::ComputeHeightRatio(void)
{
  m_fHeightRatio = (m_fMaxHeight-m_fMinHeight) / 255.0f;
}

float PveObject::GetHeightRatio(void)
{
  return m_fHeightRatio;
}

unsigned int PveObject::GetTilesPerX(void)
{
  return m_uiTilesPerX;
}

unsigned int PveObject::GetTilesPerY(void)
{
  return m_uiTilesPerY;
}

void PveObject::SetElevation(float elevation)
{
  m_fElevate = elevation;
}

PveObject::~PveObject()
{
  Flush();
}
