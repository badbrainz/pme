#include "NewMapScene.h"

NewMapScene::NewMapScene(const String &name) : Scene(name)
{
}

void NewMapScene::actionPerformed(GUIEvent &evt)
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

NewMapScene::~NewMapScene()
{
}