#include "Stage.h"
#include "Configuration.h"

#pragma comment(linker, "/ENTRY:WinMainCRTStartup")
#ifdef _DEBUG
#pragma comment(linker, "/subsystem:console" )
#endif

int  WINAPI WinMain(HINSTANCE   hInstance,
                    HINSTANCE   hPrevInstance,
                    LPSTR       lpCmdLine,
                    int         nCmdShow)
{
  MediaPathManager::registerPath("Data/");

	Configuration config;
  Stage         stage;
  MSG           msg;
  
	if (!config.Initialize())
    return -1;

  if (!stage.Initialize())
    return -1;

  while (stage.IsActive())
  {
    if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
    {
      if (msg.message == WM_QUIT)
        stage.SetActive(false);
      else
      {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    }
    else
      stage.Run();
  }

  return int(msg.wParam);
}
