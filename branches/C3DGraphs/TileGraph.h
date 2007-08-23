#ifndef TILEGRAPH
#define TILEGRAPH

#include "../C3DNodes/C3DVisitors/TileGraphVisitor.h"
#include "../C3DControllers/TileModelController.h"
#include "../C3DNodes/TextureCoordsNode.h"
#include "../C3DNodes/TileModelNode.h"
#include "../C3DNodes/IndicesNode.h"
#include "../C3DNodes/TextureNode.h"
#include "../C3DNodes/Node.h"

#include "../C3DMemory/GenericPool.h"
#include "../OC/ocarray.h"

struct AttachmentPoint
{
  ~AttachmentPoint()
  {
    m_AggregateNodes.clearFast();
  }
  ArrayPtr <TextureCoordsNode> m_AggregateNodes;
};

class TileGraph
{
  public:
     TileGraph();
    ~TileGraph();
  
  public:
    void                  Trim();
    void                  Initialize(unsigned int levels);
    void                  Render(unsigned int level, TileGraphVisitor *visitor);
    unsigned int          Integrate(unsigned int level, Tuple2f *coords, unsigned int texID);
    TileModelNode*        ModelGraph(unsigned int level, unsigned int index);
    
    void Flush();

  private:
    Node*                 CheckForRepeat(Node* node, unsigned int identifier);
  
  private:
    Array    <AttachmentPoint> m_AttatchmentPoints;
    ArrayPtr <Node>            m_Levels;
    ArrayPtr <Node>            m_Nodes;
    
    /*GenericPool <Node,                 10,  10> m_NodePool;
    GenericPool <TextureNode,          10,  10> m_TextureNodePool;
    GenericPool <IndicesNode,          10,  10> m_IndicesNodePool;
    GenericPool <TileModelNode,     1e4*2,  10> m_TileModelNodePool;
    GenericPool <TextureCoordsNode,   150, 150> m_TextureCoordsNodePool;*/
    
    /*ArrayPtr <Node>              m_AquiredNodes;
    ArrayPtr <TextureNode>       m_AquiredTextureNodes;
    ArrayPtr <IndicesNode>       m_AquiredIndicesNodes;
    ArrayPtr <TileModelNode>     m_AquiredTileModelNodes;
    ArrayPtr <TextureCoordsNode> m_AquiredTextureCoordsNodes;*/
};

/**
 * In order for Array to contain elements of type T, T
 * needs to support:
 *   (1) a copy constructor
 *   (2) an operator =
 *   (3) an operator ==
 *   (4) a default constructor.
 */


#endif