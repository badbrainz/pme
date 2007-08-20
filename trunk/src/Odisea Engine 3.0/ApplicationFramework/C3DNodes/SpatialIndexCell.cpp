#include "SpatialIndexCell.h"
#include "C3DVisitors/TileGraphVisitor.h"

SpatialIndexCell::SpatialIndexCell()
{
  m_bRangeVisibility = true;
}

void SpatialIndexCell::Accept(TileGraphVisitor* visitor)
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