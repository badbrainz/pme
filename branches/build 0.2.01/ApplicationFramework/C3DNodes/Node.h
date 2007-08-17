#ifndef NODE
#define NODE

#include "NodeLink.h"

class SpatialIndexVisitor;
class TileGraphVisitor;

class Node
{
  public:
    Node();
    virtual ~Node(){}
    
    virtual void Accept(SpatialIndexVisitor* visitor);
    virtual void Accept(TileGraphVisitor* visitor);
  
  public:
    void	        Detach();
    void					Attach(Node *node);
    
    void          Hide();
    void          Reveal();
    
    Node*         GetParentNode();
    Node*         GetPreviousSibling();
    Node*         GetNextSibling();
    Node*         GetFirstChild();
    Node*         GetLastChild();
  
  protected:
    NodeLink m_ParentLink;
    NodeLink m_ChildLink;
};

#endif