#ifndef PVEOBJECT
#define PVEOBJECT

#include "../C3DMemory/VirtualMemory.h"
#include "../C3DTerrain/Tile.h"
#include "Tools/Logger.h"
#include <fstream>

struct TileIndexSet
{
  unsigned int indices[9];
};

struct TileVertexSet
{
  Tuple3f vertices[9];
};

struct TileColorSet
{
  Tuple4ub colors[9];
};

struct TileVertexBlock
{
  Tuple3f   vertices[9];
  Tuple4ub  colors[9];
};

class PveObject
{
  public:
     PveObject();
    ~PveObject();

  public:
    bool                LoadFromFile(const String &filePath);
    
    Tile               *GetTile(int tileIndex);
    unsigned int        GetTilesPerX(void);
    unsigned int        GetTilesPerY(void);
    float               GetMaxHeight(void);
    float               GetMinHeight(void);
    float               GetHeightRatio(void);
    
    const unsigned int *GetTileVertexIndices(int tileIndex);
    const Tuple3f      *GetVertexBufferChunk(int tileIndex);
    Tuple3f            *GetTileVertexChunk(int tileIndex);
    Tuple4ub           *GetTileColorChunk(int tileIndex);
    const Tuple4ub     *GetColorBufferChunk(int tileIndex);
    const Tuple3f      *GetVertexBuffer(void);
    const Tuple4ub     *GetColorBuffer(void);
    
    //void                SetColorBufferChunk(int tileIndex, const Tuple4ub *colors);
    void                SetElevation(float elevation);
    
    float               ComputeMaxTileHeight(unsigned int index);
    float               ComputeMinTileHeight(unsigned int index);
  
  private:
    void                ReadHeader(void);
    void                ReadVertexChunk(void);
    void                ReadColorChunk(void);
    void                ReadTileChunk(void);
    
    void                SkipTileArray(void);
    void                BuildTerrainVertexField(void);
    void                BuildTileVertexBlock(void);
    void                InitializeMemoryBlock(void);
    void                SetupTileIndices(void);
    
    void                ComputeHeightRatio(void);
    
    //void                BuildTileColorChunks(void);
    //void                BuildTileVertexChunks(void);
    //void                ComputeTileMinHeights(void);
    //void                ComputeTileMaxHeights(void);
  
  private:

/**
 *block memory
 */
    VirtualMemory       m_MemoryBlock;
    Tuple3f            *m_pUncompressedVerts;
    unsigned char      *m_pCompressedYBuffer;
    TileVertexBlock    *m_pTileVertexBlock;
    TileIndexSet       *m_pTileIndexSet;
    Tuple4ub           *m_pColorBuffer;
    Tile               *m_pTileSet;
/** 
 */
    
    Tuple3f             m_TempVertexBuffer[9];
    Tuple4ub            m_TempColorBuffer[9];
    
    unsigned int        m_uiTilesPerX,
                        m_uiTilesPerY;
    
    float               m_fHeightRatio,
                        m_fMaxHeight,
                        m_fMinHeight,
                        m_fElevate;
    
    std::ifstream       m_InputFile;
};

#endif