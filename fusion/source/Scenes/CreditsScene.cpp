#include "CreditsScene.h"
#include "../Managers/SceneManager.h"
#include "../Commands/ChangeCommand.h"
#include "../Stage.h"

CreditsScene::CreditsScene(const char* name) : Scene(name)
{
  sceneStart = 0;
}

bool CreditsScene::initialize()
{
  ChangeCommand* cmd = new ChangeCommand();
  cmd->setTarget("MainMenuScene");
  cmd->setSource("return");
  sceneController.addCommand(cmd);
  
  camera.setup(0,0,100, 0,0,0, 0,1,0);
  
  fireStringDB.initialize();
  
  return Scene::initialize();
}

void CreditsScene::beginScene()
{
  Scene::beginScene();
  glClearColor(0,0,0,1);
  
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE);
  
  SceneManager::getStage()->setCursorVisibility(false);
  
  sceneStart = Timer::getCurrentTime();
  fireStringDB.reset();
}

void CreditsScene::update(const FrameInfo &frameInfo)
{
  const FrameInfo* info = &frameInfo;
  camera.update(0);
  frustum.update();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  if (fireStringDB.end())
  {
    sceneController.execute("return");
    return;
  }
  
  fireStringDB.update(info->m_Interval);
  fireStringDB.draw();
}

void CreditsScene::endScene()
{
  SceneManager::getStage()->setCursorVisibility(true);
  glDisable(GL_BLEND);
}

void CreditsScene::handleMouseEvent(MouseEvent& evt, int extraInfo)
{
  Scene::handleMouseEvent(evt, extraInfo);
  
  switch (extraInfo)
  {
    case CLICKED:
      if (2.0f <= Timer::getElapsedTimeSeconds(sceneStart))
        sceneController.execute("return");
      break;
  }
}

CreditsScene::~CreditsScene()
{}

