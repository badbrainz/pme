#include "Geometry.h"
#include "ShadowInfo.h"
#include "../Managers/GeometryManager.h"

Geometry::Geometry(const char* name_, int indicesC, int verticesC) : IOXMLObject("Geometry")
{
  initialize();
  verticesCount = verticesC;
  indicesCount  = indicesC;
  name          = name_;
}

Geometry::Geometry(const Geometry &geom)  : IOXMLObject("Geometry")
{
  if(this != &geom)
    initialize();

  this->operator =(geom);
}

Geometry &Geometry::operator =(const Geometry &geom) 
{
  if(this != &geom)
  {
    memcpy(byReference, geom.byReference, 12);

    VERTEX_FORMAT     = geom.VERTEX_FORMAT;
    strideSize        = geom.strideSize;

    infoShadowActive  = geom.infoShadowActive;
    elementsVBOID     = geom.elementsVBOID;
    indicesVBOID      = geom.indicesVBOID;
    verticesCount     = geom.verticesCount;
    indicesCount      = geom.indicesCount;
    name              = geom.name;
    type              = geom.type;

    for(int i = 0; i < 8; i++){
      byReference[i] = false;
      setTextureElements(i, geom.textureCoords[i], geom.texElementsCount[i], false);
    }

    setVertices(geom.vertices, false);
    setNormals (geom.normals,  false);
    setIndices (geom.indices,  false);
    setColors  (geom.colors,   false);

    deleteObject(infoShadow);
    if(geom.infoShadow)
      activeInfoShadow();
  }
  return *this;
}

BoundsDescriptor &Geometry::getBoundsDescriptor()
{
  return boundsDescriptor;
}

void   Geometry::setBoundsDescriptor(BoundsDescriptor &descriptor)
{
  boundsDescriptor = descriptor;
}

ShadowInfo *Geometry::getInfoShadow(){ return infoShadow; }

void   Geometry::setVertexFormat(int format){ VERTEX_FORMAT = format; }
int    Geometry::getVertexFormat()          { return VERTEX_FORMAT;   }

void   Geometry::setVerticesCount(int count){ verticesCount = count; }
int    Geometry::getVerticesCount()         { return verticesCount;  }

void   Geometry::setIndicesCount(int count){ indicesCount = count; }
int    Geometry::getIndicesCount()         { return indicesCount;  }

void   Geometry::setStrideSize(int i){ strideSize = i;    }
int    Geometry::getStrideSize()     { return strideSize; }

bool   Geometry::setType(int geomType)
{
  switch(geomType)
  {
    case GL_TRIANGLE_STRIP:
    case GL_QUAD_STRIP:
    case GL_TRIANGLES:
    case GL_POLYGON:
    case GL_POINTS:
    case GL_QUADS:
    case GL_LINES:
      type = geomType;
    return true;
  }
  return false;
}

int    Geometry::getType()            { return type;     }

void   Geometry::setElementsVBOID(int id){ elementsVBOID  = id;  }
int    Geometry::getElementsVBOID()      { return elementsVBOID; }

void   Geometry::setIndicesVBOID(int id){ indicesVBOID  = id;  }
int    Geometry::getIndicesVBOID()      { return indicesVBOID; }

float* Geometry::getVertices()   { return vertices;          }
float* Geometry::getNormals()    { return normals;           }
float* Geometry::getColors()     { return colors;            }
int*   Geometry::getIndices()    { return indices;           }

int Geometry::getOffset(int geomElement)
{
  int offset = 0;

  switch(geomElement)
  {
    case TEXTURE0: offset  = 0;
    break;
    case TEXTURE1: offset  = texElementsCount[0]*4;
    break;
    case TEXTURE2: offset  = texElementsCount[1]*4 + getOffset(TEXTURE1);
    break;
    case TEXTURE3: offset  = texElementsCount[2]*4 + getOffset(TEXTURE2);
    break;
    case TEXTURE4: offset  = texElementsCount[3]*4 + getOffset(TEXTURE3);
    break;
    case TEXTURE5: offset  = texElementsCount[4]*4 + getOffset(TEXTURE4);
    break;
    case TEXTURE6: offset  = texElementsCount[5]*4 + getOffset(TEXTURE5);
    break;
    case TEXTURE7: offset  = texElementsCount[6]*4 + getOffset(TEXTURE6);
    break;
    case COLOR:   offset   = texElementsCount[7]*4 + getOffset(TEXTURE7);
    break;
    case NORMALS:  offset  = ((VERTEX_FORMAT & COLOR) != 0)*12 + getOffset(COLOR);
    break;
    case VERTICES: offset  = ((VERTEX_FORMAT & NORMALS) != 0)*12 + getOffset(NORMALS);
    break;
  }
  return offset;
}

int  Geometry::getTriangleCount()
{
  triangleCount = (type == GL_TRIANGLE_STRIP) ? indicesCount - 2:
                  (type == GL_TRIANGLE_FAN  ) ? indicesCount - 2:
                  (type == GL_TRIANGLES     ) ? indicesCount/3  :
                  (type == GL_QUADS         ) ? indicesCount/2  : 0;
  return triangleCount;
}

bool Geometry::activeInfoShadow()
{
  if(infoShadow)
    return true;

  if(!indicesCount   ||
     !verticesCount  ||
     !indices        ||
     type != GL_TRIANGLES)
    return Logger::writeErrorLog("Couldn't activate the shadow info");

  infoShadow = new ShadowInfo(this);
  infoShadow->constructEdges();
  return true;
}

void Geometry::initialize(){
  memset(byReference, false, 12);

  for(int i = 0; i < 8; i++)
  {
    texElementsCount[i] = 0;
    textureCoords[i]     = NULL;
  }

  VERTEX_FORMAT    = VERTICES;
  infoShadow       = NULL;
  vertices         = NULL;
  normals          = NULL;
  indices          = NULL;
  colors           = NULL;

  elementsVBOID    = 0;
  indicesVBOID     = 0;
  verticesCount    = 0;
  indicesCount     = 0;
  strideSize       = 0;
  type             = GL_TRIANGLES;
}

void Geometry::setTextureElementsCount(int index, int elementSize){
  if(index < 0 || index > MAX_TEX_UNITS)
    return;
  texElementsCount[index]       = elementSize;
}

int Geometry::getTextureElementsCount(int index)
{
  return  index >= 0 && index < MAX_TEX_UNITS ? texElementsCount[index]: 0;
}

float* Geometry::getTextureCoords(int index)
{
  return index < 0 || index > MAX_TEX_UNITS ? NULL : textureCoords[index];
}

void Geometry::setTextureElements(int index      , float* pointer,
                                  int elementSize, bool byRef)
{

  if(index < 0 || index > MAX_TEX_UNITS) return;
  if(!pointer)  return;

  if(!byReference[index])
    deleteArray(textureCoords[index]);

  VERTEX_FORMAT          |= (TEXTURE0 <<index);
  byReference[index]      = byRef;
  texElementsCount[index] = elementSize;

  if(byRef)
    textureCoords[index] = pointer;
  else{
    textureCoords[index]     = new float[verticesCount*elementSize];
    memcpy(textureCoords[index], pointer, verticesCount*sizeof(float)*elementSize);
  }
}

void Geometry::setColors(float* crs, bool byRef){
  if(!crs)            return;
  if(!byReference[8]) deleteArray(colors);

  byReference[8] = byRef;
  VERTEX_FORMAT |= COLOR;

  if(byReference[8])
    colors = crs;
  else{
    colors = new float[verticesCount*3];
    memcpy(colors, crs, verticesCount*sizeof(float)*3);
  }
}

void Geometry::setNormals(float* normalsCoords, bool byRef)
{
  if(!normalsCoords)  return;
  if(!byReference[9]) deleteArray(normals);

  byReference[9] = byRef;
  VERTEX_FORMAT |= NORMALS;

  if(byRef)
    normals = normalsCoords;
  else{
    normals = new float[verticesCount*3];
    memcpy(normals, normalsCoords, verticesCount*sizeof(float)*3);
  }
}

void Geometry::setVertices(float* verticesCoords, bool byRef)
{
  if(!verticesCoords)
    return;

  if(!byReference[10]) deleteArray(vertices);

  byReference[10] = byRef;

  if(byRef)
    vertices = verticesCoords;
  else{
    vertices = new float[verticesCount*3];
    memcpy(vertices, verticesCoords, verticesCount*sizeof(float)*3);
  }

   float minX =  1000000.0f,
         minY =  1000000.0f,
         minZ =  1000000.0f,
         maxX = -1000000.0f,
         maxY = -1000000.0f,
         maxZ = -1000000.0f;

  Tuple3f* indexedVectors = (Tuple3f*)vertices,
         * temp           = NULL;

  //if(computeBounds)
  {
    for(int i = 0; i < verticesCount; i++)
    {
      temp = &indexedVectors[i];
      if(temp->x < minX) minX = temp->x;
      if(temp->x > maxX) maxX = temp->x;

      if(temp->y < minY) minY = temp->y;
      if(temp->y > maxY) maxY = temp->y;

      if(temp->z < minZ) minZ = temp->z;
      if(temp->z > maxZ) maxZ = temp->z;
    }
    boundsDescriptor.computeBounds(minX, minY, minZ, maxX, maxY, maxZ);
  }
}

void Geometry::setIndices(int * ind, bool byRef){
  if(!ind)
   return;

  if(!byReference[11]) deleteArray(indices);

  byReference[11] = byRef;

  if(byRef)
    indices = ind;
  else{
    indices   = new int[indicesCount];
    memcpy(indices, ind, indicesCount*sizeof(int));
  }
}

void Geometry::overrideByReferenceFlag(int element, bool val)
{
  if(element & TEXTURE0) byReference[0] = val;
  if(element & TEXTURE1) byReference[1] = val;
  if(element & TEXTURE2) byReference[2] = val;
  if(element & TEXTURE3) byReference[3] = val;
  if(element & TEXTURE4) byReference[4] = val;
  if(element & TEXTURE5) byReference[5] = val;
  if(element & TEXTURE6) byReference[6] = val;
  if(element & TEXTURE6) byReference[7] = val;

  if(element & COLOR    ) byReference[ 8] = val;
  if(element & NORMALS  ) byReference[ 9] = val;
  if(element & VERTICES ) byReference[10] = val;
  if(element & INDICES  ) byReference[11] = val;
}

bool Geometry::isByReference(int element)
{
  switch(element)
  {
    case TEXTURE0: return byReference[ 0]; break;
    case TEXTURE1: return byReference[ 1]; break;
    case TEXTURE2: return byReference[ 2]; break;
    case TEXTURE3: return byReference[ 3]; break;
    case TEXTURE4: return byReference[ 4]; break;
    case TEXTURE5: return byReference[ 5]; break;
    case TEXTURE6: return byReference[ 6]; break;
    case TEXTURE7: return byReference[ 7]; break;
    case COLOR:    return byReference[ 8]; break;
    case NORMALS:  return byReference[ 9]; break;
    case VERTICES: return byReference[10]; break;
    case INDICES:  return byReference[11]; break;
  }
  return false;
}

void Geometry::cleanUpAfterCompilation()
{
  if(!byReference[8])  deleteArray(colors);
  if(!byReference[9])  deleteArray(normals);
  if(!byReference[10]) deleteArray(vertices);
  if(!byReference[11]) deleteArray(indices);

  for(int i = 0; i < 8; i++)
    if(!byReference[i])
      deleteArray(textureCoords[i]);

  memset(byReference, false, 12);
  Logger::writeInfoLog(String("Cleaning up geometry after compilation -> ") + name);
}

bool Geometry::compile()
{
  if(VERTEX_FORMAT & RESIDE_ON_GFX_DEVICE)
    return true;

  Logger::writeInfoLog(String("Compiling geometry -> ") + name);

  if(!GLEE_ARB_vertex_buffer_object)
    return Logger::writeErrorLog("Failed: VBO extension not supported");

  if(!getVertices())
    return Logger::writeErrorLog("Failed: NULL vertices");

  float      *GPUInterleavedArray   = NULL;
  int         interleavedBufferSize = 0,
             *GPUIndices            = NULL,
              supported             = 0;

  GLuint      vboID                 =    0,
              indicesVBOID          =    0;

   for(int i = 0; i < 8; i++)
   {
     supported = VERTEX_FORMAT & (TEXTURE0 << i);
     if(supported)
       interleavedBufferSize += getTextureElementsCount(i);
     else
       break;
  }

  if((VERTEX_FORMAT & VERTICES) && getVertices()) interleavedBufferSize += 3;
  if((VERTEX_FORMAT & NORMALS ) && getNormals() ) interleavedBufferSize += 3;
  if((VERTEX_FORMAT & COLOR   ) && getColors()  ) interleavedBufferSize += 3;

  setStrideSize(interleavedBufferSize*4);

  interleavedBufferSize *= getVerticesCount();

  glGenBuffersARB(1, &vboID);
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboID);
  glBufferDataARB(GL_ARRAY_BUFFER_ARB, interleavedBufferSize * sizeof(float),
                  NULL, GL_STATIC_DRAW_ARB);


  if(glGetError() == GL_NO_ERROR)
    GPUInterleavedArray = (float *)glMapBufferARB(GL_ARRAY_BUFFER_ARB, GL_WRITE_ONLY_ARB);
  else
   return Logger::writeErrorLog("Not enough memory for the interleaved geometry arrays");

  float  *texCoorArray  = NULL,
         *vertexArray   = getVertices(),
         *normalArray   = getNormals(),
         *colorArray    = getColors();
  int     texElemSize   = 0,
          s;

  for(int i = 0, t = 0; i < interleavedBufferSize; t++)
  {
    for(s = 0; s < 8; s++)
    {
      texElemSize  = getTextureElementsCount(s);
      texCoorArray = getTextureCoords(s);

      if(VERTEX_FORMAT & (TEXTURE0 << s))
      {
        switch(texElemSize)
        {

          case 1: GPUInterleavedArray[i++] = texCoorArray[t*1 + 0]; break;
          case 2: GPUInterleavedArray[i++] = texCoorArray[t*2 + 0];
                  GPUInterleavedArray[i++] = texCoorArray[t*2 + 1]; break;
          case 3: GPUInterleavedArray[i++] = texCoorArray[t*3 + 0];
                  GPUInterleavedArray[i++] = texCoorArray[t*3 + 1];
                  GPUInterleavedArray[i++] = texCoorArray[t*3 + 2]; break;
        }
      }
      else
        break;
    }

    if((VERTEX_FORMAT & COLOR) && colorArray)
    {
      GPUInterleavedArray[i++] = colorArray[t*3 + 0];
      GPUInterleavedArray[i++] = colorArray[t*3 + 1];
      GPUInterleavedArray[i++] = colorArray[t*3 + 2];
    }

    if((VERTEX_FORMAT & NORMALS) && normalArray)
    {
      GPUInterleavedArray[i++] = normalArray[t*3 + 0];
      GPUInterleavedArray[i++] = normalArray[t*3 + 1];
      GPUInterleavedArray[i++] = normalArray[t*3 + 2];
    }

    if((VERTEX_FORMAT & VERTICES) && vertexArray)
    {
      GPUInterleavedArray[i++] = vertexArray[t*3 + 0];
      GPUInterleavedArray[i++] = vertexArray[t*3 + 1];
      GPUInterleavedArray[i++] = vertexArray[t*3 + 2];
    }
  }

	glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

  if(indices)
  {
    glGenBuffersARB(1, &indicesVBOID);
    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, indicesVBOID);
    glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, getIndicesCount()*sizeof(int),
                    getIndices(), GL_STATIC_DRAW_ARB);

    if(glGetError() != GL_NO_ERROR)
    {
      glDeleteBuffersARB(1, &indicesVBOID);
      indicesVBOID = 0;
      return Logger::writeErrorLog("Error creating index buffer");
    }

    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
    setIndicesVBOID(indicesVBOID);
  }

  setElementsVBOID(vboID);

  VERTEX_FORMAT |= RESIDE_ON_GFX_DEVICE;

  return true;
}

bool Geometry::loadXMLSettings(XMLElement *element)
{
  XMLElement *node       = NULL,
             *child      = NULL,
             *subChild   = NULL;

  if(!isSuitable(element))
    return false;

  if(child = element->getChildByName("description"))
    return IOXMLObject::loadXMLSettings(child->getValuec());

  destroy();

  for(size_t i = 0; i < element->getChildrenCount(); i++)
  {
    node = element->getChild(i);
    if(!node) continue;
    const String &name = node->getName();

    if(name == "indicesCount")
    {
      setIndicesCount(node->getValuei());
      continue;
    }

    if(name == "type")
    {
      type = getXMLType(node->getValue());
      if(type == -1)
      {
        destroy();
        return Logger::writeErrorLog(String("Unknown Geometry type -> ") + node->getValue());
      }
      continue;
    }

    if(name == "TexCoords")
    {
      int id           = 0,
          size         = 0;
      float coeffX     = 1.0f,
            coeffY     = 1.0f;

      if(!(child = node->getChildByName("id")))
      {
        Logger::writeErrorLog("XML node <Texture> must have an <id> child");
        return NULL;
      }

      id = child->getValuei();

      if(!(child = node->getChildByName("size")))
      {
        Logger::writeErrorLog("XML node <Texture> must have a <size> child");
        return NULL;
      }

      size = child->getValuei();

      if(child = node->getChildByName("coeffX"))
        coeffX = clamp(child->getValuef(), 0.1f, 20.0f);

      if(child = node->getChildByName("coeffY"))
        coeffY = clamp(child->getValuef(), 0.1f, 20.0f);

      if(!(child = node->getChildByName("RawData")))
      {
        Logger::writeErrorLog("XML node <Texture> must have a <RawData> child");
        return NULL;
      }

      child->rawData.skipDelete = true;
      setVerticesCount(child->rawData.byteCount/(size*sizeof(float)));

      if(coeffX != 1.0f || coeffY != 1.0f)
      {
        float *texCoords  = (float*)child->rawData.data;
        int    floatCount =  child->rawData.byteCount/sizeof(float);
        for(int t = 0; t < floatCount; t+=2)
        {
          texCoords[t    ] *=  coeffX;
          texCoords[t + 1] *=  coeffY;
        }
      }

      setTextureElements(id, (float*)child->rawData.data, size, true);
      overrideByReferenceFlag((0x1 << id), false);
      continue;
    }

    if(name == "Vertices")
    {
      if(!(child = node->getChildByName("RawData")))
      {
        Logger::writeErrorLog("XML node <Vertices> must have a <RawData> child");
        return NULL;
      }
      child->rawData.skipDelete = true;
      setVerticesCount(child->rawData.byteCount/(3*sizeof(float)));
      setVertices((float*)child->rawData.data, true);
      overrideByReferenceFlag(VERTICES, false);
      continue;
    }

    if(name == "Normals")
    {
      if(!(child = node->getChildByName("RawData")))
      {
        Logger::writeErrorLog("XML node <Normals> must have a <RawData> child");
        return NULL;
      }
      child->rawData.skipDelete = true;
      setVerticesCount(child->rawData.byteCount/(3*sizeof(float)));
      setNormals((float*)child->rawData.data, true);
      overrideByReferenceFlag(NORMALS, false);
      continue;
    }

    if(name == "Indices")
    {
      if(!(child = node->getChildByName("RawData")))
      {
        Logger::writeErrorLog("XML node <Indices> must have a <RawData> child");
        return NULL;
      }
      child->rawData.skipDelete = true;
      setIndicesCount(child->rawData.byteCount/sizeof(int));
      setIndices((int*)child->rawData.data, true);
      overrideByReferenceFlag(INDICES, false);
      continue;
    }
  }
  return true;
}

bool Geometry::exportXMLSettings(ofstream &xmlFile)
{
  if(!xmlFile.is_open())
    return Logger::writeErrorLog("Cannot export Geometry to XML: file not ready.");

  if(VERTEX_FORMAT & RESIDE_ON_GFX_DEVICE)
    return Logger::writeErrorLog("Cannot export Geometry to XML: compiled.");

  char *rawData = NULL;
  int   count   = 0;

  xmlFile << "<Geometry type         = \"";

  switch(type)
  {
    case GL_TRIANGLE_STRIP: xmlFile << "TRIANGLE_STRIP\"\n"; break;
    case GL_QUAD_STRIP:     xmlFile << "QUAD_STRIP\"\n";     break;
    case GL_TRIANGLES:      xmlFile << "TRIANGLES\"\n";      break;
    case GL_POLYGON:        xmlFile << "POLYGON\"\n";        break;
    case GL_POINTS:         xmlFile << "POINTS\"\n";         break;
    case GL_QUADS:          xmlFile << "QUADS\"\n";          break;
    case GL_LINES:          xmlFile << "LINES\"\n";          break;
    default:
     return Logger::writeErrorLog("Cannot export Geometry to XML: unknown type.");
  }

  xmlFile << "          indicesCount = \"" << getIndicesCount() << "\">\n";

  for(int i = 0; i < 10; i++)
  {
    rawData = (char*)getTextureCoords(i);
    if(!rawData)
      break;

    count = getTextureElementsCount(i) *
            getVerticesCount()         *
            sizeof(float);

    xmlFile << "  <TexCoords id   = \"" << i << "\"\n";
    xmlFile << "             size = \"" << getTextureElementsCount(i) << "\">\n";
    xmlFile << "    <RawData length = \"" << count << "\">";
    xmlFile.write(rawData, count);
    xmlFile << "    </RawData>\n";
    xmlFile << "  </TexCoords>\n";
  }

  rawData = (char*)getVertices();

  if(rawData)
  {
    count =  getVerticesCount() *  sizeof(float) * 3;
    xmlFile << "  <Vertices>\n";
    xmlFile << "    <RawData length = \"" << count << "\">";
    xmlFile.write(rawData, count);
    xmlFile << "    </RawData>\n";
    xmlFile << "  </Vertices>\n";
  }

  rawData = (char*)getIndices();

  if(rawData)
  {
    count =  getIndicesCount() *  sizeof(int);
    xmlFile << "  <Indices>\n";
    xmlFile << "    <RawData length = \"" << count << "\">";
    xmlFile.write(rawData, count);
    xmlFile << "    </RawData>\n";
    xmlFile << "  </Indices>\n";
  }

  rawData = (char*)getNormals();

  if(rawData)
  {
    count =  getVerticesCount() *  sizeof(float) * 3;
    xmlFile << "  <Normals>\n";
    xmlFile << "    <RawData length = \"" << count << "\">";
    xmlFile.write(rawData, count);
    xmlFile << "    </RawData>\n";
    xmlFile << "  </Normals>\n";
  }
  xmlFile << "</Geometry>\n";
  return true;
}

int Geometry::getXMLType(const String &value)
{
  if(value.getLength())
  {
    if(value == "TRIANGLE_STRIP") return GL_TRIANGLE_STRIP;
    if(value == "QUAD_STRIP")     return GL_QUAD_STRIP;
    if(value == "TRIANGLES")      return GL_TRIANGLES;
    if(value == "POLYGON")        return GL_POLYGON;
    if(value == "POINTS")         return GL_POINTS;
    if(value == "QUADS")          return GL_QUADS;
    if(value == "LINES")          return GL_LINES;
  }
  return -1;
}

bool Geometry::destroy()
{
  GeometryInfo *geomtryInfo = GeometryManager::getGeometryInfo(name);

  if(geomtryInfo)
  {
    geomtryInfo->decreaseUserCount();
    if(geomtryInfo->getUserCount() > 0)
      return false;
  }

  if(!byReference[8])  deleteArray(colors);
  if(!byReference[9])  deleteArray(normals);

  if(!byReference[10]) deleteArray(vertices);
  if(!byReference[11]) deleteArray(indices);

  for(int i = 0; i < MAX_TEX_UNITS; i++)
    if(!byReference[i])
      deleteArray(textureCoords[i]);

  deleteObject(infoShadow);

  if(indicesVBOID)
  {
    GLuint del   = indicesVBOID;
    indicesVBOID = 0;
    glDeleteBuffersARB(1, &del);
  }

  if(elementsVBOID)
  {
    GLuint del    = elementsVBOID;
    elementsVBOID = 0;
    glDeleteBuffersARB(1, &del);
  }
  GeometryManager::flushUnusedGeometry();
  return true;
}