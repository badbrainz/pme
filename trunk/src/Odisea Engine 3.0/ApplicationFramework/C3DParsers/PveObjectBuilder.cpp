#include "PveObject.h"

const float tilesize = 0.5f;

void PveObject::InitializeMemoryBlock(void)
{
  ///TileVertexBlock method seems to be faster
  unsigned int bc    = (m_uiTilesPerX*2+1)*(m_uiTilesPerY*2+1);
  unsigned int tiles = m_uiTilesPerX*m_uiTilesPerY;
  unsigned int chunk = bc;
  unsigned int bytes = round((float)tiles*sizeof(Tile)           /chunk)*chunk
                      +round((float)bc   *sizeof(unsigned char)  /chunk)*chunk
                      +round((float)bc   *sizeof(Tuple4ub)       /chunk)*chunk
                      +round((float)bc   *sizeof(Tuple3f)        /chunk)*chunk
                      +round((float)tiles*sizeof(TileIndexSet)   /chunk)*chunk
                      +round((float)tiles*sizeof(TileVertexBlock)/chunk)*chunk;
                      //+round((float)tiles*sizeof(TileVertexSet)  /chunk)*chunk
                      //+round((float)tiles*sizeof(TileColorSet)   /chunk)*chunk;

  m_MemoryBlock.SetChunkSize(chunk);
  m_MemoryBlock.SetMinAllocSize(chunk*2);
  m_MemoryBlock.AllocateMemory(bytes);
}

void PveObject::BuildTerrainVertexField(void)
{
  unsigned int width  = m_uiTilesPerX*2+1;
  unsigned int height = m_uiTilesPerY*2+1;
  unsigned int index	= 0;

  float xPos = 0;
  float yPos = 0;

  m_pUncompressedVerts = (Tuple3f*) m_MemoryBlock.GetRange(width*height*sizeof(Tuple3f));

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

/*void PveObject::BuildTileVertexChunks(void)
{
  unsigned int size = tilesPerX*tilesPerY*sizeof(TileVertexSet);
  tileVertexSet = (TileVertexSet*) memoryBlock.GetRange(size);
  const Tuple3f *chunk = 0;
  for (unsigned int i = 0; i < tilesPerX*tilesPerY; i++)
  {
    chunk = GetVertexBufferChunk(i);
    for (unsigned int j = 0; j < 9; j++)
      tileVertexSet[i].vertices[j] = chunk[j];
  }
}

void PveObject::BuildTileColorChunks(void)
{
  unsigned int size = tilesPerX*tilesPerY*sizeof(TileColorSet);
  tileColorSet = (TileColorSet*) memoryBlock.GetRange(size);
  const Tuple4ub *chunk = 0;
  for (unsigned int i = 0; i < tilesPerX*tilesPerY; i++)
  {
    chunk = GetColorBufferChunk(i);
    for (unsigned int j = 0; j < 9; j++)
      tileColorSet[i].colors[j] = chunk[j];
  }
}*/

void PveObject::BuildTileVertexBlock(void)
{
  unsigned int size = m_uiTilesPerX*m_uiTilesPerY*sizeof(TileVertexBlock);
  m_pTileVertexBlock = (TileVertexBlock*) m_MemoryBlock.GetRange(size);
  const Tuple3f  *vchunk = 0;
  const Tuple4ub *cchunk = 0;

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

  m_pTileIndexSet = (TileIndexSet*) m_MemoryBlock.GetRange(width*height*sizeof(TileIndexSet));

  for (unsigned int a = 0; a < m_uiTilesPerY; a++, yOff += 2, xOff = 0)
  for (unsigned int b = 0; b < m_uiTilesPerX; b++, xOff += 2, index = 0, i++)
  {
    for (unsigned int y = 0; y < 3; y++)
    for (unsigned int x = 0; x < 3; x++, index++)
      m_pTileIndexSet[i].indices[index] = (yOff+y) * arrayWidth + (xOff+x);
  }
}