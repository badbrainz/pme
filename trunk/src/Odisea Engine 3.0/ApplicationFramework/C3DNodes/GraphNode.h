#ifndef GRAPHNODE
#define GRAPHNODE

#include "GraphNodeLink.h"

class TileGraphVisitor;

class GraphNode
{
  public:
    GraphNode();
    virtual ~GraphNode(){}
    
    virtual void Accept(TileGraphVisitor* visitor);
  
  public:
    void	        Detach();
    void					Attach(GraphNode *node);
    
    void          Hide();
    void          Reveal();
    
    GraphNode*    GetParentNode();
    GraphNode*    GetPreviousSibling();
    GraphNode*    GetNextSibling();
    GraphNode*    GetFirstChild();
    GraphNode*    GetLastChild();
  
  protected:
    GraphNodeLink m_ParentLink;
    GraphNodeLink m_ChildLink;
};

#endif