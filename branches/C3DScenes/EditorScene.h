#ifndef EDITORSCENE
#define EDITORSCENE

#include "Scene.h"
#include "Renderer/Renderer.h"
#include "../C3DParsers/PveObject.h"
#include "../C3DGeometry/Ray3D.h"
#include "../C3DGuiComponents/TileSet.h"

class TerrainDatabase;

class EditorScene : public Scene
{
  public:
    EditorScene(const String &name = "EditorScene");
    virtual ~EditorScene();
  
  public:
    virtual void  actionPerformed(GUIEvent &evt);

    virtual void  HandleMouseEvent(MouseEvent evt, int extraInfo);
    virtual void  HandleKeyEvent(KeyEvent evt, int extraInfo);
    
    virtual bool  Initialize();
    virtual void  BeginScene();
    virtual void  Update(const FrameInfo &frameInfo);
    
    void InitializeNewMap(const MapDescriptor& descriptor);
    void SetTerrainDatabase(TerrainDatabase* database){m_pTerrainDatabase = database;}

  private:
    GUILabel   *m_pFpsCounter;
    GUIPanel   *m_UserControls;
    GUIButton  *m_TileButton;
    
    Frustum     m_Frustum;
    Camera      m_Camera;
    
    PveObject   m_PveObject;
    
    bool        m_bMouseLocked,
                m_bPickingEnabled,
                m_bDebugView;
    
    Ray3D       m_Ray;
    
    TileSet     m_TileSet;
    
    TerrainDatabase *m_pTerrainDatabase;
};

#endif