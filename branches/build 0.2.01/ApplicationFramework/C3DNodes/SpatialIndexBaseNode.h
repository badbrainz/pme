#ifndef SPATIALINDEXBASENODE
#define SPATIALINDEXBASENODE

#include "GraphNode.h"
#include "Math/MathUtils.h"
#include "Geometry/BoundsDescriptor.h"

class TileGraphVisitor;

class SpatialIndexBaseNode : public GraphNode
{
  public:
    SpatialIndexBaseNode();
    virtual ~SpatialIndexBaseNode();
    
    virtual void      Accept(TileGraphVisitor* visitor);
    
    void              SetBoundsDescriptor(const BoundsDescriptor &descriptor);
    void              UpdateBounds(BoundsDescriptor &descriptor);
    BoundsDescriptor &GetBoundsDescriptor(void);
    
    void              SetRange(const Tuple4i &range);
    Tuple4i          &GetRange();
    void              SetLevel(int level);
    int               GetLevel();
  
  protected:
    BoundsDescriptor  m_BoundsDescriptor;
    Tuple4i           m_Range;
    int               m_Level;
};



#endif