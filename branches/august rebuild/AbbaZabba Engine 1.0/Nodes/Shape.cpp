#include "Shape.h"
#include "TransformGroup.h"

#include "../Renderer/Renderer.h"
#include "../Managers/GeometryManager.h"

int pickingIndicesProgress = 1;

Shape::Shape(Geometry *geom, Appearance *app)
{
  initialize();
  setAppearance(app);
  setGeometry(geom);
  visible        = true;
}

Shape::Shape(const Shape &shape)
{
  if(this != &shape)
    initialize();

  this->operator=(shape);
}

Shape &Shape::operator = (const Shape &shape)
{
  if(this != &shape)
  {
    if(shape.appearance)
      setAppearance(shape.appearance);

    if(shape.geometry)
      setGeometry(shape.geometry);

    pickingIndex = pickingIndicesProgress++;
    visible      = shape.visible;
  }
  return *this;
}

void Shape::initialize()
{
  pickingIndex = pickingIndicesProgress++;
  appearance   = NULL;
  geometry     = NULL;
  visible      = true;
  parent       = NULL;
}

void Shape::setParent(TransformGroup *parentArg)
{
  if(!parentArg) return;
  parent = parentArg;
}

TransformGroup *Shape::getParent()
{
  return parent;
}

int Shape::draw(int rendering)
{
  if(!appearance || !geometry || !visible)
    return 0;

  int  format = geometry->getVertexFormat();

  if(rendering & MATERIAL && (appearance->getMaterial()))
    appearance->getMaterial()->bindMaterial();

  if(rendering & BLEND_ATTR)
    appearance->getBlendAttributes().apply();

  if(rendering & ALPHA_ATTR)
    appearance->getAlphaAttributes().apply();

  if(appearance->colorOn())
    glColor4fv(appearance->getColor());

  if(rendering  & SHADERS)
    if(appearance->shadersOn())
      appearance->getShaders().activate();

  if(format & RESIDE_ON_GFX_DEVICE)
    drawVBOs(rendering);
  else
  {
    enableTextureUnits(rendering);
    drawRegular(rendering);
  }

  if(format & VERTICES)
    glDisableClientState(GL_VERTEX_ARRAY);

  if(format & NORMALS)
    glDisableClientState(GL_NORMAL_ARRAY);

  if((format & COLOR) && (rendering & COLOR))
    glDisableClientState(GL_COLOR_ARRAY);

  disableTexureUnits(rendering);

  if(rendering  & SHADERS)
    if(appearance->shadersOn())
      appearance->getShaders().deactivate();

  if(appearance->colorOn())
    glColor3f(1,1,1);

  if(rendering & BLEND_ATTR)
    appearance->getBlendAttributes().stop();

  if(rendering & ALPHA_ATTR)
    appearance->getAlphaAttributes().stop();

  if(format & RESIDE_ON_GFX_DEVICE)
  {
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
    if(geometry->getIndicesVBOID())
      glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
  }
  return geometry->getTriangleCount();
}

void Shape::drawVBOs(int rendering)
{
  int  format  = geometry->getVertexFormat();

  glBindBufferARB(GL_ARRAY_BUFFER_ARB, geometry->getElementsVBOID());
  enableTextureUnits(rendering);

  if((format & COLOR) && (rendering & COLOR))
  {
    glColorPointer(3, GL_FLOAT, geometry->getStrideSize(), OFFSET(geometry->getOffset(COLOR)));
    glEnableClientState(GL_NORMAL_ARRAY);
  }

  if(format & NORMALS)
  {
    glNormalPointer(GL_FLOAT, geometry->getStrideSize(), OFFSET(geometry->getOffset(NORMALS)));
    glEnableClientState(GL_NORMAL_ARRAY);
  }

  if(format & VERTICES)
  {
    glVertexPointer(3, GL_FLOAT, geometry->getStrideSize(), OFFSET(geometry->getOffset(VERTICES)));
    glEnableClientState(GL_VERTEX_ARRAY);
  }

  if(geometry->getIndicesVBOID())
  {
    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, geometry->getIndicesVBOID());
    glDrawElements(geometry->getType(),  geometry->getIndicesCount(),
                   GL_UNSIGNED_INT,  0);
  }
  else
  {
    if(geometry->getIndices())
      glDrawElements(geometry->getType(), geometry->getIndicesCount(),
                     GL_UNSIGNED_INT, geometry->getIndices());
    else
      glDrawArrays(geometry->getType(), 0, geometry->getIndicesCount());
  }
}

void Shape::drawRegular(int rendering)
{
  int format  = geometry->getVertexFormat();

  if((format & COLOR) && (rendering & COLOR))
  {
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(3, GL_FLOAT, 0,   geometry->getColors());
  }

  if(format & NORMALS)
  {
    glNormalPointer(GL_FLOAT, geometry->getStrideSize(), geometry->getNormals());
    glEnableClientState(GL_NORMAL_ARRAY);
  }

  if(format & VERTICES)
  {
    glVertexPointer(3, GL_FLOAT, geometry->getStrideSize(), geometry->getVertices());
    glEnableClientState(GL_VERTEX_ARRAY);
  }
  if(geometry->getIndices())
    glDrawElements(geometry->getType(), geometry->getIndicesCount(),
                   GL_UNSIGNED_INT, geometry->getIndices());
  else
    glDrawArrays(geometry->getType(), 0, geometry->getIndicesCount());
}

bool Shape::isTextureActive(int format, int index)
{
  bool activeTexture = false;

  switch(index)
  {
    case 7: activeTexture = ((format & TEXTURE7) != 0); break;
    case 6: activeTexture = ((format & TEXTURE6) != 0); break;
    case 5: activeTexture = ((format & TEXTURE5) != 0); break;
    case 4: activeTexture = ((format & TEXTURE4) != 0); break;
    case 3: activeTexture = ((format & TEXTURE3) != 0); break;
    case 2: activeTexture = ((format & TEXTURE2) != 0); break;
    case 1: activeTexture = ((format & TEXTURE1) != 0); break;
    case 0: activeTexture = ((format & TEXTURE0) != 0); break;
    default: activeTexture = false;
  }
  return activeTexture;
}

void Shape::enableTextureUnits(int rendering)
{
  Texture *texturePointer;
  bool       activeTexture = false;
  int        format        = geometry->getVertexFormat();

  for(int i =0; i < Renderer::getMaxAvailableTextureUnits(); i++)
  {
    if(!isTextureActive(rendering, i))
      continue;
    activeTexture = false;

    if(geometry->getTextureElementsCount(i) > 0)
    {
      glClientActiveTextureARB(GL_TEXTURE0_ARB + i);
      if(format & RESIDE_ON_GFX_DEVICE)
        glTexCoordPointer(geometry->getTextureElementsCount(i),
                          GL_FLOAT,
                          geometry->getStrideSize(),
                          OFFSET(geometry->getOffset(TEXTURE0 << i)));
      else
        glTexCoordPointer(geometry->getTextureElementsCount(i) ,
                          GL_FLOAT,
                          0,
                          geometry->getTextureCoords(i)) ;
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    texturePointer = appearance->getTexture(i);
    activeTexture  = texturePointer && texturePointer->getID();
    if(activeTexture)
      texturePointer->activate(i);
  }
}

void Shape::disableTexureUnits(int format)
{
  Texture *texturePointer;
  bool     activeTexture = false;

  for(int i = Renderer::getMaxAvailableTextureUnits() - 1; i >-1; i--)
  {
    if(!isTextureActive(format, i))  continue;
    if(geometry->getTextureElementsCount(i) > 0){
      glClientActiveTextureARB(GL_TEXTURE0_ARB + i);
      glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    texturePointer = appearance->getTexture(i);
    activeTexture  = texturePointer && texturePointer->getID();

    if(activeTexture)
      texturePointer->deactivate();
  }
}

void Shape::compile(bool shadows)
{
  if(!geometry)
    return;
  if(!(geometry->getVertexFormat() & RESIDE_ON_GFX_DEVICE))
  {
    geometry->compile();
    if(parent && !parent->isOccluder())
      geometry->cleanUpAfterCompilation();
  }
}

void Shape::renderSilhouette(Tuple4f &viewPosition)
{
  ShadowInfo *shadowInfo = geometry->getInfoShadow();
  if(!shadowInfo)
    return;

  shadowInfo->computeSilhouette(viewPosition);
  shadowInfo->drawSilhouette();
}

void Shape::doShadowPass(Tuple4f &lightPos, bool drawCaps, int renderingPath)
{
  ShadowInfo *shadowInfo = geometry->getInfoShadow();

  if(!shadowInfo)
    return;

  shadowInfo->drawDegeneratedQuads(lightPos, drawCaps, renderingPath);
}

void Shape::setGeometry(Geometry *geom)
{
  GeometryInfo *geomtryInfo = NULL;

  if(!geom)
    return;

  if(geometry)
  if(geomtryInfo = GeometryManager::getGeometryInfo(geometry->getName()))
    geomtryInfo->decreaseUserCount();

  if(geomtryInfo = GeometryManager::getGeometryInfo(geom->getName()))
    geomtryInfo->increaseUserCount();
  else
  {
    geomtryInfo = new GeometryInfo(geom->getName(), geom);
    GeometryManager::addGeometryInfo(geomtryInfo);
  }
  geometry = geom;
}

Geometry   *Shape::getGeometry()  { return geometry;   }

void        Shape::setAppearance(Appearance *app){ appearance =  app; }
Appearance *Shape::getAppearance()               { return appearance; }

int  Shape::getPickingIndex() { return pickingIndex; }

void Shape::setVisible(bool v){ visible = v;    }
bool Shape::isVisible()       { return visible; }

bool Shape::loadXMLSettings(XMLElement *element, const char *shapePath)
{
  if(!element || !shapePath)
    return Logger::writeErrorLog("Invalid XMLElement or shapePath -> Shape::loadXMLSettings ");

  XMLElement *geometryNode   = element->getChildByName("Geometry"),
             *appearanceNode = element->getChildByName("Appearance");
   
  if(!geometryNode || !appearanceNode)
    return Logger::writeErrorLog(String("Can't locate <Appearance> or <Geometry> in <Shape> nodel -> ") + shapePath);

  if(!appearance)
    appearance = new Appearance();
  else
    appearance->destroy();

  if(geometry)
    geometry->destroy();
  
  GeometryInfo *geomtryInfo = GeometryManager::getGeometryInfo(shapePath);

  if(!geomtryInfo)
  {
    geometry = new Geometry(shapePath);

    if(geometry->loadXMLSettings(geometryNode))
    {
      geomtryInfo = new GeometryInfo(geometry->getName(), geometry);
      GeometryManager::addGeometryInfo(geomtryInfo);
    }
    else
    {
      deleteObject(geometry);
      return Logger::writeErrorLog(String("Failed to load XML geometry -> ") + shapePath);
    }
  }
  else
    setGeometry(geomtryInfo->getMedia());

  if(!appearance->loadXMLSettings(appearanceNode))
    Logger::writeErrorLog(String("Failed to load XML appearance -> ") + shapePath);
  return true;
}

bool Shape::exportXMLSettings(ofstream &xmlFile  )
{
  if(!xmlFile.is_open())
    return Logger::writeErrorLog("Cannot export Shape to XML: file not ready.");

  if(!geometry)
    return Logger::writeErrorLog("Cannot export Shape to XML: NULL geometry.");

  if(!appearance)
    return Logger::writeErrorLog("Cannot export Shape to XML: NULL appearance.");

  int success = 1;

  xmlFile << "<Shape>\n";
  success *= appearance->exportXMLSettings(xmlFile);
  success *= geometry  ->exportXMLSettings(xmlFile);
  xmlFile << "</Shape>\n";
  return (success != 0);
}

bool Shape::exportXMLSettings(const char* xmlPath)
{
  ofstream xmlFile(xmlPath, ios::app | ios::binary);
  bool result = exportXMLSettings(xmlFile);
  xmlFile.close();
  return result;
}

Shape::~Shape()
{
  if(geometry)
    geometry->destroy();
  deleteObject(appearance);
  parent = NULL;
}