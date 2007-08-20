#include "EditorScene.h"
#include "../C3DDatabase/TerrainDatabase.h"

#include "../C3DNODES/C3DVisitors/SpatialIndexDebugVisitor.h"
#include "../C3DNODES/C3DVisitors/SpatialVisibilityVisitor.h"
#include "../C3DNODES/C3DVisitors/TileGraphRendererVisitor.h"
#include "../C3DNODES/C3DVisitors/SpatialIntersectVisitor.h"

TerrainDatabase terrainDatabase;
GameFileDescriptor gameFileDescriptor;
SpatialVisibilityVisitor visibilityVisitor;
TileGraphRendererVisitor baseVisitor;
TileGraphRendererVisitor blendVisitor;
SpatialIndexDebugVisitor debugVisitor;
SpatialIntersectVisitor intersectVisitor;
PteObject pteObject;
EditorScene::EditorScene(const String &name) : Scene(name)
{
  m_bMouseLocked    = false;
  m_bDebugView      = false;
  m_bPickingEnabled = false;
}

bool EditorScene::Initialize()
{
  Scene::Initialize();
  
  m_pFpsCounter  = (GUILabel*)  m_Gui.getWidgetByCallbackString("FpsCounter");
  m_UserControls = (GUIPanel*)  m_Gui.getWidgetByCallbackString("EditorPanel");
  m_TileButton   = (GUIButton*) m_Gui.getWidgetByCallbackString("ActiveTileTexture");
  
  m_Camera.setViewerPosition(Tuple3f(0,4,8));
  m_Camera.setFocusPosition(Tuple3f(0,0,-8));
  m_Camera.setUpDirection(Tuple3f(0,1,0));
  m_Camera.setTranslationSpeed(0.05f);
  m_Camera.update(0);
  m_Frustum.update();

  gameFileDescriptor.pvePath = MediaPathManager::lookUpMediaPath("MP_4_3.PVE");
  gameFileDescriptor.ptePath = MediaPathManager::lookUpMediaPath("MP_4_3.PTE");
  
  terrainDatabase.LoadGameData(gameFileDescriptor);
  
  visibilityVisitor.SetFrustum(&m_Frustum);
  visibilityVisitor.SetTerrain(&terrainDatabase);
  terrainDatabase.Cull(&visibilityVisitor);
  
  blendVisitor.EnableBlend(true);
  
  intersectVisitor.SetRay(&m_Ray);
  
  glPolygonOffset(-1, -1);
  
  if (pteObject.LoadFromFile(gameFileDescriptor.ptePath))//warning: loaded twice
    m_TileSet.LoadTileSet(&pteObject);
  
  m_Gui.addWidget(m_TileSet.GetGuiComponent());
  m_Gui.forceUpdate(true);

  return true;
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

  terrainDatabase.Cull(&visibilityVisitor);//important: this must be used first.
  terrainDatabase.Draw(0, &baseVisitor);
  terrainDatabase.Draw(1, &blendVisitor);
  
  if (m_bDebugView)
    terrainDatabase.Cull(&debugVisitor);
  
  if (m_bPickingEnabled)
  {
    glEnable(GL_POLYGON_OFFSET_LINE);
    terrainDatabase.Cull(&intersectVisitor);
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
      const TextureTileDescriptor *descriptor = m_TileSet.GetTileInfo(callbackString);
      if (descriptor)
      {
        m_TileButton->setTextureRectangle(descriptor->coords);
        m_TileButton->setTexture(*descriptor->texture);
        m_Gui.forceUpdate(true);
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
      m_Camera.lockMouse(!m_bMouseLocked/* && !m_TileSet.IsVisible()*/);
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