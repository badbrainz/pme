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
};


#endif