#include "EditorScene.h"
#include "../C3DDatabase/TerrainDatabase.h"

#include "../C3DNODES/C3DVisitors/SpatialIndexDebugVisitor.h"
#include "../C3DNODES/C3DVisitors/SpatialVisibilityVisitor.h"
#include "../C3DNODES/C3DVisitors/TileGraphRendererVisitor.h"
#include "../C3DNODES/C3DVisitors/SpatialIntersectVisitor.h"

//TerrainDatabase terrainDatabase;
GameFileDescriptor gameFileDescriptor;
SpatialVisibilityVisitor visibilityVisitor;
TileGraphRendererVisitor baseVisitor;
TileGraphRendererVisitor blendVisitor;
SpatialIndexDebugVisitor debugVisitor;
SpatialIntersectVisitor intersectVisitor;

EditorScene::EditorScene(const String &name) : Scene(name)
{
  m_bMouseLocked    = false;
  m_bDebugView      = false;
  m_bPickingEnabled = false;
}

bool EditorScene::Initialize()
{
  Scene::Initialize();
  
  glPolygonOffset(-1, -1);
  
  m_pFpsCounter  = (GUILabel*)  m_Gui.getWidgetByCallbackString("FpsCounter");
  m_UserControls = (GUIPanel*)  m_Gui.getWidgetByCallbackString("EditorPanel");
  m_TileButton   = (GUIButton*) m_Gui.getWidgetByCallbackString("ActiveTileTexture");
  
  m_Camera.setViewerPosition(Tuple3f(0,4,8));
  m_Camera.setFocusPosition(Tuple3f(0,0,-8));
  m_Camera.setUpDirection(Tuple3f(0,1,0));
  m_Camera.setTranslationSpeed(0.05f);
  m_Camera.update(0);
  m_Frustum.update();

  /*gameFileDescriptor.pvePath = MediaPathManager::lookUpMediaPath("MP_4_3.PVE");
  gameFileDescriptor.ptePath = MediaPathManager::lookUpMediaPath("MP_4_3.PTE");
  
  m_pTerrainDatabase->LoadGameData(gameFileDescriptor);
  
  visibilityVisitor.SetTerrain(m_pTerrainDatabase);
  visibilityVisitor.SetFrustum(&m_Frustum);
  intersectVisitor.SetRay(&m_Ray);
  blendVisitor.EnableBlend(true);
  
  m_pTerrainDatabase->Cull(&visibilityVisitor);
  
  for (unsigned int i = 0; i < m_pTerrainDatabase->GetTextureCount(); i++)
    m_TileSet.CreateTileSet(m_pTerrainDatabase->GetTextureID(i));

  m_Gui.addWidget(m_TileSet.GetGuiComponent());
  m_Gui.forceUpdate(true);*/

  return true;
}

void EditorScene::InitializeNewMap(const MapDescriptor& descriptor)
{
  gameFileDescriptor.pvePath = MediaPathManager::lookUpMediaPath("MP_4_3.PVE");
  gameFileDescriptor.ptePath = MediaPathManager::lookUpMediaPath("MP_4_3.PTE");
  
  m_pTerrainDatabase->LoadGameData(gameFileDescriptor);
  
  visibilityVisitor.SetTerrain(m_pTerrainDatabase);
  visibilityVisitor.SetFrustum(&m_Frustum);
  intersectVisitor.SetRay(&m_Ray);
  blendVisitor.EnableBlend(true);
  
  m_pTerrainDatabase->Cull(&visibilityVisitor);
  
  for (unsigned int i = 0; i < m_pTerrainDatabase->GetTextureCount(); i++)
    m_TileSet.CreateTileSet(m_pTerrainDatabase->GetTextureID(i));

  m_Gui.addWidget(m_TileSet.GetGuiComponent());
  m_Gui.forceUpdate(true);
}

void EditorScene::BeginScene()
{
  Scene::BeginScene();
}

void EditorScene::Update(const FrameInfo &frameInfo)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  const FrameInfo* info = &frameInfo;

  m_Camera.update(info->m_Interval);
  m_Frustum.update();

  m_pTerrainDatabase->Cull(&visibilityVisitor);//important: this must be used first.
  m_pTerrainDatabase->Draw(0, &baseVisitor);
  m_pTerrainDatabase->Draw(1, &blendVisitor);
  
  if (m_bDebugView)
    m_pTerrainDatabase->Cull(&debugVisitor);
  
  if (m_bPickingEnabled)
  {
    glEnable(GL_POLYGON_OFFSET_LINE);
    m_pTerrainDatabase->Cull(&intersectVisitor);
    glDisable(GL_POLYGON_OFFSET_LINE);
  }

  if (m_pFpsCounter)
    m_pFpsCounter->setLabelString(String("Current FPS: ") + int(info->m_Fps));

  Renderer::enter2DMode(info->m_Width, info->m_Height);
    m_Gui.render(info->m_Interval);
  Renderer::exit2DMode();
}

void EditorScene::actionPerformed(GUIEvent &evt)
{
  const String &callbackString  = evt.getCallbackString();
  GUIRectangle *sourceRectangle = evt.getEventSource();
  int           widgetType      = sourceRectangle->getWidgetType();

  if (widgetType == BUTTON)
  {
    GUIButton *button = (GUIButton*) sourceRectangle;
    
    if (button->isClicked())
    {
      m_SceneController.Execute(callbackString);
      
      //hmm...
      if (m_TileSet.IsVisible())
      {
        const TextureTileInfo *info = m_TileSet.GetTileInfo(callbackString);
        if (info)
        {
          m_TileButton->setTextureRectangle(info->coords);
          m_TileButton->setTexture(*info->texture);
          m_Gui.forceUpdate(true);
        }
      }
      return;
    }
  }
  
  if (widgetType == CHECK_BOX)
  {
    GUICheckBox *checkBox = (GUICheckBox*) sourceRectangle;
    
    if (checkBox->isClicked())
    {
      if (callbackString == "Debug")          {m_bDebugView      = checkBox->isChecked();}
      if (callbackString == "Picking")        {m_bPickingEnabled = checkBox->isChecked();}
      if (callbackString == "DisplayTileset") {m_TileSet.SetVisible(checkBox->isChecked());}
    }
  }
}

void EditorScene::HandleMouseEvent(MouseEvent evt, int extraInfo)
{
  switch (extraInfo)
  {
    case DRAGGED:
      m_Camera.lockMouse(!m_bMouseLocked);
      m_Camera.setMouseInfo(evt.getX(), evt.getY());
    break;
    
    case RELEASED:
      m_bMouseLocked = false;
    break;
    
    case CLICKED:
      if (m_UserControls)
      {
        const Tuple4i &windowBounds  = m_UserControls->getWindowBounds();
        const Tuple4i &tilesetBounds = m_TileSet.GetGuiBounds();
        m_bMouseLocked  = ((evt.getY() >= windowBounds.y)  &&
                           (evt.getY() <= windowBounds.w)  &&
                           (evt.getX() >= windowBounds.x)  &&
                           (evt.getX() <= windowBounds.z)) ||
                          ((m_TileSet.IsVisible())         &&
                           (evt.getY() >= tilesetBounds.y) && 
                           (evt.getY() <= tilesetBounds.w) &&
                           (evt.getX() >= tilesetBounds.x) &&
                           (evt.getX() <= tilesetBounds.z));
      }
    break;
    
    case MOVED:
      m_Camera.lockMouse(false);
      m_Camera.setMouseInfo(evt.getX(), evt.getY());
      
      if (m_bPickingEnabled)
        m_Ray.Set(m_Camera.getViewerPosition(), Renderer::getWorldCoords(Tuple3f((float)evt.getX(),(float)evt.getYInverse(),1)));
    break;
  }

  m_Gui.checkMouseEvents(evt, extraInfo);
}

void EditorScene::HandleKeyEvent(KeyEvent evt, int extraInfo)
{
  m_Gui.checkKeyboardEvents(evt, extraInfo);

  switch (extraInfo)
  {
    case KEY_RELEASED:
      m_Camera.setKeyboardInput(evt, false);
    break;

    case KEY_PRESSED:
      m_Camera.setKeyboardInput(evt, true);
    break;
  }
}

EditorScene::~EditorScene()
{
}