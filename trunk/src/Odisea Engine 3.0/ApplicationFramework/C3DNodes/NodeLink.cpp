#include "NodeLink.h"

NodeLink::NodeLink()
{
	m_pParentNode 	= 0;
	m_pChilNode			= 0;
	m_pPreviousLink = this;
	m_pNextLink 		= this;
}

void NodeLink::SetParentNode(Node *node)
{
	m_pParentNode = node;
}

void NodeLink::SetChildNode(Node *node)
{
	m_pChilNode = node;
}

Node *NodeLink::GetParentNode()
{
	return m_pParentNode;
}

Node *NodeLink::GetChildNode()
{
	return m_pChilNode;
}

void NodeLink::SetPreviousLink(NodeLink *link)
{
	m_pPreviousLink = link;
}

void NodeLink::SetNextLink(NodeLink *link)
{
	m_pNextLink = link;
}

NodeLink *NodeLink::GetPreviousLink()
{
	return m_pPreviousLink;
}

NodeLink *NodeLink::GetNextLink()
{
	return m_pNextLink;
}
