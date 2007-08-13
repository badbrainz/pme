#ifndef MAINMENUSCENE
#define MAINMENUSCENE

#include "Scene.h"

class MainMenuScene : public Scene
{
  public:
  	MainMenuScene(const String &name = "MainMenuScene");
  	virtual ~MainMenuScene();
  
  public:
     virtual void actionPerformed(GUIEvent &evt);
     virtual void Update(const FrameInfo &frameInfo);
};

#endif