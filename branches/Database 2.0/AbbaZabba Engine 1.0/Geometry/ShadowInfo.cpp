#include "Geometry.h"
#include "ShadowInfo.h"
#include "../Viewer/Camera.h"
#include "../Renderer/Renderer.h"

ShadowInfo::ShadowInfo(Geometry *geom){
  initialize();
  parent = geom;
}

void ShadowInfo::initialize(){
  degeneratedQuadsInfo   = NULL;
  silhouetteUpdated      = false;
  trianglesCapsInfo      = NULL;
  regularQuadsData       = NULL;
  drawableBufferI        = NULL;
  lightPosition.x        = -2.365234f;
  capsBufferData         = NULL;
  verticesBuffer         = NULL;
  vboIDTriangles         =    0;
  indicesCount           =    0;
  vboIDQuads             =    0;
  edgeCount              =    0;
  index                  =    0;
  parent                 = NULL;
  faces                  = NULL;
  edges                  = NULL;
}

void ShadowInfo::createFaces(){
  if(!parent)
    return;

  if(faces)
    deleteArray(faces);

  if(verticesBuffer)
    deleteArray(verticesBuffer);

  int *indices = parent->getIndices();

  verticesBuffer = new Tuple3f[parent->getVerticesCount()];

  memcpy(verticesBuffer,
         parent->getVertices(),
         parent->getVerticesCount()*sizeof(Tuple3f));
 
  faces  = new Triangle[parent->getTriangleCount()];

  int triangleCount = parent->getTriangleCount();
  
  for(int j=0; j< triangleCount; ++j){
    faces[j].setVertices(&verticesBuffer[indices[j*3+0]],
                         &verticesBuffer[indices[j*3+1]],
                         &verticesBuffer[indices[j*3+2]]);
  }
}

void ShadowInfo::computeSilhouette(Tuple4f &lightSource)
{
  Tuple3f lightSource3f(lightSource.x, lightSource.y, lightSource.z);
  int triangleCount = parent->getTriangleCount();

  if(lightPosition != lightSource ||  lightPosition.x == -2.365234f)
  {
    for(int i = 0; i< triangleCount; ++i)
      faces[i].setFacingPoint(lightSource3f);
    silhouetteUpdated = true;
  }
  else
    silhouetteUpdated = false;

  lightPosition = lightSource;
}

void ShadowInfo::drawSilhouette(){
  if(silhouetteUpdated)
  {
    indicesCount = 0;
    for(int i = 0; i< edgeCount; ++i){
      if(faces[edges[i].triangleIndex[0]].facingLight && 
        !faces[edges[i].triangleIndex[1]].facingLight)
      {

        drawableBufferI[indicesCount + 0] = edges[i].vertexIndex[0];
        drawableBufferI[indicesCount + 1] = edges[i].vertexIndex[1];
        indicesCount +=2;
      }
      else
      {
       if((faces[edges[i].triangleIndex[1]].facingLight && 
          !faces[edges[i].triangleIndex[0]].facingLight))
       {
        drawableBufferI[indicesCount + 0] = edges[i].vertexIndex[1];
        drawableBufferI[indicesCount + 1] = edges[i].vertexIndex[0];
         indicesCount +=2;
       }
     } 

    }
  }
  glVertexPointer(3, GL_FLOAT, 0, verticesBuffer);
  glEnableClientState(GL_VERTEX_ARRAY);
  glDrawElements(GL_LINES, indicesCount, GL_UNSIGNED_INT, drawableBufferI);
  glDisableClientState(GL_VERTEX_ARRAY);
}

void ShadowInfo::drawDegeneratedQuads(Tuple4f& lightLocation,
                                      bool     enableCapsDrawing, 
                                      int      renderingFormat)
{
  if(!(renderingFormat & VOLUME_EXTRUSION_GPU) || 
     !GLEE_ARB_vertex_buffer_object            ||
     !GLEE_ARB_vertex_program)
  {
    computeSilhouette(lightLocation);
    Tuple3f *ptr = NULL;
    if(silhouetteUpdated){
      index = 0;

      for(int i = 0; i< edgeCount; ++i){
        if(faces[edges[i].triangleIndex[0]].facingLight && 
           !faces[edges[i].triangleIndex[1]].facingLight)
        {
          ptr = &verticesBuffer[edges[i].vertexIndex[0]];
          regularQuadsData[index++].set(ptr->x, ptr->y, ptr->z, 1.0f);
          regularQuadsData[index++].set(ptr->x - lightPosition.x,
                                        ptr->y - lightPosition.y,
                                        ptr->z - lightPosition.z,
                                        0.0f);

          ptr = &verticesBuffer[edges[i].vertexIndex[1]];
          regularQuadsData[index++].set(ptr->x - lightPosition.x,
                                        ptr->y - lightPosition.y,
                                        ptr->z - lightPosition.z,
                                        0.0f);
          regularQuadsData[index++].set(ptr->x, ptr->y, ptr->z, 1.0f);
        }
        else{
          if(!faces[edges[i].triangleIndex[0]].facingLight && 
              faces[edges[i].triangleIndex[1]].facingLight)
          {
            ptr = &verticesBuffer[edges[i].vertexIndex[1]];
            regularQuadsData[index++].set(ptr->x, ptr->y, ptr->z, 1.0f);
            regularQuadsData[index++].set(ptr->x - lightPosition.x,
                                          ptr->y - lightPosition.y,
                                          ptr->z - lightPosition.z,
                                          0.0f);

            ptr = &verticesBuffer[edges[i].vertexIndex[0]];
            regularQuadsData[index++].set(ptr->x - lightPosition.x,
                                          ptr->y - lightPosition.y,
                                          ptr->z - lightPosition.z,
                                          0.0f); 
            regularQuadsData[index++].set(ptr->x, ptr->y, ptr->z, 1.0f);
          }
        }
      }
    }
    glVertexPointer(4, GL_FLOAT, 0, regularQuadsData);
    glEnableClientState(GL_VERTEX_ARRAY);
  
    glDrawArrays(GL_QUADS, 0,  index);
    glDisableClientState(GL_VERTEX_ARRAY);  
  }
  else
  {
    lightPosition = lightLocation;
    glProgramEnvParameter4fARB(GL_VERTEX_PROGRAM_ARB, 0, lightPosition.x,  
                                                         lightPosition.y,
                                                         lightPosition.z,
                                                         lightPosition.w);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboIDQuads);

	  glNormalPointer(GL_FLOAT, 24, OFFSET(0));
    glEnableClientState(GL_NORMAL_ARRAY);

    glVertexPointer(3, GL_FLOAT, 24, OFFSET(12));
    glEnableClientState(GL_VERTEX_ARRAY);
  
    glDrawArrays(GL_QUADS, 0,  edgeCount*4);

    glDisableClientState(GL_NORMAL_ARRAY); 
    glDisableClientState(GL_VERTEX_ARRAY);  

    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
  } 

  if(enableCapsDrawing)
    drawCaps(renderingFormat);
}

void ShadowInfo::drawCaps(int renderingFormat)
{
  int triangleCount = parent->getTriangleCount(),
      index         = 0;

  if(!(renderingFormat & VOLUME_EXTRUSION_GPU) || 
     !GLEE_ARB_vertex_buffer_object            ||
     !GLEE_ARB_vertex_program)
  {
    if(silhouetteUpdated){
      for(int i = 0; i< triangleCount; ++i){
        if(faces[i].facingLight){
          for(int j = 0; j < 3; j++){
            capsBufferData[index++].set(faces[i].vertices[j]->x,
                                        faces[i].vertices[j]->y,
                                        faces[i].vertices[j]->z,
                                        1.0f);
          }
        }
        else{
          for(int j = 0; j < 3; j++)
            capsBufferData[index++].set(faces[i].vertices[j]->x - lightPosition.x,
                                        faces[i].vertices[j]->y - lightPosition.y,
                                        faces[i].vertices[j]->z - lightPosition.z,
                                        0.0f); 
        }
      }
    }
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(4, GL_FLOAT, 0, capsBufferData);
    glDrawArrays(GL_TRIANGLES, 0,  triangleCount*3);
    glDisableClientState(GL_VERTEX_ARRAY);
  }
  else{
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboIDTriangles);

	  glNormalPointer(GL_FLOAT, 24, OFFSET(0));
    glEnableClientState(GL_NORMAL_ARRAY);

    glVertexPointer(3, GL_FLOAT, 24, OFFSET(12));
    glEnableClientState(GL_VERTEX_ARRAY);
  
    glDrawArrays(GL_TRIANGLES, 0,  triangleCount*3);

    glDisableClientState(GL_NORMAL_ARRAY); 
    glDisableClientState(GL_VERTEX_ARRAY);  

    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
  }
}

void ShadowInfo::constructDegeneratedQuads()
{
  int  triangleCount = parent->getTriangleCount(),
       elementsSize  = parent->getIndicesCount(),
      *indices       = parent->getIndices();

  if(degeneratedQuadsInfo)
    deleteArray(degeneratedQuadsInfo);

  degeneratedQuadsInfo  = new Tuple3f[edgeCount*8];

  if(trianglesCapsInfo)
    deleteArray(trianglesCapsInfo);

  trianglesCapsInfo  = new Tuple3f[elementsSize*2];

  int triIndex0 = 0,
      triIndex1 = 0;

  for(int i= 0, edgePoint = 0; i< edgeCount; ++i){
    triIndex0 = edges[i].triangleIndex[0];
    triIndex1 = edges[i].triangleIndex[1];

    triIndex0 = triIndex0 == -1 ? edges[i].triangleIndex[1] : triIndex0;
    triIndex1 = triIndex1 == -1 ? edges[i].triangleIndex[0] : triIndex1;

    degeneratedQuadsInfo[edgePoint++] = faces[triIndex0].plane.getNormal();  
    degeneratedQuadsInfo[edgePoint++] = verticesBuffer[edges[i].vertexIndex[0]];

    degeneratedQuadsInfo[edgePoint++] = faces[triIndex1].plane.getNormal();
    degeneratedQuadsInfo[edgePoint++] = verticesBuffer[edges[i].vertexIndex[0]];

    degeneratedQuadsInfo[edgePoint++] = faces[triIndex1].plane.getNormal();   
    degeneratedQuadsInfo[edgePoint++] = verticesBuffer[edges[i].vertexIndex[1]];

    degeneratedQuadsInfo[edgePoint++] = faces[triIndex0].plane.getNormal();  
    degeneratedQuadsInfo[edgePoint++] = verticesBuffer[edges[i].vertexIndex[1]];
  }

  int index = 0;
  for(int i = 0; i < triangleCount; ++i){
    for(int j = 0; j < 3; j++){
      trianglesCapsInfo[index++] = faces[i].plane.getNormal(); 
      trianglesCapsInfo[index++] = *faces[i].vertices[j];
    }
  }

  if(GLEE_ARB_vertex_buffer_object)
    createShadowVolumeVBO();
} 

void ShadowInfo::constructEdges()
{
  createFaces();

  int  triangleCount = parent->getTriangleCount(),
      *indices       = parent->getIndices();

  edges = new Edge[triangleCount * 3];
  edgeCount = 0;

  int  i1        = 0,
       i2        = 0,
       i3        = 0;
  Edge *edge =  edges;

  for(int a = 0; a < triangleCount; a++)
  {
    i1 = indices[a*3+0];
    i2 = indices[a*3+1];
    i3 = indices[a*3+2];

    if(i1 < i2)
    {
      edge->vertexIndex[0] = i1;
      edge->vertexIndex[1] = i2;
      edge->triangleIndex[0] = a;
      edge->triangleIndex[1] = -1;
      edgeCount++;
      edge++;
    }

    if(i2 < i3)
    {
      edge->vertexIndex[0] = i2;
      edge->vertexIndex[1] = i3;
      edge->triangleIndex[0] = a;
      edge->triangleIndex[1] = -1;
      edgeCount++;
      edge++;
    }

    if(i3 < i1)
    {
      edge->vertexIndex[0] = i3;
      edge->vertexIndex[1] = i1;
      edge->triangleIndex[0] = a;
      edge->triangleIndex[1] = -1;
      edgeCount++;
      edge++;
    }
  }

  for(int a = 0; a < triangleCount; a++)
  {
    i1 = indices[a*3+0];
    i2 = indices[a*3+1];
    i3 = indices[a*3+2];

    if(i1 > i2)
    {
      edge = edges;
      for(int b = 0; b < edgeCount; b++)
      {
        if((edge->vertexIndex[0]   == i2) &&
           (edge->vertexIndex[1]   == i1) &&
           (edge->triangleIndex[1] == -1))
        {
          edge->triangleIndex[1] = a;
          break;
        }
        edge++;
      }
    }

    if(i2 > i3)
    {
      edge = edges;
      for (long b = 0; b < edgeCount; b++)
      {
        if((edge->vertexIndex[0] == i3) &&
           (edge->vertexIndex[1] == i2) &&
           (edge->triangleIndex[1] == -1))
           {
             edge->triangleIndex[1] = a;
             break;
           }
        edge++;
      }
    }

    if(i3 > i1)
    {
      edge =  edges;
      for(int b = 0; b < edgeCount; b++)
      {
        if((edge->vertexIndex[0] == i1) &&
           (edge->vertexIndex[1] == i3) &&
           (edge->triangleIndex[1] == -1))
        {
          edge->triangleIndex[1] = a;
          break;
        }
        edge++;
      }
    }
  }

  if(drawableBufferI)
    deleteArray(drawableBufferI);

  drawableBufferI = new int[edgeCount*8];

  if(regularQuadsData)
    deleteArray(regularQuadsData);
 
  regularQuadsData  = new Tuple4f[edgeCount*4]; 

  if(capsBufferData)
    deleteArray(capsBufferData);

  capsBufferData    = new Tuple4f[triangleCount*3];
  constructDegeneratedQuads();
} 

void ShadowInfo::cleanUpAfterVBOCreation(){
  deleteArray(degeneratedQuadsInfo);
  deleteArray(trianglesCapsInfo);
}

bool ShadowInfo::createShadowVolumeVBO()
{
  float *GPUInterleavedArray = NULL;
  glGenBuffersARB(1, &vboIDQuads);
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboIDQuads);
  glBufferDataARB(GL_ARRAY_BUFFER_ARB, edgeCount * 4 * 2 * 3 * sizeof(float),
                  degeneratedQuadsInfo, GL_STATIC_DRAW_ARB);
 
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

  glGenBuffersARB(1, &vboIDTriangles);
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboIDTriangles);
  glBufferDataARB(GL_ARRAY_BUFFER_ARB, parent->getTriangleCount() * 3 * 2 * 3 * sizeof(float),
                  trianglesCapsInfo, GL_STATIC_DRAW_ARB);
 
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
  cleanUpAfterVBOCreation();
  return true;
}

void ShadowInfo::setLightPosition(Tuple4f &lightSource)
{
  lightPosition = lightSource;
}

ShadowInfo::~ShadowInfo()
{
  deleteArray(degeneratedQuadsInfo);
  deleteArray(trianglesCapsInfo);
  deleteArray(regularQuadsData);
  deleteArray(drawableBufferI);
  deleteArray(verticesBuffer);
  deleteArray(capsBufferData);
  deleteArray(edges);
  deleteArray(faces);
}
