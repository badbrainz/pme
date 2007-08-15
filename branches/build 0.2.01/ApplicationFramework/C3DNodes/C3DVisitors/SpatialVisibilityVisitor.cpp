#include "SpatialVisibilityVisitor.h"

#include "../SpatialIndexCell.h"
#include "../NodeIterator.h"

#include "Viewer/Frustum.h"
#include "../../C3DDatabase/TerrainDatabase.h"

void SpatialVisibilityVisitor::SetFrustum(Frustum *frustumpointer)
{
  m_pFrustum = frustumpointer;
}

void SpatialVisibilityVisitor::SetTerrain(TerrainDatabase *terrain)
{
  m_pTerrainDatabase = terrain;
}

void SpatialVisibilityVisitor::Visit(SpatialIndexNode *base)
{
  if(m_pFrustum->AABBInFrustum(base->GetBoundsDescriptor()))
  {
    NodeIterator iter(base->GetFirstChild());
	  while (!iter.End())
	  {
		  iter.Current()->Accept(this);
		  iter++;
	  }
  }
}

void SpatialVisibilityVisitor::Visit(SpatialIndexCell *cell)
{
  if(m_pFrustum->AABBInFrustum(cell->GetBoundsDescriptor()))
    m_pTerrainDatabase->ControllerVisibility(cell->GetRange(), true);
  else
    m_pTerrainDatabase->ControllerVisibility(cell->GetRange(), false);
}