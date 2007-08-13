#ifndef GL_RENDERER_H
#define GL_RENDERER_H

#include "ARenderer.h"

class GLRenderer : public ARenderer
{
  public:

    GLRenderer();
   ~GLRenderer();

    virtual bool initialize(const Window *winPtr = NULL);
    virtual bool isInitialized();

    virtual bool enable(CDubyte rState); 
    virtual bool disable(CDubyte rState); 
    virtual bool isEnabled(CDubyte state);

    virtual bool blendFunc(CDubyte sFunc, CDubyte dFunc, CDubyte blendOp = BlendOp::ADD);
    virtual bool alphaFunc(CDubyte function, float reference);
    virtual bool depthFunc(CDubyte func); 
  private:
   
};
#endif