#ifndef NEWMAPSCENE
#define NEWMAPSCENE

#include "Scene.h"

class NewMapScene : public Scene
{
  public:
  	NewMapScene(const String &name = "NewMapScene");
  	virtual ~NewMapScene();
  
  public:
     void actionPerformed(GUIEvent &evt);
};

#endif