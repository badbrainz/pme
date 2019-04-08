#ifndef CREDITSSCENE
#define CREDITSSCENE

#include "Scene.h"
#include "../Tools/FireTextUtils.h"
#include "../Renderer/Renderer.h"

class CreditsScene : public Scene
{
  public:
    CreditsScene(const char* name = "CreditsScene");
    ~CreditsScene();
    
  public:
    bool initialize();
    
    void beginScene();
    void update(const FrameInfo &frameInfo);
    void endScene();
    
    void handleMouseEvent(MouseEvent& evt, int extraInfo);

  private:
    
  private:
    __int64 sceneStart;
    Frustum frustum;
    Camera camera;
    FireStringDB fireStringDB;
};

#endif
