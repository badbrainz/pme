#ifndef D3D_RENDERER_H
#define D3D_RENDERER_H
//
//#include "ARenderer.h"
////#include<d3dx9.h>
//
//#ifndef SAFE_RELEASE
//  #define SAFE_RELEASE(x){ if(x) x->Release(); x = NULL; }
//#endif 
//
//class Window;
//class D3DRenderer : public ARenderer
//{
//  public:
//    D3DRenderer();
//   ~D3DRenderer();
//
//    virtual bool initialize(const Window *winPtr = NULL);
// 
//    virtual bool enable(CDubyte rState);
//    virtual bool disable(CDubyte rState);
//    virtual bool isEnabled(CDubyte state);
//
//    virtual bool blendFunc(CDubyte sFunc, CDubyte dFunc, CDubyte blendOp = BlendOp::ADD);
//    virtual bool alphaFunc(CDubyte function, float reference);
//    virtual bool depthFunc(CDubyte func);
//
//    Tuple3f getCameraPosition();
//    static  LPDIRECT3DDEVICE9   getDefaultDevice();
//    static  LPDIRECT3D9         getDefaultObject();
//    static  const D3DCAPS9     &getDefaultCaps();
//
//    static void         setDefaultRenderer(D3DRenderer *dRenderer);
//    static D3DRenderer *getDefaultRenderer();
//
//    LPDIRECT3DDEVICE9   getD3DDevice();
//    LPDIRECT3D9         getD3DObject();
//    const D3DCAPS9     &getD3DCaps()    const;
//    const Tuple2i      &getBufferSize() const;
//
//  private:
//    LPDIRECT3DDEVICE9   deviceD3D;
//    LPDIRECT3D9         objectD3D;
//    D3DCAPS9            capsD3D;
//    Tuple2i             bufferSize;
//    static D3DRenderer *defaultD3DRenderer;
//};
#endif