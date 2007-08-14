#include "SpatialIndexNode.h"
#include "C3DVisitors/TileGraphVisitor.h"

SpatialIndexNode::SpatialIndexNode()
{
  m_Level = 0;
}

void SpatialIndexNode::Accept(TileGraphVisitor* visitor)
{
  visitor->Visit(this);
}

void SpatialIndexNode::SetBoundsDescriptor(const BoundsDescriptor &descriptor)
{
  m_BoundsDescriptor = descriptor;
}

void SpatialIndexNode::UpdateBounds(BoundsDescriptor &descriptor)
{
  m_BoundsDescriptor = descriptor;
}

void SpatialIndexNode::SetRange(const Tuple4i &range)
{
  m_Range = range;
}

void SpatialIndexNode::SetLevel(int level)
{
  m_Level = level;
}

int SpatialIndexNode::GetLevel()
{
  return m_Level;
}

BoundsDescriptor &SpatialIndexNode::GetBoundsDescriptor(void)
{
  return m_BoundsDescriptor;
}

Tuple4i &SpatialIndexNode::GetRange()
{
  return m_Range;
}

SpatialIndexNode::~SpatialIndexNode()
{
}