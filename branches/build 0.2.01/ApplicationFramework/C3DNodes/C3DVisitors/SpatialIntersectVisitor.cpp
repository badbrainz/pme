#include "SpatialIntersectVisitor.h"
#include "../../C3DDatabase/TerrainDatabase.h"
#include "../../C3DGeometry/Ray3D.h"
#include "../SpatialIndexNode.h"
#include "../SpatialIndexCell.h"
#include "../NodeIterator.h"

Tuple4ub redColors[9];

SpatialIntersectVisitor::SpatialIntersectVisitor()
{
  for (int i = 0; i < 9; i++)
  redColors[i].set(0xff,0x00,0x00,0xff);
}

void SpatialIntersectVisitor::Visit(SpatialIndexBaseNode *base)
{
  if (Intersect(&base->GetBoundsDescriptor()))
  {
    NodeIterator iter(base->GetFirstChild());
	  while (!iter.End())
	  {
		  iter.Current()->Accept(this);
		  iter++;
	  }
  }
}

void SpatialIntersectVisitor::Visit(SpatialIndexNode *node)
{
  if (Intersect(&node->GetBoundsDescriptor()))
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
  BoundsDescriptor *bounds = &cell->GetBoundsDescriptor();
  if (Intersect(bounds))
  {
    /*Tuple4i range = cell->GetRange();
    int width  = range.z - range.x;
    int height = range.w - range.y;
    for (int i = 0 i <= range
    TileModelContorller *controller = m_pTerrainDatabase->GetController();*/
    glColor3f(1, 1, 1);
    bounds->render(BoundsDescriptor::AABB | BoundsDescriptor::WIRE);
  }
}

void SpatialIntersectVisitor::SetTerrain(TerrainDatabase *terrain)
{
  m_pTerrainDatabase = terrain;
}

bool SpatialIntersectVisitor::Intersect(BoundsDescriptor *bounds)
{
  Tuple3f ro = m_pRay->GetOrigin();
  Tuple3f rd = m_pRay->GetDestination();
  
  Tuple3f d  = (rd - ro) * 0.5f;
  Tuple3f a(fabsf(d.x), fabsf(d.y), fabsf(d.z));
  
  Tuple3f e  = bounds->getExtents();
  Tuple3f c  = ro + d - bounds->getCenterAABB();

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

void SpatialIntersectVisitor::SetRay(Ray3D *ray)
{
  m_pRay = ray;
}