#include "SpatialIndexBaseNode.h"
#include "C3DVisitors/TileGraphVisitor.h"

SpatialIndexBaseNode::SpatialIndexBaseNode()
{
  m_Level = 0;
}

void SpatialIndexBaseNode::Accept(TileGraphVisitor* visitor)
{
  visitor->Visit(this);
}

void SpatialIndexBaseNode::SetBoundsDescriptor(const BoundsDescriptor &descriptor)
{
  m_BoundsDescriptor = descriptor;
}

void SpatialIndexBaseNode::UpdateBounds(BoundsDescriptor &descriptor)
{
  m_BoundsDescriptor = descriptor;
}

void SpatialIndexBaseNode::SetRange(const Tuple4i &range)
{
  m_Range = range;
}

void SpatialIndexBaseNode::SetLevel(int level)
{
  m_Level = level;
}

int SpatialIndexBaseNode::GetLevel()
{
  return m_Level;
}

BoundsDescriptor &SpatialIndexBaseNode::GetBoundsDescriptor(void)
{
  return m_BoundsDescriptor;
}

Tuple4i &SpatialIndexBaseNode::GetRange()
{
  return m_Range;
}

SpatialIndexBaseNode::~SpatialIndexBaseNode()
{
}