//#include "TileSet.h"
#include "TabbedPanel.h"
#include "Renderer/Renderer.h"
#include "../C3DParsers/PteObject.h"
#include "../C3DScenes/EditorScene.h"

TileSet::TileSet()
{
  m_uiTilePageCount = 0;

  m_pGui          = new GUIPanel("TileSet");
  m_pTilesetpanel = new TabbedPanel("TileSetPanel");

  m_pTilesetpanel->setTabButtonsBordersColor(Tuple3f(0,1,0));
  m_pTilesetpanel->setTabButtonsColor(Tuple3f(0,0.4f,0));
  m_pTilesetpanel->getMainPanel()->setClipSize(0);
  m_pTilesetpanel->getMainPanel()->setBordersColor(Tuple3f(0.2f,0.2f,0.2f));
  m_pTilesetpanel->getMainPanel()->setBGColor(Tuple4f(50.0f,50.0f,50.0f,0.75f));

  m_pGui->addWidget(m_pTilesetpanel);
  m_pGui->setVisible(false);
}

bool TileSet::CreateTileSet(unsigned int textureID)
{
  float xOff = .25, yOff = .25;
  
  GUIPanel    *newTabpanel;
  Texture     *newTexture;
  GUIButton   *newButton;
  GUIPanel    *newPanel;
  
  newTexture = m_ManagedTextures.Create();
  String texturename = String("TexturePage") + m_uiTilePageCount;
  newTexture->create2DShell(texturename, 256, 256, GL_RGBA8, GL_RGBA, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
  
  glViewport(0, 0, 256, 256);
  
  Renderer::enter2DMode();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    DrawFullScreenQuad(256, 256);
    newTexture->copyCurrentBuffer();
    glDisable(GL_TEXTURE_2D);
  Renderer::exit2DMode();
  
  String tabpanelname = String("TilePage") + m_uiTilePageCount;///"TilePage_page#"
  newTabpanel = CreateTilePage(tabpanelname);
  
  for (int y = 0; y < 4; y++)
  {
    String panelname = String("TileRow") + m_uiTilePageCount; panelname += y;///"TileRow_page#_row#"
    newPanel = CreateTileRow(panelname);
    newPanel->setInterval(4,2);
    
    for (int x = 0; x < 4; x++)
    {
      Tuple4f rectangle(xOff*x, yOff*y, xOff*(x+1), yOff*(y+1));

      String buttonname = String("TileTexture") + m_uiTilePageCount; buttonname += (y*4+x);///"TileTexture_page#_buttonindex"
      newButton = CreateTileButton(buttonname, *newTexture, rectangle);
      newButton->setClipSize(0);

      newPanel->addWidget(newButton);
      
      TextureTileInfo info;
      info.texture      = newTexture;
      info.pageIndex    = m_uiTilePageCount;
      info.coordsIndex  = y*4+x;
      info.coords       = rectangle;
      m_TextureInfoList[buttonname] = info;
    }
    
    newTabpanel->addWidget(newPanel);
  }
  
  m_pTilesetpanel->addPanel(newTabpanel);
  m_pTilesetpanel->getTabButton(m_uiTilePageCount)->setMinAlpha(0.4f);
  m_pTilesetpanel->getTabButton(m_uiTilePageCount)->setLabelString(String() + m_uiTilePageCount);

  m_pGui->forceUpdate(true);
  
  ++m_uiTilePageCount;

  return true;
}

GUIPanel* TileSet::GetGuiComponent()
{
  return m_pGui;
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
  m_pGui->setVisible(visible);
}

const Tuple4i& TileSet::GetGuiBounds()
{
  return m_pGui->getWindowBounds();
}

bool TileSet::IsVisible()
{
  return m_pGui->isVisible();
}

const TextureTileInfo* TileSet::GetTileInfo(const String& name)
{
  std::map <String, TextureTileInfo>::const_iterator iter = m_TextureInfoList.find(name);
  
  if (iter != m_TextureInfoList.end())
    return &iter->second;
  else
    return 0;
}
