#include "SpatialVisibilityVisitor.h"

#include "../SpatialIndexNode.h"
#include "../SpatialIndexCell.h"
#include "../NodeIterator.h"

#include "Viewer/Frustum.h"
#include "../../C3DDatabase/TerrainDatabase.h"

void SpatialVisibilityVisitor::Visit(SpatialIndexBaseNode *base)
{
  if (m_pFrustum->AABBInFrustum(base->GetBoundsDescriptor()))
  {
    NodeIterator iter(base->GetFirstChild());
	  while (!iter.End())
	  {
		  iter.Current()->Accept(this);
		  iter++;
	  }
  }
  else
  {
    m_pTerrainDatabase->CellVisibility(base->GetRange(), false);
  }
}

void SpatialVisibilityVisitor::Visit(SpatialIndexNode *node)
{
  if (m_pFrustum->AABBInFrustum(node->GetBoundsDescriptor()))
  {
    NodeIterator iter(node->GetFirstChild());
	  while (!iter.End())
	  {
		  iter.Current()->Accept(this);
		  iter++;
	  }
  }
  else
  {
    m_pTerrainDatabase->CellVisibility(node->GetRange(), false);
  }
}

void SpatialVisibilityVisitor::Visit(SpatialIndexCell *cell)
{
  if (m_pFrustum->AABBInFrustum(cell->GetBoundsDescriptor()))
  {
    m_pTerrainDatabase->ControllerVisibility(cell->GetRange(), true);
    cell->RangeVisibility(true);
  }
  else
  {
    m_pTerrainDatabase->ControllerVisibility(cell->GetRange(), false);
    cell->RangeVisibility(false);
  }
}

void SpatialVisibilityVisitor::SetFrustum(Frustum *frustumpointer)
{
  m_pFrustum = frustumpointer;
}

void SpatialVisibilityVisitor::SetTerrain(TerrainDatabase *terrain)
{
  m_pTerrainDatabase = terrain;
}