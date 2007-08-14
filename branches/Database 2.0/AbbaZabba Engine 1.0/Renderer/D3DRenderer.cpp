//#include "D3DRenderer.h"
//#include "../Window/Window.h"
//#include "../Extensions/GLee.h"
//
//D3DRenderer *D3DRenderer::defaultD3DRenderer = NULL;
//
//D3DRenderer::D3DRenderer()
//{
//  initialized = false;
//  deviceD3D  = NULL;
//  objectD3D  = NULL;
//
//  ZeroMemory(&capsD3D, sizeof(D3DCAPS9));
//}
//
//bool D3DRenderer::initialize(const Window *window)
//{
//  if(!window)
//    return Logger::writeErrorLog("NULL window arg, D3DRenderer::initialize");
//
//  if(!window->getHandle())
//    return Logger::writeErrorLog("NULL window handle, D3DRenderer::initialize");
//
//  if(initialized)
//    return Logger::writeErrorLog("D3DRenderer already initialized, D3DRenderer::initialize");
//
//  D3DPRESENT_PARAMETERS parameters;
//  D3DDISPLAYMODE        displayMode;
//  DWORD                 verticesPCaps = 0;
//
//  ZeroMemory(&parameters, sizeof(parameters));
//
//  if(!(objectD3D = Direct3DCreate9(D3D_SDK_VERSION)))
//    return Logger::writeErrorLog("Failed to create a D3D Object, D3DRenderer::initialize");
//
//  if(FAILED(objectD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode)))
//    return Logger::writeErrorLog("Failed to retrieve the D3D dislay mode, D3DRenderer::initialize");
//
//  if(FAILED(objectD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &capsD3D)))
//    return Logger::writeErrorLog("Failed to retrieve the D3D hardware caps, D3DRenderer::initialize");
//
//  if(capsD3D.VertexProcessingCaps != 0)
//    verticesPCaps |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
//  else
//    verticesPCaps |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
//
//  if(window->isFullscreen())
//  {
//    parameters.BackBufferHeight = window->getHeight();
//    parameters.BackBufferWidth  = window->getWidth();
//    parameters.Windowed         = false;
//  }
//  else
//    parameters.Windowed = true;
//
//  parameters.EnableAutoDepthStencil = true;
//  parameters.AutoDepthStencilFormat = D3DFMT_D24S8;
//  parameters.BackBufferFormat       = displayMode.Format;
//  parameters.BackBufferCount        = 1;
//  parameters.SwapEffect             = D3DSWAPEFFECT_DISCARD;
//  parameters.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;
//
//  bufferSize.set(window->getWidth(), window->getHeight());
//
//  if(FAILED(objectD3D->CreateDevice(D3DADAPTER_DEFAULT,
//                                    D3DDEVTYPE_HAL,
//                                    window->getHandle(),
//                                    verticesPCaps,
//                                   &parameters,
//                                   &deviceD3D)))
//    return Logger::writeErrorLog("Failed to create a D3D device, D3DRenderer::initialize");
//
//  if(!deviceD3D)
//    return Logger::writeErrorLog("NULL D3D device, D3DRenderer::initialize");
//
//  return (initialized = true);
//}
//
//bool D3DRenderer::enable(CDubyte rState)
//{
//  int renderState = ARenderer::retrieveRenderStateD3D(rState);
//
//  if(renderState != -1)
//  {
//    glEnable(renderState);
//    return true;
//  }
//  return false;
//}
//
//bool D3DRenderer::disable(CDubyte rState)
//{
//  int renderState = ARenderer::retrieveRenderStateD3D(rState);
//
//  if(renderState != -1)
//  {
//    glDisable(renderState);
//    return true;
//  }
//  return false;
//}
//
//bool D3DRenderer::isEnabled(CDubyte rState)
//{
//  int renderState = ARenderer::retrieveRenderStateD3D(rState);
//
//  if(renderState != -1)
//  {
//    glIsEnabled(renderState);
//    return true;
//  }
//  return false;
//
//  return false;
//}
//
//bool D3DRenderer::blendFunc(CDubyte sFunc, CDubyte dFunc, CDubyte blendOp)
//{
//  /*switch(rState)
//  {
//  }*/
//
//  return false;
//}
//
//
//bool D3DRenderer::alphaFunc(CDubyte function, float reference)
//{
// /* switch(rState)
//  {
//  }
//*/
//  return false;
//}
//
//
//bool D3DRenderer::depthFunc(CDubyte func)
//{
///*  switch(rState)
//  {
//  }*/
//
//  return false;
//}
//
//const Tuple2i &D3DRenderer::getBufferSize() const
//{
//  return bufferSize;
//}
//
//LPDIRECT3DDEVICE9 D3DRenderer::getDefaultDevice()
//{
//  LPDIRECT3DDEVICE9 dDevice = getDefaultRenderer()->getD3DDevice();
//
//  if(!dDevice)
//   Logger::writeFatalErrorLog("NULL default device, D3DRenderer::getDefaultDevice");
//
//  return dDevice;
//}
//
//LPDIRECT3D9    D3DRenderer::getDefaultObject()
//{
//  LPDIRECT3D9 dObject = getDefaultRenderer()->getD3DObject();
//
//  if(!dObject)
//   Logger::writeFatalErrorLog("NULL default object, D3DRenderer::getDefaultDevice");
//
//  return dObject;
//}
//
//const D3DCAPS9 &D3DRenderer::getDefaultCaps()
//{
//  D3DRenderer *dRenderer = getDefaultRenderer();
//  return dRenderer->getD3DCaps();
//}
//
//void  D3DRenderer::setDefaultRenderer(D3DRenderer *dRenderer)
//{
//  defaultD3DRenderer = dRenderer;
//}
//
//D3DRenderer *D3DRenderer::getDefaultRenderer()
//{
//  if(!defaultD3DRenderer)
//   Logger::writeFatalErrorLog("Invalid default device, D3DRenderer::getDefaultDevice()");
//
//  return defaultD3DRenderer;
//}
//
//LPDIRECT3DDEVICE9  D3DRenderer::getD3DDevice()
//{
//  return deviceD3D;
//}
//
//LPDIRECT3D9      D3DRenderer::getD3DObject()
//{
//  return objectD3D;
//}
//
//const D3DCAPS9    &D3DRenderer::getD3DCaps() const
//{
//  return capsD3D;
//}
//
//Tuple3f D3DRenderer::getCameraPosition()
//{
//  static  Matrix4f viewMatrix;
//
//  Tuple3f cameraPosition;
//
//  if(deviceD3D)
//  {
//    deviceD3D->GetTransform(D3DTS_VIEW, (D3DXMATRIXA16*)&viewMatrix);
//    viewMatrix.setInverse();
//    cameraPosition.set(viewMatrix[12], viewMatrix[13], viewMatrix[14]);
//  }
//
//  return cameraPosition;
//}
//
//D3DRenderer::~D3DRenderer()
//{
//  SAFE_RELEASE(deviceD3D);
//  SAFE_RELEASE(objectD3D);
//}
