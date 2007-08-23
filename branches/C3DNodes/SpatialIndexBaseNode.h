#ifndef SPATIALINDEXBASENODE
#define SPATIALINDEXBASENODE

#include "Node.h"
#include "Math/MathUtils.h"
#include "Geometry/BoundsDescriptor.h"

class SpatialIndexVisitor;

class SpatialIndexBaseNode : public Node
{
  public:
    SpatialIndexBaseNode();
    virtual ~SpatialIndexBaseNode();
    
    virtual void      Accept(SpatialIndexVisitor* visitor);
    
    void              SetBoundsDescriptor(const BoundsDescriptor &descriptor);
    void              UpdateBounds(BoundsDescriptor &descriptor);
    BoundsDescriptor &GetBoundsDescriptor(void);
    
    void              SetRange(const Tuple4i &range);
    Tuple4i          &GetRange();
    void              SetLevel(int level);
    int               GetLevel();
    
    void Destroy(){m_BoundsDescriptor.reset();}
  
  protected:
    BoundsDescriptor  m_BoundsDescriptor;
    Tuple4i           m_Range;
    int               m_Level;
};



#endif