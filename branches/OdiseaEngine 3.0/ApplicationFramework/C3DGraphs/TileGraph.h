#ifndef TILEGRAPH
#define TILEGRAPH

#include "../C3DNodes/C3DVisitors/TileGraphVisitor.h"
#include "../C3DControllers/TileModelController.h"
#include "../C3DManagers/ResourceManager.h"
#include "../C3DNodes/TextureCoordsNode.h"
#include "../C3DNodes/TileModelNode.h"
#include "../C3DNodes/IndicesNode.h"
#include "../C3DNodes/TextureNode.h"
#include "../C3DNodes/GraphNode.h"

#include "../OC/ocarray.h"

struct AttachmentPoint
{
  ArrayPtr <TextureCoordsNode> m_AggregateNodes;
};

class TileGraph
{
  public:
     TileGraph();
    ~TileGraph();
  
  public:
    void                  Initialize(unsigned int levels);
    void                  Render(unsigned int level, TileGraphVisitor *visitor);
    unsigned int          Integrate(unsigned int level, Tuple2f *coords, unsigned int texID);
    TileModelNode*  ModelGraph(unsigned int level, unsigned int index);

  private:
    GraphNode*            CheckForRepeat(GraphNode* node, unsigned int identifier);
  
  private:
    ResourceManager <TileModelController> m_ManagedTileModelControllers;
    ResourceManager <TextureCoordsNode>   m_ManagedTextureCoordsNodes;
    ResourceManager <TileModelNode>       m_ManagedTileModelNodes;
    ResourceManager <IndicesNode>         m_ManagedIndicesNodes;
    ResourceManager <TextureNode>         m_ManagedTextureNodes;
    ResourceManager <GraphNode>           m_ManagedGraphNodes;
    
    Array    <AttachmentPoint>            m_AttatchmentPoints;
    ArrayPtr <GraphNode>                  m_Levels;
};

#endif