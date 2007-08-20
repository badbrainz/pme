#include "TransformGroup.h"
#include "../Renderer/Renderer.h"
#include "../Tools/Factory3DS.h"

#define  NV_SHADOW_PASS  0x00001
#define  JOE_SHADOW_PASS 0x00002
#define  ATI_SHADOW_PASS 0x00004


DistanceBufferTGP TransformGroup::groupsObjectList;
Tuple3f           corners[4];
Planef            lightFrustum[5];

TransformGroup::TransformGroup() : IOXMLObject("Group")
{
  applyWorldTransform = false;
  visibleOccluder     = true;
  occluder            = false;
  visible             = true;
  parent              = NULL;
}

TransformGroup::TransformGroup(const Transform3D &t)  : IOXMLObject("Group")
{
  setTransform(t);
}

int TransformGroup::render(int renderingFormat, Frustum *frustum)
{
  size_t index = 0;
  int    order = (renderingFormat & FRONT_TO_BACK) ? FRONT_TO_BACK :
                 (renderingFormat & BACK_TO_FRONT) ? BACK_TO_FRONT : 0,
         tc    = 0;

  if(!visible || (renderingFormat & CULL_OCCLUDERS && !visibleOccluder))
    return tc;

  if(order)
  {
    TransformGroup *currentGroup   = NULL;
    Tuple3f         newEyePosition = Renderer::getCameraPosition(),
                    groupCenter;          

    size_t          shapesCount   = 0,
                    i             = 0;

    float           coeff         = (order == FRONT_TO_BACK) ? 1.0f : -1.0f;
 
    groupsObjectList.clear();
    collectTransformGroups(this, frustum);

    for(i = 0; i < groupsObjectList.size(); i++)
    {
      currentGroup = groupsObjectList[i].getObject();
      currentGroup->updateBoundsDescriptor();

      groupCenter = currentGroup->boundsDescriptor.getCenterOBB();
      groupsObjectList[i].setDistance(coeff*groupCenter.getDistance(newEyePosition));
    }  

    sort(groupsObjectList.begin(), groupsObjectList.end());
 
    for(i = 0; i < groupsObjectList.size(); i++)
    {
      currentGroup  = groupsObjectList[i].getObject();

      if(!currentGroup->visible)
        continue;
 
      shapesCount = currentGroup->shapes.size();
      currentGroup->doTransform();

      for(size_t index = 0; index <shapesCount; index++)
        tc += currentGroup->shapes[index]->draw(renderingFormat);
 
      currentGroup->undoTransform();
    }
  }
  else
  {
    updateBoundsDescriptor();

    if(frustum)
      if(!frustum->AABBInFrustum(boundsDescriptor))
        return 0;

     doTransform();

     for(index = 0; index < shapes.size(); index++)
       tc +=shapes[index]->draw(renderingFormat);

     undoTransform();

     for(index = 0; index < groups.size(); index++)
       tc += groups[index]->render(renderingFormat, frustum);
  }
  return tc;
}

void TransformGroup::renderSilhouettes(Tuple3f &viewPosition,
                                       Frustum *frustum)
{
  Tuple3f viewPos(viewPosition);
  size_t  index         = 0;

  transformToObjectSpace(viewPos);

  if(frustum)
    if(!frustum->AABBInFrustum(updateAndGetBoundsDescriptor()))
       return;

  doTransform();

  for(index = 0; index < shapes.size(); index++)
    shapes[index]->renderSilhouette(Tuple4f(viewPos, 0.0f));

  undoTransform();

  for(index = 0; index < groups.size(); index++)
    groups[index]->renderSilhouettes(viewPosition, frustum);
}

void TransformGroup::renderShadowVolumes(const Tuple3f& lightPosition,
                                         float          lightRange,
                                         int            renderPath)
{
  int  selectedShadowPass = (GLEE_ATI_separate_stencil)    ?
                             ATI_SHADOW_PASS               :
                            (GLEE_EXT_stencil_two_side)    ?
                             NV_SHADOW_PASS : JOE_SHADOW_PASS;

  selectedShadowPass |= renderPath;

  GLboolean turnLightingBackOn = glIsEnabled(GL_LIGHTING);
  glDisable(GL_LIGHTING);

  Tuple4i   scissor;
  Tuple2f   zMinZMax(0.0f, 1.0f);

 /* if(GLEE_EXT_depth_bounds_test)
  {
    scissor = Renderer::getScissorRect(lightInfo);
    if(GLEE_EXT_depth_bounds_test){
      glDepthBoundsEXT(zMinZMax.x, zMinZMax.y);
      glEnable(GL_DEPTH_BOUNDS_TEST_EXT);
    }
  }
  else */

  scissor= Renderer::getScissorRect(lightPosition, lightRange);
  setupLightFrustum(lightPosition);
  updateWorldTransform();

  glDepthMask(GL_FALSE);
  glEnable(GL_SCISSOR_TEST);
  
  glScissor(scissor[0], scissor[1], scissor[2] - scissor[0], scissor[3] - scissor[1]);

  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glColorMask(0, 0, 0, 0);
  glClear(GL_STENCIL_BUFFER_BIT);
  glShadeModel(GL_FLAT);
  glDepthFunc(GL_LESS);
  glEnable(GL_STENCIL_TEST);
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(0.3f,0.3f);
  glStencilFunc(GL_ALWAYS, 0, ~0);

  if(GLEE_NV_depth_clamp)
    glEnable(GL_DEPTH_CLAMP_NV);

  if(renderPath & VOLUME_EXTRUSION_GPU)
    Renderer::enableGPUVolumeExtrusion();

  renderShadowVolumesPrivate(lightPosition, lightRange, selectedShadowPass);

  if(renderPath & VOLUME_EXTRUSION_GPU)
    Renderer::disableGPUVolumeExtrusion();

  glPopAttrib();

  if(turnLightingBackOn)
     glEnable(GL_LIGHTING);

  glPushAttrib(GL_ENABLE_BIT);
  glStencilFunc(GL_EQUAL, 0, ~0);
  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
  glEnable(GL_STENCIL_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glColorMask(1,1,1,1);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE);
  glDepthFunc(GL_LEQUAL);

  render(CULL_OCCLUDERS   |
         CULL_FRUSTUM     |
         COLOR | MATERIAL | ALL_TEXTURES | SHADERS | ALPHA_ATTR);

  glDisable(GL_BLEND);
  glPopAttrib();

  /*if(GLEE_EXT_depth_bounds_test)
    glDisable(GL_DEPTH_BOUNDS_TEST_EXT);*/

  glDisable(GL_SCISSOR_TEST);
  glDepthMask(GL_TRUE);
  glDepthFunc(GL_LEQUAL);
}

void TransformGroup::renderShadowVolumesPrivate(const Tuple3f& lightPosition,
                                                float lightRange,
                                                int   renderPath)
{
  Tuple3f lightPosObjSpace3f(lightPosition);
  Tuple4f lightPosObjSpace4f;

  if(!updateAndGetBoundsDescriptor().sphereOverlapsAABB(lightPosition, lightRange))
  {
    visibleOccluder = false;
    return;
  }

  transformToObjectSpace(lightPosObjSpace3f);
  lightPosObjSpace4f.set(lightPosObjSpace3f, lightRange);

  visibleOccluder = true;

  doTransform();

  if(occluder)
  {
    bool  needZFail  = boundsDescriptor.frustumOverlapsAABB(lightFrustum, 5);

    for(size_t s = 0; s < shapes.size(); s++)
    {
      if(renderPath & NV_SHADOW_PASS || renderPath & ATI_SHADOW_PASS){
        glDisable(GL_CULL_FACE);

        if(renderPath & ATI_SHADOW_PASS){
          glStencilFuncSeparateATI(GL_ALWAYS, GL_ALWAYS, 0, ~0);
          if(needZFail){
            glStencilOpSeparateATI(GL_FRONT, GL_KEEP, GL_DECR_WRAP_EXT, GL_KEEP);
            glStencilOpSeparateATI(GL_BACK,  GL_KEEP, GL_INCR_WRAP_EXT, GL_KEEP);
          }
          else
          {
            glStencilOpSeparateATI(GL_FRONT, GL_KEEP, GL_KEEP, GL_INCR_WRAP_EXT);
            glStencilOpSeparateATI(GL_BACK,  GL_KEEP, GL_KEEP, GL_DECR_WRAP_EXT);
          }
        }

        if(renderPath & NV_SHADOW_PASS){
          if(needZFail){
            glActiveStencilFaceEXT(GL_BACK);
            glStencilFunc(GL_ALWAYS, 0, ~0);
            glStencilOp(GL_KEEP, GL_INCR_WRAP_EXT, GL_KEEP);
            glActiveStencilFaceEXT(GL_FRONT);
            glStencilFunc(GL_ALWAYS, 0, ~0);
            glStencilOp(GL_KEEP, GL_DECR_WRAP_EXT, GL_KEEP);
          }
          else
          {
            glActiveStencilFaceEXT(GL_FRONT);
            glStencilFunc(GL_ALWAYS, 0, ~0);
            glStencilOp(GL_KEEP, GL_KEEP, GL_INCR_WRAP_EXT);
            glActiveStencilFaceEXT(GL_BACK);
            glStencilFunc(GL_ALWAYS, 0, ~0);
            glStencilOp(GL_KEEP, GL_KEEP, GL_DECR_WRAP_EXT);
          }
          glEnable(GL_STENCIL_TEST_TWO_SIDE_EXT);
        }
      }
      else
      {
        if(needZFail){
          glStencilOp(GL_KEEP, GL_INCR, GL_KEEP);
          glCullFace(GL_FRONT);
          shapes[s]->doShadowPass(lightPosObjSpace4f, needZFail, renderPath);
          glStencilOp(GL_KEEP, GL_DECR, GL_KEEP);
          glCullFace(GL_BACK);
        }
        else
        {
          glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
          glCullFace(GL_BACK);
          shapes[s]->doShadowPass(lightPosObjSpace4f, needZFail, renderPath);
          glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
          glCullFace(GL_FRONT);
        }
      }
      shapes[s]->doShadowPass(lightPosObjSpace4f, needZFail, renderPath);
    }
  }
  undoTransform();

  for(size_t g = 0; g < groups.size(); g++)
    groups[g]->renderShadowVolumesPrivate(lightPosition, lightRange, renderPath);
}

void TransformGroup::renderAABB()
{
  if(updateAndGetBoundsDescriptor().isInitialized())
    boundsDescriptor.render(BoundsDescriptor::WIRE | BoundsDescriptor::AABB);

  for(size_t g = 0; g < groups.size(); g++)
    groups[g]->renderAABB();
}

void TransformGroup::renderOBB()
{
  if(updateAndGetBoundsDescriptor().isInitialized())
    boundsDescriptor.render(BoundsDescriptor::WIRE | BoundsDescriptor::OBB);

  for(size_t g = 0; g < groups.size(); g++)
    groups[g]->renderOBB();
}

Shape *TransformGroup::pickClosest(const Tuple2i &mouse,
                                   const Tuple2i &rect,
                                   const Tuple4f &fov,
                                   Frustum *cull)
{
  Tuple4i viewport;
  GLuint  selectionBuffer[100],
          hits;

  ZeroMemory(selectionBuffer, 100);

  glGetIntegerv(GL_VIEWPORT, viewport);

  glSelectBuffer(100, selectionBuffer);
  glRenderMode(GL_SELECT);
  glInitNames();
  glPushName(-1);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluPickMatrix(mouse.x, mouse.y, rect.x, rect.y, viewport);
  gluPerspective(fov.x, fov.y, fov.z, fov.w);
  glMatrixMode(GL_MODELVIEW);

  glColorMask(0,0,0,0);
  pickClosest(cull);
  glColorMask(1,1,1,1);

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glFlush();

  hits = glRenderMode(GL_RENDER);
  if(hits)
  {

    GLuint dist    = 4294967295U,
           closest = 0;

    while (hits)
    {
      if(selectionBuffer[(hits - 1) * 4 + 1] < dist)
      {
        dist    = selectionBuffer[(hits - 1) * 4 + 1];
        closest = selectionBuffer[(hits - 1) * 4 + 3];
      }
      hits--;
    }
    return getShapeByPickingIndex(closest, cull);
  }
  return NULL;
}

void  TransformGroup::pickClosest(Frustum *frustum)
{
  size_t i = 0;

  if(frustum)
    if(!frustum->AABBInFrustum(boundsDescriptor))
       return;

  if(!visible)
    return;

  doTransform();
  for(i = 0; i < shapes.size(); i++)
  {
    glLoadName(shapes[i]->getPickingIndex());
    shapes[i]->draw(0);
  }
  undoTransform();

  for(i = 0; i < groups.size(); i++)
    groups[i]->pickClosest(frustum);
}

Shape *TransformGroup::getShapeByPickingIndex(int pickingIndex, Frustum *frustum)
{
  if(frustum)
    if(!frustum->AABBInFrustum(boundsDescriptor))
       return NULL;

  size_t i = 0;
  Shape *target = NULL;

  for(i = 0; i < shapes.size(); i++)
    if(shapes[i]->getPickingIndex() == pickingIndex)
      return shapes[i];

  for(i = 0; i < groups.size(); i++)
    if(target = groups[i]->getShapeByPickingIndex(pickingIndex, frustum))
      break;

  return target;
}

void TransformGroup::setupLightFrustum(const Tuple3f &lightPosition)
{
  Matrix4f  matrixBuffer;
  Tuple4i   viewport;
  Tuple3f   nearPlaneNormal,
            cameraPosition,
            rightVector,
            upVector;

  float     distance = 0.0f;

  glGetIntegerv(GL_VIEWPORT, viewport);
  glGetFloatv(GL_MODELVIEW_MATRIX , matrixBuffer);

  rightVector.set(matrixBuffer[0],matrixBuffer[4], matrixBuffer[8]);
  upVector.set   (matrixBuffer[1],matrixBuffer[5], matrixBuffer[9]),

  nearPlaneNormal.crossProduct(upVector, rightVector),
  nearPlaneNormal.normalize();

  matrixBuffer.setInverse();

  cameraPosition.set(matrixBuffer[12], matrixBuffer[13], matrixBuffer[14]);
  cameraPosition += nearPlaneNormal;

  distance = nearPlaneNormal.getDotProduct(lightPosition) -
             nearPlaneNormal.getDotProduct(cameraPosition);

  rightVector *= float(viewport.z)/viewport.w;

  corners[0]  =
  corners[1]  =
  corners[2]  =
  corners[3]  = cameraPosition;

  corners[0] -= upVector;
  corners[0] -= rightVector;

  corners[1] -= upVector;
  corners[1] += rightVector;

  corners[2] += rightVector;
  corners[2] += upVector;

  corners[3] += upVector;
  corners[3] -= rightVector;

  if(distance > EPSILON)
  {
    lightFrustum[0].set(lightPosition, corners[0], corners[1]);
    lightFrustum[1].set(lightPosition, corners[1], corners[2]);
    lightFrustum[2].set(lightPosition, corners[2], corners[3]);
    lightFrustum[3].set(lightPosition, corners[3], corners[0]);
    lightFrustum[4].set(corners[0], corners[2], corners[1]);
  }
  else
  {
    lightFrustum[0].set(lightPosition, corners[1], corners[0]);
    lightFrustum[1].set(lightPosition, corners[2], corners[1]);
    lightFrustum[2].set(lightPosition, corners[3], corners[2]);
    lightFrustum[3].set(lightPosition, corners[0], corners[3]);
    lightFrustum[4].set(corners[0], corners[1], corners[2]);
  }
}

void TransformGroup::renderLightFrustum(Tuple3f &lightPosition)
{
  glPushMatrix();
  glColor3f(0,0,1);
  glBegin(GL_LINES);
  for(int i = 0; i < 4; i++)
  {
    glVertex3fv(lightPosition);
    glVertex3fv(corners[i]);
  }
  glEnd();
  glPopMatrix();
}

void TransformGroup::doTransform()
{
  if(applyWorldTransform)
  {
    glPushMatrix();
    glMultMatrixf(worldTransform);
  }
}

void TransformGroup::undoTransform()
{
  if(applyWorldTransform)
    glPopMatrix();
}

bool TransformGroup::addChild(Shape *child)
{

  if(!child)
    return Logger::writeErrorLog("Couldn't add shape to TransformGroup: NULL");

  if(!child->getGeometry() || !child->getAppearance())
    return Logger::writeErrorLog("Couldn't add shape to TransformGroup: NULL Geometry or Appearance");

  if(child->getParent())
    return Logger::writeErrorLog("Couldn't add shape to TransformGroup: Already has a parent");

  if(occluder)
    child->getGeometry()->activeInfoShadow();

  Logger::writeInfoLog(String("Adding shape to TransformGroup"));

  child->setParent(this);
  shapes.push_back(child);
  return true;
}

bool TransformGroup::addChild(TransformGroup *child)
{

  if(!child)
    return Logger::writeErrorLog("Couldn't add Group to TransformGroup: NULL");

  if(this == child)
    return Logger::writeErrorLog("Couldn't add shape to TransformGroup: parent is same as child");

  if(child->getParent())
    return Logger::writeErrorLog("Couldn't add shape to TransformGroup: Already has a parent");

  Logger::writeInfoLog(String("Adding Group to TransformGroup"));

  groups.push_back(child);
  child->setParent(this);
  transform.overrideUpdatedFlag(true);
  updateWorldTransform();
  return true;
}

void TransformGroup::setParent(TransformGroup *prt)
{
  parent = prt;
}

void TransformGroup::transformToWorldSpace(BoundsDescriptor &descriptor)
{
  updateWorldTransform();
  descriptor *= worldTransform;
}

void TransformGroup::transformToObjectSpace(Tuple3f &tuple)
{
  updateWorldTransform();
  Matrix4f inverse = worldTransform;
  inverse.setInverse();
  tuple *= inverse;
}

void TransformGroup::transformToWorldSpace(Tuple3f &tuple)
{
  updateWorldTransform();
  tuple *= worldTransform;
}

void TransformGroup::compile()
{
  for(size_t s = 0; s < shapes.size(); s++)
    if(occluder)
      shapes[s]->compile(true);
    else
      shapes[s]->compile(false);

  for(size_t g = 0; g < groups.size(); g++)
    groups[g]->compile();
}

void TransformGroup::setVisible(bool vis)
{
  visible = vis;
}

int TransformGroup::getShapesCount()
{
  return int(shapes.size());
}

int TransformGroup::getGroupsCount()
{
  return int(groups.size());
}

Shape *TransformGroup::getShape(int index)
{
  return  shapes.at(index);
}

TransformGroup *TransformGroup::getParent()
{
  return parent;
}

TransformGroup *TransformGroup::getGroup(int index)
{
  return  groups.at(index);
}

vector<Shape*> &TransformGroup::getShapes()
{
  return shapes;
}

vector<TransformGroup*> &TransformGroup::getGroups()
{
  return groups;
}

TransformGroup *TransformGroup::getGroup(const char* groupName)
{
  TransformGroup *target = NULL;

  if(!groupName)
    return NULL;

  if(name == groupName)
    return this;

  for(size_t i = 0; i < groups.size(); i++)
    if(target = groups[i]->getGroup(groupName))
      break;

  return target;
}

void TransformGroup::setTransform(const Transform3D &t)
{
  transform = t;
  transform.overrideUpdatedFlag(true);
  updateWorldTransform();
}

Transform3D &TransformGroup::getTransform(){
  return transform;
}

void TransformGroup::setOccluder(bool occ){
  occluder = occ;
}

void TransformGroup::updateBoundsDescriptor()
{
  updateWorldTransform();
  boundsDescriptor.reset();

  for(size_t s = 0; s < shapes.size(); s++)
  {
    if(!boundsDescriptor.isInitialized())
      boundsDescriptor  = shapes[s]->getGeometry()->getBoundsDescriptor();
    else
      boundsDescriptor += shapes[s]->getGeometry()->getBoundsDescriptor();
  }

  if(boundsDescriptor.isInitialized())
    boundsDescriptor *= worldTransform;

  for(size_t g = 0; g < groups.size(); g++)
    if(!boundsDescriptor.isInitialized())
      boundsDescriptor  = groups[g]->updateAndGetBoundsDescriptor();
    else
      boundsDescriptor += groups[g]->updateAndGetBoundsDescriptor();
}

BoundsDescriptor &TransformGroup::updateAndGetBoundsDescriptor()
{
  updateBoundsDescriptor();
  return getBoundsDescriptor();
}

BoundsDescriptor &TransformGroup::getBoundsDescriptor()
{
  return boundsDescriptor;
}

float TransformGroup::getEstimatedVolumeSize()
{
  Tuple3f diagonal  = boundsDescriptor.getMaxEndAABB();
          diagonal -= boundsDescriptor.getMinEndAABB();
  return diagonal.getLength();
}


bool TransformGroup::loadXMLSettings(XMLElement *element)
{
  if(!isSuitable(element))
    return false;

  XMLElement  *child    = NULL;
  destroy();

  for(size_t i = 0; i < element->getChildrenCount(); i++)
  {
    if(!(child = element->getChild(i)))
      continue;
    const String &childName = child->getName();

    if(childName == "visible")
      setVisible(child->getValue() == "true");

    if(childName == "occluder")
      setOccluder(child->getValue() == "true");

    if(childName == "name")
      setName(child->getValue());

    if(childName == "Transform")
    {
      transform.loadXMLSettings(child);
      continue;
    }

    if(childName == "Group")
    {
      TransformGroup *group = new TransformGroup();
      if(!group->loadXMLSettings(child))
      {
        deleteObject(group);
      }
      else
        addChild(group);
      continue;
    }

    if(childName == "Shape")
    {
      loadXMLShape(child);
      continue;
    }
  }
  return true;
}

bool TransformGroup::exportXMLSettings(ofstream &xmlFile  )
{
  if(!xmlFile.is_open())
    return Logger::writeErrorLog("Cannot export Group to XML: file not ready.");

  xmlFile << "<Group visible = \"" << (visible ? "true\"" : "false\"")
          << (!name.getLength() ? ">\n" : (String("\n        name    = \"") + name +  "\"> \n"));
  transform.exportXMLSettings(xmlFile);

  size_t c = 0;

  ofstream shapeFile;
  for(c = 0; c < shapes.size(); c++)
  {
    if(!shapeFile.is_open())
    {
      shapeFile.open(shapes[c]->getGeometry()->getName(), ios::app | ios::binary);
    }
    shapes[c]->exportXMLSettings(shapeFile);
  }

  if(shapeFile.is_open())
  {
    xmlFile << "  <Shape path = \"" << shapes[0]->getGeometry()->getName() << "\" />\n";
    shapeFile.close();
  }

  for(c = 0; c < groups.size(); c++)
    groups[c]->exportXMLSettings(xmlFile);

  xmlFile << "</Group>" << endl;
  return true;
}

bool TransformGroup::loadXMLShape(XMLElement *element)
{
  if(!element)
    return  Logger::writeErrorLog("NULL Shape node");

  if(element->getName() != "Shape")
    return Logger::writeErrorLog("Need a <Shape> tag in the XML Node");

  XMLElement *child;
  XMLStack    stack;

  if(!(child = element->getChildByName("path")))
    return Logger::writeErrorLog("Need a path child in <Shape>");

  const char* verifiedPath = MediaPathManager::lookUpMediaPath(child->getValuec());
  bool  visible  = true;
  char  stringBuffer[256];
  int   meshCount    = 0;

  if(!verifiedPath)
    return Logger::writeErrorLog(String("Couldn't locate Shape file at ->") + child->getValuec());

  if(strstr(verifiedPath, ".3ds") || strstr(verifiedPath, ".3DS"))
    return load3DS(this, true, (char*)verifiedPath);

  stack.loadXMLFile(verifiedPath);

  if(child = element->getChildByName("visible"))
     visible = (child->getValue() == "true");

  for(size_t i = 0; i < stack.getChildrenCount(); i++)
  {
    if(!(child = stack.getChild(i)))
      continue;

    if(child->getName() == "Shape")
    {
      sprintf(stringBuffer, "%s_%d", verifiedPath, meshCount++);
      Shape *shape = new Shape();

      if(shape->loadXMLSettings(child, stringBuffer))
      {
        shape->setVisible(visible);
        addChild(shape);
      }
      else
      {
        deleteObject(shape);
        Logger::writeErrorLog(String("Failed to load Shape -> ") + stringBuffer);
      }
    }
  }
  return true;
}

void  TransformGroup::updateWorldTransform()
{
  if(transform.updatedFlagOn())
  { 
    if(parent)
    {
      worldTransform  = parent->getWorldTransform();
      worldTransform *= transform.getMatrix4f();
    }
    else
      worldTransform = transform.getMatrix4f();

    for(size_t g = 0; g < groups.size(); g++)
    {
      groups[g]->transform.overrideUpdatedFlag(true);
      groups[g]->updateWorldTransform();
    }
    transform.overrideUpdatedFlag(false);
    applyWorldTransform = (worldTransform != IDENTITY);
  }
}

const Matrix4f &TransformGroup::getWorldTransform() const
{
  return worldTransform;
}

void TransformGroup::collectTransformGroups(TransformGroup *parent,
                                            Frustum        *frustum)
{
  const vector<TransformGroup*> &children = parent->groups;

  if(parent && !frustum)
  {
    for(size_t g = 0; g < children.size(); g++)
    {
      groupsObjectList.push_back(DistanceObjectTGP(children[g]));   
      collectTransformGroups(children[g]);
    }
  }

  if(parent && frustum)
  {
    for(size_t g = 0; g < children.size(); g++)
    {
      if(frustum->AABBInFrustum(children[g]->getBoundsDescriptor()))
      {
        groupsObjectList.push_back(DistanceObjectTGP(children[g]));   
        collectTransformGroups(children[g], frustum);
      }
    }
  }
}

TransformGroup *TransformGroup::removeGroup(const char *name)
{
  TransformGroup *target = NULL;

  if(name)
  {
    for(size_t i =0; i < groups.size(); i++)
      if((groups[i]->getName() == name) && !target)
      {
        target = removeGroup(int(i));
        break;
      }
      else
        target = groups[i]->removeGroup(name);
  }
  return target;
}

TransformGroup *TransformGroup::removeGroup(int index)
{
  if(index < 0 || index >= int(groups.size()))
    return NULL;
  
  vector<TransformGroup*>  temp;
  TransformGroup          *target;

  for(size_t i =0; i < groups.size(); i++)
   if(index != int(i))
     temp.push_back(groups[i]);
   else
     target = groups[i];

  groups.clear();

  for(size_t i =0; i < temp.size(); i++)
    groups.push_back(temp[i]);

  temp.clear();
 
  target->setParent(NULL);
  target->getTransform().overrideUpdatedFlag(true);
  target->updateWorldTransform();

  return target;
}

void TransformGroup::destroy()
{
  for(size_t i =0; i < shapes.size(); i++)
    deleteObject(shapes[i]);

  for(size_t i =0; i < groups.size(); i++)
    deleteObject(groups[i]);

  parent   = NULL;
  visible  = true;
  occluder = false;

  groups.clear();
  shapes.clear();
}