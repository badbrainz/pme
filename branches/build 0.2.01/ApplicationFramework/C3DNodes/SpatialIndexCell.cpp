#include "SpatialIndexCell.h"
#include "C3DVisitors/SpatialIndexVisitor.h"

SpatialIndexCell::SpatialIndexCell()
{
  m_bRangeVisibility = true;
}

void SpatialIndexCell::Accept(SpatialIndexVisitor* visitor)
{
  visitor->Visit(this);
}

void SpatialIndexCell::RangeVisibility(bool value)
{
  m_bRangeVisibility = value;
}

bool SpatialIndexCell::RangeVisibility(void)
{
  return m_bRangeVisibility;
}

SpatialIndexCell::~SpatialIndexCell()
{
}