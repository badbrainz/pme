#ifndef NODE
#define NODE

#include "NodeLink.h"
#include "../C3DManagers/ResourceManager.h"

class SpatialIndexVisitor;
class TileGraphVisitor;

class Node : public ManagedResource
{
  public:
    Node();
    virtual ~Node(){}
    
    Node(const Node &copy);
    Node& operator = (const Node &right);
    
    virtual void Accept(SpatialIndexVisitor* visitor);
    virtual void Accept(TileGraphVisitor* visitor);
    
    void Destroy(){}
  
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