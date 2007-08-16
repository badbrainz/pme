#include "Scene.h"

Scene::Scene(const String &name) : IOXMLObject(name)
{
  m_bInitialized = false;
}

bool Scene::Initialize()
{
	if (!IOXMLObject::loadXMLSettings(String(ioname) + ".xml"))
    return Logger::writeErrorLog(String("could not initialize ") + ioname);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glDepthMask(GL_TRUE);
  
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  
  glClearColor(0,0,0,0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  
	m_Gui.setGUIEventListener(this);
	m_Gui.setVisible(true);
  m_bInitialized = true;
	
	return true;
}

bool Scene::loadXMLSettings(XMLElement *element)
{
	if (!isSuitable(element))
    return false;
	
  XMLElement *child = 0;
	bool success = false;
	
  if (child = element->getChildByName("Controller"))
		m_SceneController.loadXMLSettings(child);

	if (child = element->getChildByName("Panel"))
		success = m_Gui.loadXMLSettings(child);
	
	return success;
}

void Scene::BeginScene()
{
	if (!m_bInitialized)
    Initialize();
}

void Scene::PauseScene()
{
}

void Scene::EndScene()
{
}

void Scene::Update(const FrameInfo &frameInfo)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  Renderer::enter2DMode(frameInfo.m_Width, frameInfo.m_Height);
    m_Gui.render(frameInfo.m_Interval);
  Renderer::exit2DMode();
}

void Scene::HandleWindowEvent(WindowEvent evt, int extraInfo)
{
  SetPerspective(evt, true);
}

void Scene::HandleMouseEvent(MouseEvent evt, int extraInfo)
{
  m_Gui.checkMouseEvents(evt, extraInfo);
}

void Scene::HandleKeyEvent(KeyEvent evt, int extraInfo)
{
  m_Gui.checkKeyboardEvents(evt, extraInfo);
}

String& Scene::GetSceneName()
{
	return ioname;
}

void Scene::SetPerspective(WindowEvent evt, bool wEvt)
{
	int width  = evt.getWidth();
	int height = evt.getHeight();

	glViewport(0, 0, width, height);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0f, (float)width/height, 0.1f, 1500.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

	if (wEvt)
	{
		m_Gui.setPosition(0,0);
		m_Gui.setDimensions(float(width), float(height));
		m_Gui.forceUpdate(true);
	}
}

void Scene::Destroy()
{
  m_Gui.clear();
}

Scene::~Scene()
{
  Destroy();
}
