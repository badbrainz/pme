#ifndef TERRAINDATABASE
#define TERRAINDATABASE

#include "../C3DGraphs/TileGraph.h"
#include "../C3DParsers/PveObject.h"
#include "../C3DParsers/PteObject.h"
#include "../C3DNodes/SpatialIndexCell.h"
#include "../C3DControllers/TileModelController.h"

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
    void ControllerVisibility(const Tuple4i &range, bool visible);
    void Draw(unsigned int level, TileGraphVisitor *visitor);
    void Cull(TileGraphVisitor *visitor);//name change: Accept ?
  
  private:
    void SetupSpatialIndexStructure(void);
    void SetupCellDataStructures(void);
    void SetupGraphStructure(void);
    
    void ComputeCellBoundaries(void);
    void ComputeTreeBoundaries(void);
  
  private:
    PveObject m_PveObject;
    PteObject m_PteObject;
    
    TileGraph m_TileGraph;
    
    ResourceManager <TileModelController> m_ManagedTileModelControllers;
    ResourceManager <SpatialIndexNode>    m_ManagedBranches;
    ResourceManager <SpatialIndexCell>    m_ManagedCells;
    
    ArrayPtr <TileModelController>  m_Controllers;
    ArrayPtr <SpatialIndexNode>     m_SpatialIndexCells;//managed
    ArrayPtr <SpatialIndexNode>     m_SpatialIndexBranches;//managed
    
    SpatialIndexNode *m_pTrunk;
};

#endif