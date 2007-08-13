#include "SceneManager.h"
#include "../C3DScenes/Scene.h"
#include "../Stage.h"

SceneInfoList SceneManager::m_SceneInfoList;
Scene* 				SceneManager::m_pCurrentScene = 0;
Stage* 				SceneManager::m_pCurrentStage = 0;

///
SceneInfo::SceneInfo(Scene *scene) : m_pScene(scene)
{}

SceneInfo::SceneInfo(const SceneInfo &copy)
{
	this->operator = (copy);
}

SceneInfo &SceneInfo::operator = (const SceneInfo &right)
{
	if (this != &right)
		m_pScene = right.m_pScene;
	
  return *this;
}

Scene* SceneInfo::GetScenePointer()
{
	return m_pScene;
}

///
void SceneManager::SetStage(Stage *stage)
{
	m_pCurrentStage = stage;
}

void SceneManager::DeactivateCurrentStage()
{
	m_pCurrentStage->SetActive(false);
}

void SceneManager::SetScene(const String &name)
{
	SceneInfo* info = SceneManager::GetSceneInfo(name);
	
	if (!info)
		return;
	
	m_pCurrentScene = info->GetScenePointer();
  m_pCurrentScene->BeginScene();
}

void SceneManager::SwitchScene(const String& name)
{
  m_pCurrentScene->PauseScene();
	SetScene(name);
  m_pCurrentStage->ForceUpdate();
}

void SceneManager::ChangeScene(const String& name)
{
	m_pCurrentScene->EndScene();
	SetScene(name);
  m_pCurrentStage->ForceUpdate();
}

void SceneManager::UpdateScene(const FrameInfo &frameInfo)
{
	m_pCurrentScene->Update(frameInfo);
}

bool SceneManager::AddScene(Scene *scene)
{
	if (!scene)
		return false;

	SceneInfoList::iterator iter = m_SceneInfoList.find(scene->GetSceneName());
	if (iter != m_SceneInfoList.end())
		return false;
		
	m_SceneInfoList[scene->GetSceneName()] = scene;
	m_pCurrentScene = scene;
	
	return true;
}

SceneInfo* SceneManager::GetSceneInfo(const String &name)
{
  SceneInfoList::iterator iter = m_SceneInfoList.find(name);
	if (iter == m_SceneInfoList.end())
		return 0;
	
	return &iter->second;
}

void SceneManager::DispatchEvent(KeyEvent evt, int info)
{
	if (!m_pCurrentScene)
		return;
	
	m_pCurrentScene->HandleKeyEvent(evt, info);
}

void SceneManager::DispatchEvent(MouseEvent evt, int info)
{
	if (!m_pCurrentScene)
		return;
	
	m_pCurrentScene->HandleMouseEvent(evt, info);
}

void SceneManager::DispatchEvent(WindowEvent evt, int info)
{
	if (!m_pCurrentScene)
		return;
	
	m_pCurrentScene->HandleWindowEvent(evt, info);
}

void SceneManager::DestroyAllScenes()
{
	SceneInfoList::iterator iter;
	for (iter = m_SceneInfoList.begin(); iter != m_SceneInfoList.end(); iter++)
		delete iter->second.GetScenePointer();
	
	m_pCurrentScene = 0;
	
  m_SceneInfoList.clear();
}