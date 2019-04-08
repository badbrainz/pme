#include "MainMenuScene.h"
#include "../Renderer/Renderer.h"
#include "../Kernel/Gateway.h"
#include "../Managers/ManagersUtils.h"
#include "../Stage.h"

MainMenuScene::MainMenuScene(const char* name) : Scene(name)
{
}

void MainMenuScene::beginScene()
{
  Scene::beginScene();
  
  glClearColor(0,0,0,1);
  
  const char* musicPath = Gateway::getMenuMusicPath();
  if (musicPath && Gateway::isMenuMusicEnabled())
    SoundManager::playSong(musicPath);
    
  Gateway::bindBackground();
  SceneManager::getStage()->setCursor("FlechaMenu.tga");
}

void MainMenuScene::endScene()
{
}

void MainMenuScene::update(const FrameInfo &frameInfo)
{
  const FrameInfo* info = &frameInfo;
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  Renderer::enter2DMode(info->m_Width, info->m_Height);
  Gateway::bindBackground();
  glEnable(GL_TEXTURE_2D);
  glColor3f(1,1,1);
  drawFullScreenQuad(info->m_Width, info->m_Height);
  glDisable(GL_TEXTURE_2D);
  gui.render(info->m_Interval);
  Renderer::exit2DMode();
}

void MainMenuScene::actionPerformed(GUIEvent &evt)
{
  const String &callbackString  = evt.getCallbackString();
  GUIRectangle *sourceRectangle = evt.getEventSource();
  int           widgetType      = sourceRectangle->getWidgetType();
  
  if (widgetType == BUTTON)
  {
    GUIButton *button = (GUIButton*) sourceRectangle;
    
    if (button->isClicked())
      sceneController.execute(callbackString);
  }
}

void MainMenuScene::drawFullScreenQuad(int width, int height)
{
  glBegin(GL_TRIANGLE_STRIP);
  glTexCoord2f(0.0f, 1.0f);
  glVertex2i(0, 0);
  glTexCoord2f(0.0f, 0.0f);
  glVertex2i(0, height);
  glTexCoord2f(1.0f, 1.0f);
  glVertex2i(width, 0);
  glTexCoord2f(1.0f, 0.0f);
  glVertex2i(width, height);
  glEnd();
}

MainMenuScene::~MainMenuScene()
{}
