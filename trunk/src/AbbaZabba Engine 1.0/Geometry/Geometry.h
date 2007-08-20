#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "BoundsDescriptor.h"
 
class ShadowInfo;
class Geometry  : public NamedObject, public IOXMLObject
{
  private:
    int     texElementsCount[MAX_TEX_UNITS],
            VERTEX_FORMAT,  
            triangleCount,
            verticesCount,
            indicesCount,
            elementsVBOID,
            indicesVBOID,
            strideSize,  
           *indices,
            type;

    bool    infoShadowActive,
            byReference[12];

    float  *textureCoords[MAX_TEX_UNITS],
           *vertices,  
           *normals,
           *colors;

    BoundsDescriptor boundsDescriptor;
    ShadowInfo*      infoShadow;
    void             initialize();
    int              getXMLType(const String &);

  public:
    Geometry(const Geometry &geom);
    Geometry(const char* name, int indicesCount = 0, int verticesCount = 0);
    Geometry &operator =(const Geometry &geom);
    
    virtual bool loadXMLSettings(XMLElement *element);
    virtual bool exportXMLSettings(ofstream &xmlFile  );
   
    bool   destroy();

    BoundsDescriptor &getBoundsDescriptor();
    void   setBoundsDescriptor(BoundsDescriptor &descriptor);

    bool   compile();

    void   setVertexFormat(int format);
    int    getVertexFormat();

    void   setTextureElements(int, float*, int, bool);
    void   setTextureElementsCount(int, int);
    int    getTextureElementsCount(int index);

    void   setElementsVBOID(int id);
    int    getElementsVBOID();

    void   setIndicesVBOID(int id);
    int    getIndicesVBOID();

    bool   setType(int type = GL_TRIANGLES);
    int    getType();

    void   setStrideSize(int i);
    int    getStrideSize();

    void   setIndicesCount(int count);
    void   setIndices(int*, bool);
    int*   getIndices();
    int    getIndicesCount(); 

    void   setVerticesCount(int count);
    void   setVertices(float *vertices, bool byReference);
    float* getVertices();
    int    getVerticesCount();

    void   setNormals(float* normals, bool byReference);
    float* getNormals();

    void   setColors(float* colors, bool byReference);
    float* getColors();

    ShadowInfo *getInfoShadow();
    bool        activeInfoShadow();
    void        deleteBuffers();

    void   overrideByReferenceFlag(int element, bool val);
    bool   isByReference(int element);

    void   cleanUpAfterCompilation();
  
    float* getTextureCoords(int index);
    int    getTriangleCount();
    int    getOffset(int geomElement);
};
#endif