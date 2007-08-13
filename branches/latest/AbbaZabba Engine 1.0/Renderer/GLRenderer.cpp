#include "GLRenderer.h"
#include "../Extensions/GLee.h"

bool GLRenderer::enable(CDubyte rState)
{
  int renderState = ARenderer::retrieveRenderStateGL(rState);

  if(renderState != -1)
  {
    glEnable(renderState);
    return true;
  }
  return false;
}

bool GLRenderer::disable(CDubyte rState)
{
  int renderState = ARenderer::retrieveRenderStateGL(rState);

  if(renderState != -1)
  {
    glDisable(renderState);
    return true;
  }
  return false;
}

bool GLRenderer::isEnabled(CDubyte rState)
{
  int renderState = ARenderer::retrieveRenderStateGL(rState);

  if(renderState != -1)
  {
    glIsEnabled(renderState);
    return true;
  }
  return false;

  return false;
}

bool GLRenderer::blendFunc(CDubyte sFunc, CDubyte dFunc, CDubyte blendOp)
{
  /*switch(rState)
  {
  }*/

  return false;
}


bool GLRenderer::alphaFunc(CDubyte function, float reference)
{
 /* switch(rState)
  {
  }
*/
  return false;
}


bool GLRenderer::depthFunc(CDubyte func)
{
/*  switch(rState)
  {
  }*/

  return false;
}
