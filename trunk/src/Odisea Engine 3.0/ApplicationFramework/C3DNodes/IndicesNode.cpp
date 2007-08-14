#include "IndicesNode.h"
#include "C3DVisitors/TileGraphVisitor.h"
#include <iostream>

IndicesNode::IndicesNode()
{
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