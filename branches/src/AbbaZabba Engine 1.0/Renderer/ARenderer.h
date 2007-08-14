#ifndef ABSTRACT_RENDERER_H
#define ABSTRACT_RENDERER_H

#include "../Math/MathUtils.h"

#ifndef byte
 typedef unsigned char CDubyte;
 typedef char          CDbyte;
#endif 

class Window;
namespace RenderState
{
  enum 
  {
    FOG = 1,
    LIGHTING,
    TEXTURE_1D,
    TEXTURE_2D,
    TEXTURE_3D,
    TEXTURE_CUBE_MAP,
    CULL_FACE,
    ALPHA_TEST,
    BLEND,
    DITHER,
    STENCIL_TEST,
    DEPTH_TEST,
    CLIP_PLANE0,
    CLIP_PLANE1,
    CLIP_PLANE2,
    CLIP_PLANE3,
    CLIP_PLANE4,
    CLIP_PLANE5,
    LIGHT0,
    LIGHT1,
    LIGHT2,
    LIGHT3,
    LIGHT4,
    LIGHT5,
    LIGHT6,
    LIGHT7,
    POINT_SMOOTH,
    LINE_SMOOTH,
    POLYGON_SMOOTH,
    SCISSOR_TEST,
    NORMALIZE,
    POLYGON_OFFSET_POINT,
    POLYGON_OFFSET_LINE,
    POLYGON_OFFSET_FILL,
    MULTISAMPLE
  };
}

namespace Blend
{
  enum
  {
    ZERO,
    ONE,
    SRC_COLOR,
    ONE_MINUS_SRC_COLOR,
    DST_COLOR,
    ONE_MINUS_DST_COLOR,
    SRC_ALPHA,
    ONE_MINUS_SRC_ALPHA,
    DST_ALPHA,
    ONE_MINUS_DST_ALPHA,
    SRC_ALPHA_SATURATE
  };
}

namespace BlendOp
{
  enum
  {
    ADD,
    SUBTRACT,
    REVSUBTRACT,
    MIN,
    MAX
  };
}

namespace CompareFunc
{
  enum
  {
    NEVER,
    LESS,
    EQUAL,
    LEQUAL,
    GREATER,
    NOTEQUAL,
    GEQUAL,
    ALWAYS
  };
}

class ARenderer
{
  public:

    virtual bool initialize(const Window *winPtr = NULL) = 0;
    virtual bool isInitialized();

    static  int  retrieveRenderStateD3D(CDubyte rState);
    static  int  retrieveRenderStateGL (CDubyte rState);

    virtual bool enable(CDubyte rState) = 0;
    virtual bool disable(CDubyte rState) = 0; 
    virtual bool isEnabled(CDubyte state) = 0;

    virtual bool blendFunc(CDubyte sFunc, CDubyte dFunc, CDubyte blendOp = BlendOp::ADD) = 0;
    virtual bool alphaFunc(CDubyte function, float reference) = 0;
    virtual bool depthFunc(CDubyte func) = 0;
  protected:
    bool initialized;

};
#endif