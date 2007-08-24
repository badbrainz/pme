#include "NodeIterator.h"
#include "Node.h"

NodeIterator::NodeIterator() : m_pNode(0)
{
}

NodeIterator::NodeIterator(Node* node)
{
	m_pNode = node;
}

NodeIterator& NodeIterator::operator ++ ()
{
	m_pNode = m_pNode->GetNextSibling();
	return *this;
}

NodeIterator NodeIterator::operator ++ (int)
{
	NodeIterator temp = *this;
	m_pNode = m_pNode->GetNextSibling();
	return temp;
}

NodeIterator& NodeIterator::operator -- ()
{
	m_pNode = m_pNode->GetPreviousSibling();
	return *this;
}

NodeIterator NodeIterator::operator -- (int)
{
	NodeIterator temp = *this;
	m_pNode = m_pNode->GetPreviousSibling();
	return temp;
}

bool NodeIterator::operator == (NodeIterator& right)
{
	return (m_pNode == right.m_pNode);
}

bool NodeIterator::operator != (NodeIterator& right)
{
	return !this->operator == (right);
}

bool NodeIterator::End()
{
	return (m_pNode == 0);
}

Node* NodeIterator::Current()
{
	return m_pNode;
}
