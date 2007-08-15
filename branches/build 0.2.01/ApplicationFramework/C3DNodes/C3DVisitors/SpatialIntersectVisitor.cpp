#include "SpatialIntersectVisitor.h"
#include "../../C3DGeometry/Ray3D.h"

void SpatialIntersectVisitor::SetRay(Ray3D *ray)
{
  m_pRay = ray;
}

void SpatialIntersectVisitor::Visit(SpatialIndexNode *node)
{
  BoundsDescriptor *bounds = cell->GetBoundsDescriptor();
  
  if (Intersect(bounds))
  {
    NodeIterator iter(node->GetFirstChild());
	  while (!iter.End())
	  {
		  iter.Current()->Accept(this);
		  iter++;
	  }
  }
}

void SpatialIntersectVisitor::Visit(SpatialIndexCell *cell)
{
  BoundsDescriptor *bounds = cell->GetBoundsDescriptor();
  
  if (Intersect(bounds))
  {
    //intersection...
  }
}

bool SpatialIntersectVisitor::Intersect(BoundsDescriptor *bounds)
{
  Tuple3f ro = ray->GetOrigin();
  Tuple3f rd = ray->GetDestination();
  
  Tuple3f e  = boundsdescriptor->getExtents();
  Tuple3f c  = ro + d - boundsdescriptor->getCenterAABB();
  
  Tuple3f d  = (rd - ro) * 128;
  Tuple3f a(fabsf(d.x), fabsf(d.y), fabsf(d.z));

  if (fabsf(c.x) > e.x + a.x)
    return false;
  if (fabsf(c.y) > e.y + a.y)
    return false;
  if (fabsf(c.z) > e.z + a.z)
    return false;

  if (fabsf(d.y * c.z - d.z * c.y) > (e.y * a.z) + (e.z * a.y))
    return false;
  if (fabsf(d.z * c.x - d.x * c.z) > (e.z * a.x) + (e.x * a.z))
    return false;
  if (fabsf(d.x * c.y - d.y * c.x) > (e.x * a.y) + (e.y * a.x))
    return false;

  return true;
}