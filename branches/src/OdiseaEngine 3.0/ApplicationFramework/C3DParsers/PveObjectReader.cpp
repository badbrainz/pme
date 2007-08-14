#include "PveObject.h"
#include "Managers/MediaPathManager.h"

bool PveObject::LoadFromFile(const String &filePath)
{
  inputFile.open(filePath, std::ofstream::in|std::ofstream::binary);
  ReadHeader();InitializeMemoryBlock();
  //SkipTileArray();
  ReadTileChunk();
  ReadVertexChunk();
  ReadColorChunk();
  inputFile.close();

  ComputeHeightRatio();
  BuildTerrainVertexField();
  SetupTileIndices();

  //BuildTileVertexChunks();
  //BuildTileColorChunks();
  BuildTileVertexBlock();
  
  //ComputeTileMaxHeights();
  //ComputeTileMinHeights();
  
  return true;
}

void PveObject::ReadHeader(void)
{
  inputFile.seekg(4, std::ios::cur);
  inputFile.read((char*)&tilesPerX, 4);
  inputFile.read((char*)&tilesPerY, 4);
  inputFile.read((char*)&maxHeight, 4);
  inputFile.read((char*)&minHeight, 4);
}

void PveObject::ReadVertexChunk(void)
{
  unsigned int size = (tilesPerX*2+1)*(tilesPerY*2+1)*sizeof(unsigned char);
  m_pCompressedYBuffer = (unsigned char*) memoryBlock.GetRange(size);
  inputFile.read((char*)m_pCompressedYBuffer, size);
}

void PveObject::ReadColorChunk(void)
{
  unsigned int size = (tilesPerX*2+1)*(tilesPerY*2+1)*sizeof(Tuple4ub);
  m_pColorBuffer = (Tuple4ub*) memoryBlock.GetRange(size);
  inputFile.read((char*)m_pColorBuffer, size);
}

void PveObject::ReadTileChunk(void)
{
  short           textureID[2];
  unsigned short  flags;
  unsigned char   maxTileHeight,
                  extra;

  tileSet = (Tile*) memoryBlock.GetRange(tilesPerX*tilesPerY*sizeof(Tile));

  for(unsigned int a = 0; a < tilesPerX*tilesPerY; a++)
  {
    inputFile.read((char*)&textureID, 4);
    inputFile.read((char*)&flags, 2);
    inputFile.read((char*)&maxTileHeight, 1);
    inputFile.read((char*)&extra, 1);

    tileSet[a].SetPrimaryTextureID(textureID[0]);
    tileSet[a].SetSecondaryTextureID(textureID[1]);
    tileSet[a].SetFlags(flags);
    tileSet[a].SetMaxHeight(maxTileHeight);
    tileSet[a].SetExtra(extra);
  }
}

void PveObject::SkipTileArray(void)
{
  inputFile.seekg(sizeof(Tile)*tilesPerX*tilesPerY, std::ios::cur);
}