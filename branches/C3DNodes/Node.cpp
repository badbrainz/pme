#include "Node.h"
#include "C3DVisitors/TileGraphVisitor.h"
#include "C3DVisitors/SpatialIndexVisitor.h"

Node::Node()
{
	m_ParentLink.SetChildNode(this);
	m_ChildLink.SetParentNode(this);
}

Node::Node(const Node &copy)
{
  m_ParentLink = copy.m_ParentLink;
  m_ChildLink  = copy.m_ChildLink;
}

Node& Node::operator = (const Node &right)
{
  m_ParentLink = right.m_ParentLink;
  m_ChildLink  = right.m_ChildLink;
  return *this;
}

void Node::Accept(SpatialIndexVisitor* visitor)
{
  visitor->Visit(this);
}

void Node::Accept(TileGraphVisitor* visitor)
{
  visitor->Visit(this);
}

void Node::Attach(Node *node)
{
	NodeLink *pLink = &node->m_ChildLink;
	m_ParentLink.SetParentNode(node);
	m_ParentLink.SetPreviousLink(pLink->GetPreviousLink());
	m_ParentLink.SetNextLink(pLink);
	pLink->GetPreviousLink()->SetNextLink(&m_ParentLink);
	pLink->SetPreviousLink(&m_ParentLink);
}

void Node::Detach()
{
	m_ParentLink.SetParentNode(0);
	m_ParentLink.GetPreviousLink()->SetNextLink(m_ParentLink.GetNextLink());
	m_ParentLink.GetNextLink()->SetPreviousLink(m_ParentLink.GetPreviousLink());
	m_ParentLink.SetPreviousLink(&m_ParentLink);
	m_ParentLink.SetNextLink(&m_ParentLink);
}

void Node::Hide()
{
  m_ParentLink.GetPreviousLink()->SetNextLink(m_ParentLink.GetNextLink());
  m_ParentLink.GetNextLink()->SetPreviousLink(m_ParentLink.GetPreviousLink());
  m_ParentLink.SetPreviousLink(&m_ParentLink);
  m_ParentLink.SetNextLink(&m_ParentLink);
}

void Node::Reveal()
{
  if (m_ParentLink.GetPreviousLink() == &m_ParentLink && m_ParentLink.GetNextLink() == &m_ParentLink)
  {
    NodeLink *lastChild = m_ParentLink.GetParentNode()->m_ChildLink.GetPreviousLink();
    NodeLink *nullChild = &m_ParentLink.GetParentNode()->m_ChildLink;
    m_ParentLink.SetPreviousLink(lastChild);
    m_ParentLink.SetNextLink(nullChild);
    nullChild->SetPreviousLink(&m_ParentLink);
    lastChild->SetNextLink(&m_ParentLink);
  }
}

Node* Node::GetParentNode()
{
	return m_ParentLink.GetParentNode();
}

Node* Node::GetPreviousSibling()
{
	return m_ParentLink.GetPreviousLink()->GetChildNode();
}

Node* Node::GetNextSibling()
{
	return m_ParentLink.GetNextLink()->GetChildNode();
}

Node* Node::GetFirstChild()
{
	return m_ChildLink.GetNextLink()->GetChildNode();
}

Node* Node::GetLastChild()
{
	return m_ChildLink.GetPreviousLink()->GetChildNode();
}
