#ifndef TERRAINDATABASE
#define TERRAINDATABASE

#include "../C3DGraphs/TileGraph.h"
#include "../C3DParsers/PveObject.h"
#include "../C3DParsers/PteObject.h"
#include "../C3DNodes/SpatialIndexNode.h"
#include "../C3DNodes/SpatialIndexCell.h"
#include "../C3DControllers/TileModelController.h"

#include "../C3DScenes/NewMapScene.h"

class GameFileDescriptor
{
  public:
    String pvePath;
    String ptePath;
};

class TerrainDatabase
{
  public:
     TerrainDatabase(){}
    ~TerrainDatabase();
    
    void LoadGameData(const GameFileDescriptor &descriptor);
    void LoadGameData(const MapDescriptor &descriptor);
    
    void Draw(unsigned int level, TileGraphVisitor *visitor);
    void Cull(SpatialIndexVisitor *visitor);//name change
    
    void CellVisibility(const Tuple4i &range, bool value);
    void ControllerVisibility(const Tuple4i &range, bool value);
    
    unsigned int GetTextureCount();
    unsigned int GetTextureID(unsigned int index);
  
  private:
    void SetupSpatialIndexStructure(void);
    void SetupCellDataStructures(void);
    void SetupGraphStructure(void);
    
    void ComputeCellBoundaries(void);
    void ComputeTreeBoundaries(void);
    
    void UnloadPreviousMap();
  
  private:
    PveObject               m_PveObject;
    PteObject               m_PteObject;
    TileGraph               m_TileGraph;
    SpatialIndexBaseNode   *m_pTrunk;
    
    ArrayPtr <SpatialIndexBaseNode> m_SpatialIndexBranches;
    ArrayPtr <TileModelController>  m_Controllers;
    ArrayPtr <SpatialIndexCell>     m_SpatialIndexCells;
};

#endif