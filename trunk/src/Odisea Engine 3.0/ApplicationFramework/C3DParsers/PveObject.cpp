#include "PveObject.h"
#include "Managers/MediaPathManager.h"
#include <algorithm>

PveObject::PveObject()
{
  m_pUncompressedVerts = 0;
  m_pCompressedYBuffer = 0;
  m_pColorBuffer       = 0;
  tileVertexBlock      = 0;
  tileIndexSet         = 0;
  tileSet              = 0;
  
  tilesPerX            = 0;
  tilesPerY            = 0;
  maxHeight            = 0.0f;
  minHeight            = 0.0f;
  elevate              = 1.0f;
  heightRatio          = 1.0f;
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
    index = tileIndexSet[ tileIndex ].indices[ b ];
    m_TempVertexBuffer[ b ].z = m_pUncompressedVerts[ index ].x;
    m_TempVertexBuffer[ b ].y = m_pUncompressedVerts[ index ].y * elevate;
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
    index = tileIndexSet[ tileIndex ].indices[ b ];
    m_TempColorBuffer[ b ] = m_pColorBuffer[ index ];
  }

  return m_TempColorBuffer;
}

///tiles
Tile *PveObject::GetTile(int tileIndex)
{
  return &tileSet[tileIndex];
}

const unsigned int *PveObject::GetTileVertexIndices(int tileIndex)
{
  return tileIndexSet[tileIndex].indices;
}

Tuple3f *PveObject::GetTileVertexChunk(int tileIndex)
{
  return tileVertexBlock[tileIndex].vertices;
}

Tuple4ub *PveObject::GetTileColorChunk(int tileIndex)
{
  return tileVertexBlock[tileIndex].colors;
}

///accessors
/*void PveObject::SetColorBufferChunk(int tileIndex, const Tuple4ub *colors)
{
  unsigned int vertexIndex;
  for(int b = 0; b < 9; b++)
  {
    vertexIndex = tileIndexSet[ tileIndex ].indices[ b ];
    m_pColorBuffer[ vertexIndex ] = colors[b];
  }
}*/

float PveObject::GetMaxHeight(void)
{
  return maxHeight;
}

float PveObject::GetMinHeight(void)
{
  return minHeight;
}

float PveObject::ComputeMaxTileHeight(unsigned int index)
{
  float max,
        element;

  max = -100;

  for(int i = 0; i < 9; i++)
  {
    element = m_pUncompressedVerts[tileIndexSet[index].indices[i]].y;
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
    element = m_pUncompressedVerts[tileIndexSet[index].indices[i]].y;
    min     =  min > element ? element : min;
  }

  return min;
}

void PveObject::ComputeHeightRatio(void)
{
  heightRatio = (maxHeight-minHeight) / 255.0f;
}

float PveObject::GetHeightRatio(void)
{
  return heightRatio;
}

unsigned int PveObject::GetTilesPerX(void)
{
  return tilesPerX;
}

unsigned int PveObject::GetTilesPerY(void)
{
  return tilesPerY;
}

void PveObject::SetElevation(float elevation)
{
  elevate = elevation;
}

PveObject::~PveObject()
{
  //memoryBlock.PrintMemoryBlock("memory.pba");
  //memoryBlock.PrintMemoryProfile("memory.txt");
}

/*void PveObject::ComputeTileMinHeights(void)
{
  unsigned int width  = tilesPerX,
               height = tilesPerY;

  int arrayWidth      = width  * 2 + 1,
      arrayHeight     = height * 2 + 1,
      xOff            = 0,
      yOff            = 0,
      index           = 0;

  float oldHeight     = 0,
        newHeight     = 0;

  // iterate over tiles
  for(unsigned int a = 0; a < height; a++, yOff += 2, xOff = 0)
  for(unsigned int b = 0; b < width; b++, xOff += 2, newHeight = 0)
  {
    index = a * width + b;
    
    // iterate over tile's vertices
    for(unsigned int y = 0; y < 3; y++)
    for(unsigned int x = 0; x < 3; x++)
    {
      index     = y * arrayWidth + x;
      newHeight = m_pUncompressedVerts[index].y;
      newHeight = newHeight < oldHeight ? newHeight : oldHeight;
    }
    
    tileSet[index].SetMinHeight(newHeight);
  }
}

void PveObject::ComputeTileMaxHeights(void)
{
  unsigned int width  = tilesPerX,
               height = tilesPerY;

  int arrayWidth      = width  * 2 + 1,
      arrayHeight     = height * 2 + 1,
      xOff            = 0,
      yOff            = 0,
      index           = 0,
      vertIndex       = 0;

  float test;

  for(unsigned int a = 0; a < height; a++, yOff += 2, xOff = 0)
  for(unsigned int b = 0; b < width; b++, xOff += 2, test = 0)
  {
    index = a * width + b;

    test = vertexData[tileIndexSet[index].vertexIndices[0]].y;

    for(int c = 0; c < 9; c++)
      test =  test < vertexData[tileIndexSet[index].vertexIndices[c]].y ? vertexData[tileIndexSet[index].vertexIndices[c]].y : test;

    tileSet[index].SetMaxHeight(test);
  }
}*/