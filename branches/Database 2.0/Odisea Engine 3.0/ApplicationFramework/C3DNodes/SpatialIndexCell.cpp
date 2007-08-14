#include "SpatialIndexCell.h"
#include "C3DVisitors/TileGraphVisitor.h"

void SpatialIndexCell::Accept(TileGraphVisitor* visitor)
{
  visitor->Visit(this);
}