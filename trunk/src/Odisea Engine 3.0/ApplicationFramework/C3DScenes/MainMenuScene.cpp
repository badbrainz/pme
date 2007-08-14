#include "MainMenuScene.h"

MainMenuScene::MainMenuScene(const String &name) : Scene(name)
{
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
      m_SceneController.Execute(callbackString);
  }
}

void MainMenuScene::Update(const FrameInfo &frameInfo)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  Renderer::enter2DMode(frameInfo.m_Width, frameInfo.m_Height);
    m_Gui.render(frameInfo.m_Interval);
  Renderer::exit2DMode();
}

MainMenuScene::~MainMenuScene()
{
}