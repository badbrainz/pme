#include "PveObject.h"
#include "Managers/MediaPathManager.h"

bool PveObject::LoadFromFile(const String &filePath)
{
  m_InputFile.open(filePath, std::ofstream::in|std::ofstream::binary);
  ReadHeader();InitializeMemoryBlock();
  //SkipTileArray();
  ReadTileChunk();
  ReadVertexChunk();
  ReadColorChunk();
  m_InputFile.close();

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
  m_InputFile.seekg(4, std::ios::cur);
  m_InputFile.read((char*)&m_uiTilesPerX, 4);
  m_InputFile.read((char*)&m_uiTilesPerY, 4);
  m_InputFile.read((char*)&m_fMaxHeight,  4);
  m_InputFile.read((char*)&m_fMinHeight,  4);
}

void PveObject::ReadVertexChunk(void)
{
  unsigned int size = (m_uiTilesPerX*2+1)*(m_uiTilesPerY*2+1)*sizeof(unsigned char);
  m_pCompressedYBuffer = (unsigned char*) m_MemoryBlock.GetRange(size);
  m_InputFile.read((char*)m_pCompressedYBuffer, size);
}

void PveObject::ReadColorChunk(void)
{
  unsigned int size = (m_uiTilesPerX*2+1)*(m_uiTilesPerY*2+1)*sizeof(Tuple4ub);
  m_pColorBuffer = (Tuple4ub*) m_MemoryBlock.GetRange(size);
  m_InputFile.read((char*)m_pColorBuffer, size);
}

void PveObject::ReadTileChunk(void)
{
  short           textureID[2];
  unsigned short  flags;
  unsigned char   maxTileHeight,
                  extra;

  m_pTileSet = (Tile*) m_MemoryBlock.GetRange(m_uiTilesPerX*m_uiTilesPerY*sizeof(Tile));

  for(unsigned int a = 0; a < m_uiTilesPerX*m_uiTilesPerY; a++)
  {
    m_InputFile.read((char*)&textureID, 4);
    m_InputFile.read((char*)&flags, 2);
    m_InputFile.read((char*)&maxTileHeight, 1);
    m_InputFile.read((char*)&extra, 1);

    m_pTileSet[a].SetPrimaryTextureID(textureID[0]);
    m_pTileSet[a].SetSecondaryTextureID(textureID[1]);
    m_pTileSet[a].SetFlags(flags);
    m_pTileSet[a].SetMaxHeight(maxTileHeight);
    m_pTileSet[a].SetExtra(extra);
  }
}

void PveObject::SkipTileArray(void)
{
  m_InputFile.seekg(sizeof(Tile)*m_uiTilesPerX*m_uiTilesPerY, std::ios::cur);
}