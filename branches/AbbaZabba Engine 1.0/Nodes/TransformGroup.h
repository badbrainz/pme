#ifndef TRANSFORMGROUP_H
#define TRANSFORMGROUP_H

#include "../Geometry/DistanceObject.h"
#include "../Viewer/Frustum.h"
#include "Transform3D.h"
#include "Shape.h"

typedef DistanceObject<TransformGroup*> DistanceObjectTGP;
typedef vector<DistanceObjectTGP>       DistanceBufferTGP;

  class TransformGroup : public NamedObject, public IOXMLObject
{
  public:
    TransformGroup();
   ~TransformGroup(){destroy();};
    TransformGroup(const Transform3D &);

    virtual bool loadXMLSettings(XMLElement *element);
    virtual bool exportXMLSettings(ofstream &xmlFile);
 
    bool loadXMLShape(XMLElement *node);

    void renderSilhouettes(Tuple3f &viewPosition, Frustum *frustum = NULL);
    void transformToWorldSpace(BoundsDescriptor &descriptor);
    void transformToObjectSpace(Tuple3f &tuple);
    void transformToWorldSpace(Tuple3f &tuple);
    void updateBoundsDescriptor();
    void renderAABB();
    void renderOBB();

    void renderLightFrustum(Tuple3f &lightPosition);
    void setTransform(const Transform3D &t);
    void setOccluder(bool);
    void undoTransform();
    void doTransform();

    void renderShadowVolumes(const Tuple3f& lightPosition,
                             float          lightRange,
                             int            renderingFormat);

    void setParent(TransformGroup *);
    bool addChild(TransformGroup *child);

    TransformGroup *removeGroup(const char *name);
    TransformGroup *removeGroup(int        index);

    bool addChild(Shape *child);
    void setVisible(bool);
    void compile();

    float getEstimatedVolumeSize();
    int   render(int renderingFormat, Frustum *frustum = NULL);
    int   getShapesCount();
    int   getGroupsCount();

    BoundsDescriptor        &updateAndGetBoundsDescriptor();
    BoundsDescriptor        &getBoundsDescriptor();

    vector<TransformGroup*> &getGroups();
    vector<Shape*>          &getShapes();
    TransformGroup          *getParent();
    TransformGroup          *getGroup(const char* groupName);
    TransformGroup          *getGroup(int index);
    Transform3D             &getTransform();
    Shape                   *getShapeByPickingIndex(int pickingIndex, Frustum *culling = NULL);
    Shape                   *pickClosest(const Tuple2i &mouse,
                                         const Tuple2i &pickingAreaSize,
                                         const Tuple4f &fovAspectNear,
                                         Frustum *cull = NULL);
    Shape                   *getShape(int index);

    const Matrix4f &getWorldTransform() const;
    bool isOccluder(){ return occluder; }
    bool isVisible() { return visible;  }
    void destroy();

  private:
    static DistanceBufferTGP groupsObjectList;

    vector<TransformGroup*>  groups;
    vector<Shape*>           shapes;

    BoundsDescriptor         boundsDescriptor;
    TransformGroup          *parent;
    Transform3D              transform;
    Matrix4f                 worldTransform;
    bool                     applyWorldTransform,
                             visibleOccluder,                           
                             occluder,
                             visible;

    void  renderShadowVolumesPrivate(const Tuple3f& lightPosition,
                                     float lightRange,
                                     int   renderingFormat);
    void  collectTransformGroups(TransformGroup *group,
                                 Frustum         *frustum = NULL);
    void  updateWorldTransform();
    void  setupLightFrustum(const Tuple3f &lightPosition);
    void  pickClosest(Frustum *cullingFrustum);

    int   renderOrdered(const    Tuple3f &eyePosition,
                        int      renderingFormat,
                        int      order,
                        Frustum *frustum = NULL);

};

#endif