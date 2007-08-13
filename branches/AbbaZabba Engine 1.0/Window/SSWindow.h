#ifndef SSWINDOW_H
#define SSWINDOW_H

#include "../Tools/Logger.h"
#include "../GUI/GUIFrame.h"
#include "../Events/InputEventListener.h"

#include <cstdlib>
#include <ctime>

class SSWindow
{

  public:
    SSWindow();
   ~SSWindow();

    HGLRC    getRenderingContext();
    HWND     getHandle();
    HDC      getDeviceContext();

    void     destroy();
    void     update();


    bool     createWindow(HWND wHandle,
                          int bpp  ,
                          int alpha,
                          int depth, 
                          int stencil);

    void     makeCurrent();

    void     setDimensions(const Tuple2i &dimension);
    const    Tuple2i &getDimensions() const;
    const    int getHeight() const;
    const    int getWidth()  const;

   private:
    Tuple2i  dimensions; 
    HGLRC    renderingContext; // Permanent Rendering Context
    HWND     windowHandle ;    // Holds Our Window Handle
    HDC      deviceContext;    // Private GDI Device Context
};

#endif
