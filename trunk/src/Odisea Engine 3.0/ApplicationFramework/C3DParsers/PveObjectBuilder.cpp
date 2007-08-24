#include "PveObject.h"

const float tilesize = 0.5f;

void PveObject::InitializeMemoryBlock(void)
{
}

void PveObject::BuildTerrainVertexField(void)
{
  unsigned int width  = m_uiTilesPerX*2+1;
  unsigned int height = m_uiTilesPerY*2+1;
  unsigned int index	= 0;

  float xPos = 0;
  float yPos = 0;

  m_pUncompressedVerts = new Tuple3f[width * height];

  for (unsigned int row = 0; row < height; row++, xPos = 0, yPos += tilesize)
  for (unsigned int col = 0; col < width; col++)
  {
    index               = row * width + col;
    m_pUncompressedVerts[index].x = xPos;
    m_pUncompressedVerts[index].y = m_fHeightRatio * m_pCompressedYBuffer[index] + m_fMinHeight;
    m_pUncompressedVerts[index].z = yPos;
    xPos += tilesize;
  }
}

void PveObject::BuildTileVertexBlock(void)
{
  unsigned int size = m_uiTilesPerX*m_uiTilesPerY*sizeof(TileVertexBlock);
  const Tuple3f  *vchunk = 0;
  const Tuple4ub *cchunk = 0;

  m_pTileVertexBlock = new TileVertexBlock[m_uiTilesPerX*m_uiTilesPerY];

  for (unsigned int i = 0; i < m_uiTilesPerX*m_uiTilesPerY; i++)
  {
    vchunk = GetVertexBufferChunk(i);
    cchunk = GetColorBufferChunk(i);
    for (unsigned int j = 0; j < 9; j++)
    {
      m_pTileVertexBlock[i].vertices[j] = vchunk[j];
      m_pTileVertexBlock[i].colors[j]   = cchunk[j];
    }
  }
}

void PveObject::SetupTileIndices(void)
{
  unsigned int arrayWidth	= m_uiTilesPerX*2+1;
  unsigned int width  		= m_uiTilesPerX,
               height 		= m_uiTilesPerY;
  unsigned int xOff = 0, yOff = 0, index = 0, i = 0;

  m_pTileIndexSet = new TileIndexSet[width * height];

  for (unsigned int a = 0; a < m_uiTilesPerY; a++, yOff += 2, xOff = 0)
  for (unsigned int b = 0; b < m_uiTilesPerX; b++, xOff += 2, index = 0, i++)
  {
    for (unsigned int y = 0; y < 3; y++)
    for (unsigned int x = 0; x < 3; x++, index++)
      m_pTileIndexSet[i].indices[index] = (yOff+y) * arrayWidth + (xOff+x);
  }
}