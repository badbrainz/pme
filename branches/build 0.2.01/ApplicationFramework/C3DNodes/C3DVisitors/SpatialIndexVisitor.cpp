#include "SpatialIndexVisitor.h"

#include "../SpatialIndexNode.h"
#include "../SpatialIndexCell.h"
#include "../NodeIterator.h"

#include <windows.h>
#include <gl/glu.h>
#include <gl/gl.h>

void SpatialIndexVisitor::Visit(SpatialIndexBaseNode *base)
{
  glColor3f(0, 0, 1);
  base->GetBoundsDescriptor().render(BoundsDescriptor::AABB | BoundsDescriptor::WIRE);
  
  NodeIterator iter(base->GetFirstChild());
  while (!iter.End())
  {
    iter.Current()->Accept(this);
    iter++;
  }
}

void SpatialIndexVisitor::Visit(SpatialIndexNode *node)
{
  //glColor3f(1, 0, 0);
  //node->GetBoundsDescriptor().render(BoundsDescriptor::AABB | BoundsDescriptor::WIRE);

  NodeIterator iter(node->GetFirstChild());
  while (!iter.End())
  {
    iter.Current()->Accept(this);
    iter++;
  }
}

void SpatialIndexVisitor::Visit(SpatialIndexCell *cell)
{
  //glColor3f(0, 1, 0);
  //cell->GetBoundsDescriptor().render(BoundsDescriptor::AABB | BoundsDescriptor::WIRE);
}