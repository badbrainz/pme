#include "SSWindow.h"
#include "../Managers/ManagersUtils.h"
#include "../Tools/Timer.h"
#include "../Renderer/Renderer.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

SSWindow::SSWindow()
{
  renderingContext    = NULL;
  deviceContext       = NULL;
  windowHandle        = NULL;
}

bool SSWindow::createWindow(HWND wHandle,int bpp, int alpha,
                            int depth   , int stencil)
{
  Timer::intialize();
  windowHandle = wHandle;
  GLuint    PixelFormat;

  static  PIXELFORMATDESCRIPTOR pfd =               // pfd Tells Windows How We Want Things To Be
  {
    sizeof(PIXELFORMATDESCRIPTOR),                  // Size Of This Pixel Format Descriptor
    1,                                              // Version Number
    PFD_DRAW_TO_WINDOW |                            // Format Must Support Window
    PFD_SUPPORT_OPENGL |                            // Format Must Support OpenGL
    PFD_DOUBLEBUFFER,                               // Must Support Double Buffering
    PFD_TYPE_RGBA,                                  // Request An RGBA Format
    bpp,                                            // Select Our Color Depth
    0, 0, 0, 0, 0, 0,                               // Color Bits Ignored
    alpha,                                          // Alpha Buffer
    0,                                              // Shift Bit Ignored
    0,                                              // Accumulation Buffer
    0, 0, 0, 0,                                     // Accumulation Bits Ignored
    depth,                                          // Z-Buffer (Depth Buffer)
    stencil,                                        // Stencil Buffer bits
    0,                                              // No Auxiliary Buffer
    PFD_MAIN_PLANE,                                 // Main Drawing Layer
    0,                                              // Reserved
    0, 0, 0                                         // Layer Masks Ignored
  };

  deviceContext =  GetDC(wHandle);

  if(!deviceContext)                                // Did We Get A Device Context?
  {
    destroy();                                      // Reset The Display
    Logger::writeFatalErrorLog("Can't Create A GL Device Context");
    return false;                                   // Return FALSE
  }

  if (!(PixelFormat=ChoosePixelFormat(deviceContext,&pfd)))   // Did Windows Find A Matching Pixel Format?
  {
    destroy();                                                // Reset The Display
    Logger::writeFatalErrorLog("Can't Find A Suitable PixelFormat");
    return false;                                             // Return FALSE
  }

  if(!SetPixelFormat(deviceContext,PixelFormat,&pfd))         // Are We Able To Set The Pixel Format?
  {
    destroy();                                                // Reset The Display
    Logger::writeFatalErrorLog("Can't Set The PixelFormat");
    return false;                                             // Return FALSE
  }

  if(!(renderingContext = wglCreateContext(deviceContext)))   // Are We Able To Get A Rendering Context?
  {
    destroy();                                          // Reset The Display
    Logger::writeFatalErrorLog("Can't Create A GL Rendering Context");
    return false;                                             // Return FALSE
  }

  if(!wglMakeCurrent(deviceContext, renderingContext))        // Try To Activate The Rendering Context
  {
    destroy();
    Logger::writeFatalErrorLog("Can't Activate The GL Rendering Context");
    return false;                              // Return FALSE
  }

  makeCurrent();
  return true;
}


HGLRC SSWindow::getRenderingContext(){ return renderingContext; }
HDC   SSWindow::getDeviceContext()   { return deviceContext;    }
HWND  SSWindow::getHandle()          { return windowHandle;     }

void SSWindow::makeCurrent()
{
  wglMakeCurrent(deviceContext, renderingContext);
}

void SSWindow::update()
{
  SwapBuffers(deviceContext);
}

void     SSWindow::setDimensions(const Tuple2i &dimension_){ dimensions = dimension_; }
const    Tuple2i &SSWindow::getDimensions() const          { return dimensions;       }
const    int SSWindow::getHeight() const                   { return dimensions.y;     }
const    int SSWindow::getWidth()  const                   { return dimensions.x;     }

void SSWindow::destroy()
{
  wglMakeCurrent(NULL, NULL);

  if(renderingContext)
  {
    if(!wglDeleteContext(renderingContext))  // Are We Able To Delete The RC?
      Logger::writeErrorLog("Release rendering context failed");

    renderingContext = NULL;                 // Set RC To NULL
  }

  if(deviceContext && !ReleaseDC(windowHandle, deviceContext))
  { 
    Logger::writeErrorLog("Release device context failed");
    deviceContext = NULL;                                       // Set DC To NULL
  }
}

SSWindow::~SSWindow()
{
  destroy();
}