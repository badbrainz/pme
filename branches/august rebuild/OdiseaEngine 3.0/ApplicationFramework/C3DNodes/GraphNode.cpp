#include "GraphNode.h"
#include "C3DVisitors/TileGraphVisitor.h"

GraphNode::GraphNode()
{
	m_ParentLink.SetChildNode(this);
	m_ChildLink.SetParentNode(this);
}

void GraphNode::Accept(TileGraphVisitor* visitor)
{
  visitor->Visit(this);
}

void GraphNode::Attach(GraphNode *node)
{
	GraphNodeLink *pLink = &node->m_ChildLink;
	m_ParentLink.SetParentNode(node);
	m_ParentLink.SetPreviousLink(pLink->GetPreviousLink());
	m_ParentLink.SetNextLink(pLink);
	pLink->GetPreviousLink()->SetNextLink(&m_ParentLink);
	pLink->SetPreviousLink(&m_ParentLink);
}

void GraphNode::Detach()
{
	m_ParentLink.SetParentNode(0);
	m_ParentLink.GetPreviousLink()->SetNextLink(m_ParentLink.GetNextLink());
	m_ParentLink.GetNextLink()->SetPreviousLink(m_ParentLink.GetPreviousLink());
	m_ParentLink.SetPreviousLink(&m_ParentLink);
	m_ParentLink.SetNextLink(&m_ParentLink);
}

void GraphNode::Hide()
{
  m_ParentLink.GetPreviousLink()->SetNextLink(m_ParentLink.GetNextLink());
  m_ParentLink.GetNextLink()->SetPreviousLink(m_ParentLink.GetPreviousLink());
  m_ParentLink.SetPreviousLink(&m_ParentLink);
  m_ParentLink.SetNextLink(&m_ParentLink);
}

void GraphNode::Reveal()
{
  if (m_ParentLink.GetPreviousLink() == &m_ParentLink && m_ParentLink.GetNextLink() == &m_ParentLink)
  {
    GraphNodeLink *lastChild = m_ParentLink.GetParentNode()->m_ChildLink.GetPreviousLink();
    GraphNodeLink *nullChild = &m_ParentLink.GetParentNode()->m_ChildLink;
    m_ParentLink.SetPreviousLink(lastChild);
    m_ParentLink.SetNextLink(nullChild);
    nullChild->SetPreviousLink(&m_ParentLink);
    lastChild->SetNextLink(&m_ParentLink);
  }
}

GraphNode* GraphNode::GetParentNode()
{
	return m_ParentLink.GetParentNode();
}

GraphNode* GraphNode::GetPreviousSibling()
{
	return m_ParentLink.GetPreviousLink()->GetChildNode();
}

GraphNode* GraphNode::GetNextSibling()
{
	return m_ParentLink.GetNextLink()->GetChildNode();
}

GraphNode* GraphNode::GetFirstChild()
{
	return m_ChildLink.GetNextLink()->GetChildNode();
}

GraphNode* GraphNode::GetLastChild()
{
	return m_ChildLink.GetPreviousLink()->GetChildNode();
}
