#include "IndicesNode.h"
#include "C3DVisitors/TileGraphVisitor.h"
#include <iostream>

IndicesNode::IndicesNode()
{
}

IndicesNode::IndicesNode(const IndicesNode &copy)
{
  m_Indices = copy.m_Indices;
}

IndicesNode& IndicesNode::operator = (const IndicesNode &right)
{
  m_Indices = right.m_Indices;
  return *this;
}

bool IndicesNode::operator == (const IndicesNode &right)
{
  return (m_Indices == right.m_Indices);
}

void IndicesNode::Accept(TileGraphVisitor* visitor)
{
  visitor->Visit(this);
}

void IndicesNode::SetIndices(unsigned int *indices)
{
  m_Indices = indices;
}

unsigned int *IndicesNode::GetIndices()
{
  return m_Indices;
}

IndicesNode::~IndicesNode()
{
}