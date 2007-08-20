//#include "TileSet.h"
#include "TabbedPanel.h"
#include "Renderer/Renderer.h"
#include "../C3DParsers/PteObject.h"
#include "../C3DScenes/EditorScene.h"
TileSet::TileSet()
{
}

bool TileSet::LoadTileSet(PteObject* pteObject)
{
  float xOff = .25, yOff = .25;
  
  TabbedPanel *tilesetpanel;
  GUIPanel    *tabpanel;
  Texture     *texture;
  GUIButton   *button;
  GUIPanel    *panel;

  m_Gui = new GUIPanel("TileSet");
  
  tilesetpanel = new TabbedPanel("TileSet");
  tilesetpanel->setTabButtonsBordersColor(Tuple3f(0,1,0));
  tilesetpanel->setTabButtonsColor(Tuple3f(0,0.4f,0));
  tilesetpanel->getMainPanel()->setClipSize(0);
  tilesetpanel->getMainPanel()->setBordersColor(Tuple3f(0.2f,0.2f,0.2f));
  tilesetpanel->getMainPanel()->setBGColor(Tuple4f(50.0f,50.0f,50.0f,0.75f));
  
  for (int i = 0; i < (int)pteObject->GetTexturePageCount(); i++)
  {
    texture = m_ManagedTextures.Create();
    String texturename = String("TexturePage") + i;
    texture->create2DShell(texturename, 256, 256, GL_RGBA8, GL_RGBA, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    
    glViewport(0, 0, 256, 256);
    
    Renderer::enter2DMode();
      pteObject->ActivateTexture(i);
      DrawFullScreenQuad(256, 256);
      texture->copyCurrentBuffer();
      pteObject->DeactivateTexture();
    Renderer::exit2DMode();
    
    String tabpanelname = String("TilePage") + i;
    tabpanel = CreateTilePage(tabpanelname);
    
    for (int y = 0; y < 4; y++)
    {
      String panelname = String("TileRow") + i; panelname += (y);
      panel = CreateTileRow(panelname);
      panel->setInterval(4,2);
      
      for (int x = 0; x < 4; x++)
      {
        Tuple4f rectangle(xOff*x, yOff*y, xOff*(x+1), yOff*(y+1));

        String buttonname = String("TileTexture") + i; buttonname += (y*4+x);
        button = CreateTileButton(buttonname, *texture, rectangle);
        button->setClipSize(0);

        panel->addWidget(button);
        
        TextureTileDescriptor descriptor;//use Strategy objects instead
        descriptor.texture      = texture;
        descriptor.pageIndex    = i;
        descriptor.coordsIndex  = y*4+x;
        descriptor.coords       = rectangle;
        m_TextureInfoList[buttonname] = descriptor;
      }
      
      tabpanel->addWidget(panel);
    }
    
    tilesetpanel->addPanel(tabpanel);
    tilesetpanel->getTabButton(i)->setMinAlpha(0.4f);
    tilesetpanel->getTabButton(i)->setLabelString(String() + i);
  }

  m_Gui->addWidget(tilesetpanel);
  m_Gui->forceUpdate(true);
  m_Gui->setVisible(false);
  
  return true;
}

GUIPanel* TileSet::GetGuiComponent()
{
  return m_Gui;
}

GUIButton* TileSet::CreateTileButton(const String& name, const Texture& tex, const Tuple4f& rect)
{
  GUIButton *button = new GUIButton(name);
  button->setTexture(tex);
  button->setMinAlpha(0.5f);
  button->setColor(Tuple3f(1,1,1));
  button->setDimensions(Tuple2f(64,64));
  button->setTextureRectangle(rect);
  return button;
}

GUIPanel* TileSet::CreateTileRow(const String& name)
{
  GUIPanel *panel = new GUIPanel(name);
  panel->setLayout(XAXIS_LAYOUT);
  return panel;
}

GUIPanel* TileSet::CreateTilePage(const String& name)
{
  GUIPanel *panel = new GUIPanel(name);
  panel->setInterval(0);
  panel->setLayout(YAXIS_CEN_LAYOUT);
  return panel;
}

void TileSet::DrawFullScreenQuad(int width, int height)
{
  glBegin(GL_TRIANGLE_STRIP);
  glTexCoord2f(0.0f, 1.0f); glVertex2i(    0, 0     );
  glTexCoord2f(0.0f, 0.0f); glVertex2i(    0, height);
  glTexCoord2f(1.0f, 1.0f); glVertex2i(width, 0     );
  glTexCoord2f(1.0f, 0.0f); glVertex2i(width, height);
  glEnd();
}

void TileSet::SetVisible(bool visible)
{
  m_Gui->setVisible(visible);
}

const Tuple4i& TileSet::GetGuiBounds()
{
  return m_Gui->getWindowBounds();
}

bool TileSet::IsVisible()
{
  return m_Gui->isVisible();
}

const TextureTileDescriptor* TileSet::GetTileInfo(const String& name)
{
  std::map <String, TextureTileDescriptor>::const_iterator iter = m_TextureInfoList.find(name);
  
  if (iter != m_TextureInfoList.end())
    return &iter->second;
  else
    return 0;
}
