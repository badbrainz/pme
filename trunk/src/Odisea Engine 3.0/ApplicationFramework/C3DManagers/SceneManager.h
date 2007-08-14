#ifndef SCENEMANAGER
#define SCENEMANAGER

#include "Events/WindowEvent.h"
#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"
#include "Tools/String.h"

#include "../FrameInfo.h"

#include <map>

class Scene;
class Stage;

class SceneInfo
{
  public:
    SceneInfo(Scene *scene = 0);
    SceneInfo(const SceneInfo &copy);
    SceneInfo &operator = (const SceneInfo &right);
    Scene* GetScenePointer();
  
  private:
    Scene *m_pScene;
};

typedef std::map <String, SceneInfo> SceneInfoList;

class SceneManager
{
  public:
    static void				SetStage(Stage *stage);
    static void       DeactivateCurrentStage();

    static void 			DestroyAllScenes();
    static bool 			AddScene(Scene *scene);
    static void       SetScene(const String &name);
    static void       SwitchScene(const String& name);
    static void       ChangeScene(const String& name);
    static void				UpdateScene(const FrameInfo &frameInfo);
    static SceneInfo* GetSceneInfo(const String &name);
    
    static void				DispatchEvent(KeyEvent evt, int info);
    static void				DispatchEvent(MouseEvent evt, int info);
    static void				DispatchEvent(WindowEvent evt, int info);
  
  private:
    static SceneInfoList  m_SceneInfoList;
    static Scene 				 *m_pCurrentScene;
    static Stage 				 *m_pCurrentStage;
};

#endif