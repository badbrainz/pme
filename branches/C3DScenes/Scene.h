#ifndef SCENE
#define SCENE

#include "Events/GUIEventListener.h"
#include "Parsers/IOXMLObject.h"
#include "Renderer/Renderer.h"
#include "GUI/GUIUtils.h"

#include "Events/WindowEvent.h"
#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"

#include "../C3DControllers/SceneController.h"
#include "../FrameInfo.h"

class Scene : public GUIEventListener, public IOXMLObject
{
  public:
    Scene(const String &name = "DefaultScene");
    virtual ~Scene();
  
  public:
    virtual bool loadXMLSettings(XMLElement *element);
    virtual bool exportXMLSettings(ofstream &xmlFile){return true;}
    virtual void actionPerformed(GUIEvent &evt){}
		
    virtual bool Initialize();
    virtual void Destroy();
    
    virtual void Update(const FrameInfo &frameInfo);
		virtual void BeginScene();
    virtual void PauseScene();
		virtual void EndScene();
    
    virtual void HandleWindowEvent(WindowEvent evt, int extraInfo);
    virtual void HandleMouseEvent(MouseEvent evt, int extraInfo);
    virtual void HandleKeyEvent(KeyEvent evt, int extraInfo);
		
		String&			 GetSceneName();
  
  protected:
    void SetPerspective(WindowEvent evt, bool wEvt = false);
  
  protected:
    GUIFrame        m_Gui;
    bool            m_bInitialized;
    SceneController m_SceneController;
};

#endif