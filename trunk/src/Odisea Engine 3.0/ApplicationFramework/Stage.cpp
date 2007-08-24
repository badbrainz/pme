#include "Stage.h"
#include "FrameInfo.h"

#include "C3DScenes/Scene.h"
#include "C3DScenes/NewMapScene.h"
#include "C3DScenes/EditorScene.h"
#include "C3DScenes/MainMenuScene.h"
#include "C3DManagers/SceneManager.h"

#include "C3DDatabase/TerrainDatabase.h"
TerrainDatabase terrainDatabase;

Stage::Stage(HINSTANCE instance) : IOXMLObject("Stage")
{
  Logger::initialize();

  m_Window.setInputEventListener(this);
  m_Window.setAppInstance(instance);
  m_Benchmark.setEnabled(false);
  m_bActiveState = true;

  SceneManager::SetStage(this);
}

bool Stage::Initialize()
{
  if (!IOXMLObject::loadXMLSettings(String(ioname + ".xml")))
    return Logger::writeErrorLog(String("could not initialize ") + ioname);
  
  srand((unsigned)time(0));
  
  EditorScene* editorScene = new EditorScene("EditorScene");
  NewMapScene* newmapScene = new NewMapScene("NewMapScene");
  editorScene->SetTerrainDatabase(&terrainDatabase);
  newmapScene->SetTerrainDatabase(&terrainDatabase);
  
  SceneManager::AddScene(editorScene);
  SceneManager::AddScene(newmapScene);
  SceneManager::AddScene(new MainMenuScene("MainMenuScene"));
  SceneManager::SetScene("MainMenuScene");
  
  m_Window.setVisible(true);
  m_Window.update();
  
  Logger::flush();
  
  return true;
}

bool Stage::loadXMLSettings(XMLElement *element)
{
  if (!isSuitable(element))
    return false;

  XMLElement *child   = 0;
  bool				success = false;
  
  if (child = element->getChildByName("Benchmark"))
    m_Benchmark.loadXMLSettings(child);
  
  if (child = element->getChildByName("Window"))
    success = m_Window.loadXMLSettings(child);
  
  return success;
}

bool Stage::Run()
{
  float frameInterval = m_Benchmark.getFrameInterval();

  m_Benchmark.markFrameStart();
    FrameInfo frameInfo(frameInterval, m_Benchmark.getFPS(), m_Window.getWidth(), m_Window.getHeight());
    SceneManager::UpdateScene(frameInfo);
    m_Window.update(frameInterval);
  m_Benchmark.markFrameEnd();

  m_bActiveState = m_Benchmark.running();

  return m_bActiveState;
}

void Stage::SetActive(bool state)
{
  m_bActiveState = state;
}

bool Stage::IsActive()
{
  return m_bActiveState;
}

void Stage::ForceUpdate()
{
  SceneManager::DispatchEvent(WindowEvent(0,0, m_Window.getWidth(),m_Window.getHeight()), 1);
}

///mouse event handling
void Stage::mouseReleased(MouseEvent evt)
{ 
  SceneManager::DispatchEvent(evt, RELEASED); 
}

void Stage::mouseClicked(MouseEvent evt)
{ 
  SceneManager::DispatchEvent(evt, CLICKED); 
}

void Stage::mouseMoved(MouseEvent evt)
{ 
  SceneManager::DispatchEvent(evt, MOVED); 
}

void Stage::mouseDragged(MouseEvent evt)
{ 
  SceneManager::DispatchEvent(evt, DRAGGED); 
}

///key event handling
void Stage::keyReleased(KeyEvent evt)
{
  SceneManager::DispatchEvent(evt, KEY_RELEASED);
  
  switch (evt.getKeyID())
  {
    case KEY_F8:
      m_Window.scheduleSnapshot();
    break;
  }
}

void Stage::keyPressed(KeyEvent evt)
{
  SceneManager::DispatchEvent(evt, KEY_PRESSED);
  
  /*switch (evt.getKeyID())
  {
    case KEY_ESCAPE:
      m_bActiveState = false;
    break;
  }*/
}

///window event handling
void Stage::windowPositionChanged(WindowEvent evt)
{
  SceneManager::DispatchEvent(evt, 1);
}

void Stage::windowSizeChanged(WindowEvent evt)
{
  SceneManager::DispatchEvent(evt, 1);
}

void Stage::windowMaximized(WindowEvent evt)
{
  SceneManager::DispatchEvent(evt, 1);
}

void Stage::windowMinimized(WindowEvent evt)
{
  SceneManager::DispatchEvent(evt, 1);
}

void Stage::windowClosing(WindowEvent evt)
{
}

///end
void Stage::Destroy()
{
  if (m_Benchmark.isEnabled())
    m_Benchmark.exportResults();
  
  TexturesManager::flushAllTextures();
  SceneManager::DestroyAllScenes();

  if(m_Window.isFullscreen())
  {
    Logger::writeInfoLog("Restoring task bar");
    HWND taskBarHandle = FindWindow("Shell_traywnd", "");
    SetWindowPos(taskBarHandle, 0, 0, 0, 0, 0, SWP_SHOWWINDOW);
  }
}

Stage::~Stage()
{
  Destroy();
  Logger::flush();
}