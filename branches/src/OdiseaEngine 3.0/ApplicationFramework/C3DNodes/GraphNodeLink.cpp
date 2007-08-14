#include "GraphNodeLink.h"

GraphNodeLink::GraphNodeLink()
{
	m_pParentNode 	= 0;
	m_pChilNode			= 0;
	m_pPreviousLink = this;
	m_pNextLink 		= this;
}

void GraphNodeLink::SetParentNode(GraphNode *node)
{
	m_pParentNode = node;
}

void GraphNodeLink::SetChildNode(GraphNode *node)
{
	m_pChilNode = node;
}

GraphNode *GraphNodeLink::GetParentNode()
{
	return m_pParentNode;
}

GraphNode *GraphNodeLink::GetChildNode()
{
	return m_pChilNode;
}

void GraphNodeLink::SetPreviousLink(GraphNodeLink *link)
{
	m_pPreviousLink = link;
}

void GraphNodeLink::SetNextLink(GraphNodeLink *link)
{
	m_pNextLink = link;
}

GraphNodeLink *GraphNodeLink::GetPreviousLink()
{
	return m_pPreviousLink;
}

GraphNodeLink *GraphNodeLink::GetNextLink()
{
	return m_pNextLink;
}
