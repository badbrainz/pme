#include "ARenderer.h"
#include "../Extensions/GLee.h"
//#include <d3dx9.h>

int  ARenderer::retrieveRenderStateGL(CDubyte rState)
{
  switch(rState) 
  {
    case RenderState::FOG: return GL_FOG;
    case RenderState::LIGHTING: return GL_LIGHTING;
    case RenderState::TEXTURE_1D: return GL_TEXTURE_1D;
    case RenderState::TEXTURE_2D: return GL_TEXTURE_2D;
    case RenderState::TEXTURE_3D: return GL_TEXTURE_3D;
    case RenderState::TEXTURE_CUBE_MAP: return GL_TEXTURE_CUBE_MAP;
    case RenderState::CULL_FACE: return GL_CULL_FACE;
    case RenderState::ALPHA_TEST: return GL_ALPHA_TEST;
    case RenderState::BLEND: return GL_BLEND;
    case RenderState::DITHER: return GL_DITHER;
    case RenderState::STENCIL_TEST: return GL_STENCIL_TEST;
    case RenderState::DEPTH_TEST: return GL_DEPTH_TEST;
    case RenderState::CLIP_PLANE0: return GL_CLIP_PLANE0;
    case RenderState::CLIP_PLANE1: return GL_CLIP_PLANE1;
    case RenderState::CLIP_PLANE2: return GL_CLIP_PLANE2;
    case RenderState::CLIP_PLANE3: return GL_CLIP_PLANE3;
    case RenderState::CLIP_PLANE4: return GL_CLIP_PLANE4;
    case RenderState::CLIP_PLANE5: return GL_CLIP_PLANE5;
    case RenderState::LIGHT0: return GL_LIGHT0;
    case RenderState::LIGHT1: return GL_LIGHT1;
    case RenderState::LIGHT2: return GL_LIGHT2;
    case RenderState::LIGHT3: return GL_LIGHT3;
    case RenderState::LIGHT4: return GL_LIGHT4;
    case RenderState::LIGHT5: return GL_LIGHT5;
    case RenderState::LIGHT6: return GL_LIGHT6;
    case RenderState::LIGHT7: return GL_LIGHT7;
    case RenderState::POINT_SMOOTH: return GL_POINT_SMOOTH;
    case RenderState::LINE_SMOOTH: return GL_LINE_SMOOTH;
    case RenderState::POLYGON_SMOOTH: return GL_POLYGON_SMOOTH;
    case RenderState::SCISSOR_TEST: return GL_SCISSOR_TEST;
    case RenderState::NORMALIZE: return GL_NORMALIZE;
    case RenderState::POLYGON_OFFSET_POINT: return GL_POLYGON_OFFSET_POINT;
    case RenderState::POLYGON_OFFSET_LINE: return GL_POLYGON_OFFSET_LINE;
    case RenderState::POLYGON_OFFSET_FILL: return GL_POLYGON_OFFSET_FILL;
    //case RenderState::MULTISAMPLE: return D3DRS_MULTISAMPLEANTIALIAS;
  }
  return -1;
}

int  ARenderer::retrieveRenderStateD3D(CDubyte rState)
{
      /*    D3DRS_FILLMODE                  = 8,    
    D3DRS_SHADEMODE                 = 9,  
                  = 14,   
               = 15,   
      D3DRS_SRCBLEND                  = 19,   
    D3DRS_DESTBLEND                 = 20,   

    D3DRS_CULLMODE                  = 22,     
    D3DRS_ZFUNC                     = 23,   
    D3DRS_ALPHAREF                  = 24,   
    D3DRS_ALPHAFUNC                 = 25,   
                  = 26,   
    D3DRS_ALPHABLENDENABLE          = 27,    
    D3DRS_SPECULARENABLE            = 29,    
    D3DRS_FOGCOLOR                  = 34,   
    D3DRS_FOGTABLEMODE              = 35,   
    D3DRS_FOGSTART                  = 36,    
    D3DRS_FOGEND                    = 37,   
    D3DRS_FOGDENSITY                = 38,    
    D3DRS_RANGEFOGENABLE            = 48,   
  
     D3DRS_STENCILFAIL               = 53,  
    D3DRS_STENCILZFAIL              = 54,    
    D3DRS_STENCILPASS               = 55,    
    D3DRS_STENCILFUNC               = 56,    
    D3DRS_STENCILREF                = 57,    
    D3DRS_STENCILMASK               = 58,    
    D3DRS_STENCILWRITEMASK          = 59,    
    
   
    D3DRS_TWOSIDEDSTENCILMODE       = 185,    
    D3DRS_CCW_STENCILFAIL           = 186,   
    D3DRS_CCW_STENCILZFAIL          = 187,  
    D3DRS_CCW_STENCILPASS           = 188,    
    D3DRS_CCW_STENCILFUNC           = 189,  
*/
     
  //switch(rState) 
  //{
  //  /*case RenderState::FOG: return D3DRS_FOGENABLE;
  //  case RenderState::LIGHTING: return D3DRS_LIGHTING;  
  //  case RenderState::CULL_FACE: return GL_CULL_FACE;
  //  case RenderState::ALPHA_TEST: return D3DRS_ALPHATESTENABLE; 
  //  case RenderState::DITHER: return D3DRS_DITHERENABLE;
  //  case RenderState::STENCIL_TEST: return D3DRS_STENCILENABLE;
  //  case RenderState::DEPTH_TEST: return D3DRS_ZWRITEENABLE;
  //  case RenderState::LINE_SMOOTH: return D3DRS_ANTIALIASEDLINEENABLE; 
  //  case RenderState::SCISSOR_TEST: return D3DRS_SCISSORTESTENABLE;
  //  case RenderState::NORMALIZE: return  D3DRS_NORMALIZENORMALS; */
  //}
  return -1;
}

bool ARenderer::isInitialized()
{
  return initialized;
}