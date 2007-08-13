#ifndef SHADOWINFO_H
#define SHADOWINFO_H

#include "../Math/MathUtils.h"
#include "../Tools/Logger.h"
#include "Triangle.h"

#ifndef OFFSET
#define OFFSET(x) ((char *)NULL+(x))
#endif

#ifndef RENDERING_GENERAL_ATTRIBUTES
#define RENDERING_GENERAL_ATTRIBUTES

#define TEXTURE0                       0x00000001
#define TEXTURE1                       0x00000002
#define TEXTURE2                       0x00000004
#define TEXTURE3                       0x00000008
#define TEXTURE4                       0x00000010
#define TEXTURE5                       0x00000020
#define TEXTURE6                       0x00000040
#define TEXTURE7                       0x00000080

#define COLOR                          0x00000100
#define NORMALS                        0x00000200
#define VERTICES                       0x00000400
#define INDICES                        0x00000800

#define MATERIAL			                 0x00001000
#define SHADERS                        0x00002000

#define BLEND_ATTR                     0x00004000
#define ALPHA_ATTR                     0x00008000

#define FRONT_TO_BACK                  0x00010000
#define BACK_TO_FRONT                  0x00020000

#define ALL_TEXTURES                   TEXTURE0 | TEXTURE1 | TEXTURE2 | TEXTURE3 | \
                                       TEXTURE4 | TEXTURE5 | TEXTURE6 | TEXTURE7

#define ALL_EFFECTS                    COLOR        | MATERIAL   | \
                                       ALL_TEXTURES | SHADERS    | \
                                       BLEND_ATTR   | ALPHA_ATTR

#define CULL_FRUSTUM                   0x00004000
#define CULL_OCCLUDERS                 0x00008000
#define VOLUME_EXTRUSION_CPU           0x00010000
#define VOLUME_EXTRUSION_GPU           0x00020000
#define RESIDE_ON_GFX_DEVICE           0x00040000
#define FATAL_ERROR                    0x00100000

#endif

class Geometry;
class ShadowInfo
{
  private:
    struct Edge
    {
      int vertexIndex[2],
          triangleIndex[2];
    };

    GLuint      vboIDQuads,
                vboIDTriangles;

    int        *drawableBufferI,
                indicesCount,
                edgeCount,
                index;
    Edge       *edges;

    bool       silhouetteUpdated;
    Tuple3f   *verticesBuffer,

              *trianglesCapsInfo,
               *degeneratedQuadsInfo;
    Tuple4f    lightPosition,
              *capsBufferData,
              *regularQuadsData;

    Triangle *faces;
    Geometry *parent;

    void initialize();
    void createFaces();

  public:
    ShadowInfo(Geometry *geom);

    void cleanUpAfterVBOCreation();
    bool createShadowVolumeVBO();

    void computeSilhouette(Tuple4f &lightSource);
    void drawSilhouette();
    void drawCaps( int renderingFormat);

    void drawDegeneratedQuads(Tuple4f& lightPosition,
                              bool     drawCaps,
                              int      renderingFormat);
    void constructDegeneratedQuads();
    void setLightPosition(Tuple4f &lightSource);
    void constructEdges();

    Geometry *getParent(){
      return parent;
    }

    int  getEdgeCount(){ return edgeCount; }
   ~ShadowInfo();
};

#endif