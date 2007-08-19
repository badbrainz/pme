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

PteObject pteObjectx;

EditorScene::EditorScene(const String &name) : Scene(name)
{
  m_bMouseLocked    = false;
  m_bDebugView      = false;
  m_bPickingEnabled = false;
}

bool EditorScene::Initialize()
{
  Scene::Initialize();
  m_pFpsCounter  = (GUILabel*) m_Gui.getWidgetByCallbackString("FpsCounter");
  m_UserControls = (GUIPanel*) m_Gui.getWidgetByCallbackString("EditorPanel");
  
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
  //*/
  GUITabbedPanel* tabpanel = (GUITabbedPanel*) m_Gui.getWidgetByCallbackString("TilePages");
  if (tabpanel)
  {
    if (pteObjectx.LoadFromFile(MediaPathManager::lookUpMediaPath(gameFileDescriptor.ptePath)))
    {
      texture.create2DShell("TexturePage", 256, 256, GL_RGBA8, GL_RGBA, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

      glViewport(0, 0, 256, 256);
      Renderer::enter2DMode();
      pteObjectx.ActivateTexture(1);
      DrawFullScreenQuad(256, 256);
      texture.copyCurrentBuffer();
      pteObjectx.DeactivateTexture();
      Renderer::exit2DMode();

      float xOff = .25, yOff = .25;

      GUIButton *button1;
      GUIButton *button2;
      GUIButton *button3;
      GUIButton *button4;
      GUIButton *button5;
      GUIButton *button6;
      GUIButton *button7;
      GUIButton *button8;
      GUIButton *button9;
      GUIButton *button10;
      GUIButton *button11;
      GUIButton *button12;
      GUIButton *button13;
      GUIButton *button14;
      GUIButton *button15;
      GUIButton *button16;

      GUIPanel  *panelOne;
      GUIPanel  *panelTwo;
      GUIPanel  *panelThree;
      GUIPanel  *panelFour;
      //GUIPanel  *tabPanel;

      button1  = CreateButton("a", texture, Tuple4f(xOff*0, yOff*0, xOff*1, yOff*1));
      button2  = CreateButton("b", texture, Tuple4f(xOff*1, yOff*0, xOff*2, yOff*1));
      button3  = CreateButton("c", texture, Tuple4f(xOff*2, yOff*0, xOff*3, yOff*1));
      button4  = CreateButton("d", texture, Tuple4f(xOff*3, yOff*0, xOff*4, yOff*1));
      button5  = CreateButton("e", texture, Tuple4f(xOff*0, yOff*1, xOff*1, yOff*2));
      button6  = CreateButton("f", texture, Tuple4f(xOff*1, yOff*1, xOff*2, yOff*2));
      button7  = CreateButton("g", texture, Tuple4f(xOff*2, yOff*1, xOff*3, yOff*2));
      button8  = CreateButton("h", texture, Tuple4f(xOff*3, yOff*1, xOff*4, yOff*2));
      button9  = CreateButton("i", texture, Tuple4f(xOff*0, yOff*2, xOff*1, yOff*3));
      button10 = CreateButton("j", texture, Tuple4f(xOff*1, yOff*2, xOff*2, yOff*3));
      button11 = CreateButton("k", texture, Tuple4f(xOff*2, yOff*2, xOff*3, yOff*3));
      button12 = CreateButton("l", texture, Tuple4f(xOff*3, yOff*2, xOff*4, yOff*3));
      button13 = CreateButton("m", texture, Tuple4f(xOff*0, yOff*3, xOff*1, yOff*4));
      button14 = CreateButton("n", texture, Tuple4f(xOff*1, yOff*3, xOff*2, yOff*4));
      button15 = CreateButton("o", texture, Tuple4f(xOff*2, yOff*3, xOff*3, yOff*4));
      button16 = CreateButton("p", texture, Tuple4f(xOff*3, yOff*3, xOff*4, yOff*4));

      panelOne = CreatePanel("p1");
      panelOne->addWidget(button1);
      panelOne->addWidget(button2);
      panelOne->addWidget(button3);
      panelOne->addWidget(button4);

      panelTwo = CreatePanel("p2");
      panelTwo->addWidget(button5);
      panelTwo->addWidget(button6);
      panelTwo->addWidget(button7);
      panelTwo->addWidget(button8);

      panelThree = CreatePanel("p3");
      panelThree->addWidget(button9 );
      panelThree->addWidget(button10);
      panelThree->addWidget(button11);
      panelThree->addWidget(button12);

      panelFour = CreatePanel("p4");
      panelFour->addWidget(button13);
      panelFour->addWidget(button14);
      panelFour->addWidget(button15);
      panelFour->addWidget(button16);

      GUIButton* tabbutton = tabpanel->getTabButton(0);
      tabbutton->setMinAlpha(.4f);
      //tabPanel = (GUIPanel*) m_Gui.getWidgetByCallbackString("Tile Textures");
      GUIPanel* tabPanel1 = new GUIPanel("1");
      tabPanel1->setInterval(0);
      tabPanel1->setLayout(YAXIS_CEN_LAYOUT);
      tabPanel1->addWidget(panelOne);
      tabPanel1->addWidget(panelTwo);
      tabPanel1->addWidget(panelThree);
      tabPanel1->addWidget(panelFour);
      tabpanel->addPanel(tabPanel1);
    }
  }
  //*/
  return true;
}

GUIButton* EditorScene::CreateButton(const String& name, const Texture& tex, const Tuple4f& rect)
{
  GUIButton *button = new GUIButton(name);
  button->setTexture(tex);
  button->setMinAlpha(0.4f);
  button->setColor(Tuple3f(1,1,1));
  button->setDimensions(Tuple2f(64,64));
  button->setTextureRectangle(rect);
  return button;
}

GUIPanel* EditorScene::CreatePanel(const String& name)
{
  GUIPanel *panel = new GUIPanel(name);
  panel->setLayout(XAXIS_LAYOUT);
  return panel;
}

void EditorScene::DrawFullScreenQuad(int width, int height)
{
  glBegin(GL_TRIANGLE_STRIP);
  glTexCoord2f(0.0f, 1.0f); glVertex2i(    0, 0     );
  glTexCoord2f(0.0f, 0.0f); glVertex2i(    0, height);
  glTexCoord2f(1.0f, 1.0f); glVertex2i(width, 0     );
  glTexCoord2f(1.0f, 0.0f); glVertex2i(width, height);
  glEnd();
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
      m_SceneController.Execute(callbackString);
  }
  
  if (widgetType == CHECK_BOX)
  {
    GUICheckBox *checkBox = (GUICheckBox*) sourceRectangle;
    
    if (checkBox->isClicked())
    {
      if (callbackString == "Debug")   {m_bDebugView      = checkBox->isChecked();return;}
      if (callbackString == "Picking") {m_bPickingEnabled = checkBox->isChecked();return;}
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
        const Tuple4i &windowBounds = m_UserControls->getWindowBounds();
        m_bMouseLocked  = (evt.getY() >= windowBounds.y) && (evt.getY() <= windowBounds.w) &&
                          (evt.getX() >= windowBounds.x) && (evt.getX() <= windowBounds.z);
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