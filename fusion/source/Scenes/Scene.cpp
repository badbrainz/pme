#include "Scene.h"
#include "../Renderer/Renderer.h"

Scene::Scene(const char* name) : IOXMLObject(name)
{
  initialized = false;
  viewAngle = 90.0f;
}

bool Scene::initialize()
{
  if (!IOXMLObject::loadXMLFile(String(name) + ".xml"))
    return Logger::writeErrorLog(String("Could not initialize ") + name);
    
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  
  gui.setGUIEventListener(this);
  gui.setVisible(true);
  
  initialized = true;
  
  return true;
}

bool Scene::loadXMLSettings(XMLElement *element)
{
  if (!isSuitable(element))
    return false;
    
  XMLElement *child;
  
  bool success = false;
  
  if (child = element->getChildByName("Controller"))
    sceneController.loadXMLSettings(child);
    
  if (child = element->getChildByName("Panel"))
    success = gui.loadXMLSettings(child);
    
  return success;
}

void Scene::beginScene()
{
  if (!initialized)
    initialize();
}

void Scene::pauseScene()
{
}

void Scene::endScene()
{
}

void Scene::update(const FrameInfo &frameInfo)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  Renderer::enter2DMode(frameInfo.m_Width, frameInfo.m_Height);
  gui.render(frameInfo.m_Interval);
  Renderer::exit2DMode();
}

void Scene::handleWindowEvent(WindowEvent evt, int extraInfo)
{
  setPerspective(evt, true);
}

void Scene::handleMouseEvent(MouseEvent& evt, int extraInfo)
{
  gui.checkMouseEvents(evt, extraInfo);
}

void Scene::handleKeyEvent(KeyEvent evt, int extraInfo)
{
  gui.checkKeyboardEvents(evt, extraInfo);
}

void Scene::actionPerformed(GUIEvent &evt)
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

void Scene::setPerspective(WindowEvent evt, bool wEvt)
{
  int width  = evt.getWidth();
  int height = evt.getHeight();
  
  glViewport(0, 0, width, height);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(viewAngle, (float)width/height, 0.1f, 1000.0f);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  if (wEvt)
  {
    gui.setPosition(0, 0);
    gui.setDimensions(float(width), float(height));
    gui.forceUpdate(true);
  }
}

bool Scene::isInitialized()
{
  return initialized;
}

bool Scene::exportXMLSettings(ofstream &xmlFile)
{
  return true;
}

void Scene::setViewAngle(float angle)
{
  viewAngle = angle;
}

void Scene::destroy()
{
  gui.clear();
}

Scene::~Scene()
{
  destroy();
}
