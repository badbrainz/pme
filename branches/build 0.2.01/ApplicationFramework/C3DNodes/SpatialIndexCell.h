#ifndef SPATIALINDEXCELL
#define SPATIALINDEXCELL

#include "SpatialIndexBaseNode.h"

class SpatialIndexCell : public SpatialIndexBaseNode
{
  public:
     SpatialIndexCell();
    ~SpatialIndexCell();
    
    void Accept(TileGraphVisitor* visitor);
    void RangeVisibility(bool value);
    bool RangeVisibility(void);
  
  private:
    bool m_bRangeVisibility;
};

#endif