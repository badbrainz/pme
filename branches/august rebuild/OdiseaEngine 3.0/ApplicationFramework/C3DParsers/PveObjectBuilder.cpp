#include "PveObject.h"

const float tilesize = 0.5f;

void PveObject::InitializeMemoryBlock(void)
{
  ///TileVertexBlock method seems to be faster
  unsigned int bc    = (tilesPerX*2+1)*(tilesPerY*2+1);
  unsigned int tiles = tilesPerX*tilesPerY;
  unsigned int chunk = bc;
  unsigned int bytes = round((float)tiles*sizeof(Tile)           /chunk)*chunk
                      +round((float)bc   *sizeof(unsigned char)  /chunk)*chunk
                      +round((float)bc   *sizeof(Tuple4ub)       /chunk)*chunk
                      +round((float)bc   *sizeof(Tuple3f)        /chunk)*chunk
                      +round((float)tiles*sizeof(TileIndexSet)   /chunk)*chunk
                      +round((float)tiles*sizeof(TileVertexBlock)/chunk)*chunk;
                      //+round((float)tiles*sizeof(TileVertexSet)  /chunk)*chunk
                      //+round((float)tiles*sizeof(TileColorSet)   /chunk)*chunk;

  memoryBlock.SetChunkSize(chunk);
  memoryBlock.SetMinAllocSize(chunk*2);
  memoryBlock.AllocateMemory(bytes);
}

void PveObject::BuildTerrainVertexField(void)
{
  unsigned int width  = tilesPerX*2+1;
  unsigned int height = tilesPerY*2+1;
  unsigned int index	= 0;

  float xPos = 0;
  float yPos = 0;

  m_pUncompressedVerts = (Tuple3f*) memoryBlock.GetRange(width*height*sizeof(Tuple3f));

  for (unsigned int row = 0; row < height; row++, xPos = 0, yPos += tilesize)
  for (unsigned int col = 0; col < width; col++)
  {
    index               = row * width + col;
    m_pUncompressedVerts[index].x = xPos;
    m_pUncompressedVerts[index].y = heightRatio * m_pCompressedYBuffer[index] + minHeight;
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
  unsigned int size = tilesPerX*tilesPerY*sizeof(TileVertexBlock);
  tileVertexBlock = (TileVertexBlock*) memoryBlock.GetRange(size);
  const Tuple3f  *vchunk = 0;
  const Tuple4ub *cchunk = 0;

  for (unsigned int i = 0; i < tilesPerX*tilesPerY; i++)
  {
    vchunk = GetVertexBufferChunk(i);
    cchunk = GetColorBufferChunk(i);
    for (unsigned int j = 0; j < 9; j++)
    {
      tileVertexBlock[i].vertices[j] = vchunk[j];
      tileVertexBlock[i].colors[j]   = cchunk[j];
    }
  }
}

void PveObject::SetupTileIndices(void)
{
  unsigned int arrayWidth	= (tilesPerX*2)+1;
  unsigned int width  		= tilesPerX,
               height 		= tilesPerY;
  unsigned int xOff = 0, yOff = 0, index = 0, i = 0;

  tileIndexSet = (TileIndexSet*) memoryBlock.GetRange(width*height*sizeof(TileIndexSet));

  for (unsigned int a = 0; a < tilesPerY; a++, yOff += 2, xOff = 0)
  for (unsigned int b = 0; b < tilesPerX; b++, xOff += 2, index = 0, i++)
  {
    for (unsigned int y = 0; y < 3; y++)
    for (unsigned int x = 0; x < 3; x++, index++)
      tileIndexSet[i].indices[index] = (yOff+y) * arrayWidth + (xOff+x);
  }
}