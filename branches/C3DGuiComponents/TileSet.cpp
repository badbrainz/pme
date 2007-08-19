#include "TileSet.h"
#include "Renderer/Renderer.h"
#include "../C3DParsers/PteObject.h"
#include "TabbedPanel.h"

bool TileSet::LoadTileSet(PteObject* pteObject)
{
  m_Gui.setCallbackString("TileSet");
  
  TabbedPanel* tilesetpanel = new TabbedPanel("TileSet");
  tilesetpanel->setTabButtonsBordersColor(Tuple3f(0,1,0));
  tilesetpanel->setTabButtonsColor(Tuple3f(0,0.4f,0));
  tilesetpanel->getMainPanel()->setClipSize(0);
  tilesetpanel->getMainPanel()->setBordersColor(Tuple3f(0.2f,0.2f,0.2f));
  tilesetpanel->getMainPanel()->setBGColor(Tuple4f(50.0f,50.0f,50.0f,0.75f));
  
  float xOff = .25, yOff = .25;
  
  for (int i = 0; i < (int)pteObject->GetTexturePageCount(); i++)
  {
    Texture texture;
    //m_TexturePages.append(texture);
    String texturename = String("TexturePage") + i;
    texture.create2DShell(texturename, 256, 256, GL_RGBA8, GL_RGBA, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    
    glViewport(0, 0, 256, 256);
    
    Renderer::enter2DMode();
      pteObject->ActivateTexture(i);
      DrawFullScreenQuad(256, 256);
      texture.copyCurrentBuffer();
      pteObject->DeactivateTexture();
    Renderer::exit2DMode();
    
    String tabpanelname = String("TilePage") + i;
    GUIPanel *tabpanel = CreateTilePage(tabpanelname);
    
    for (int y = 0; y < 4; y++)
    {
      String panelname = String("TileRow") + i; panelname += (y);
      GUIPanel *panel = CreateTileRow(panelname);
      
      for (int x = 0; x < 4; x++)
      {
        String buttonname = String("TileTexture") + i; buttonname += (y*4+x);
        GUIButton *button = CreateTileButton(buttonname, texture, Tuple4f(xOff*x, yOff*y, xOff*(x+1), yOff*(y+1)));
        panel->addWidget(button);
      }
      
      tabpanel->addWidget(panel);
    }
    
    tilesetpanel->addPanel(tabpanel);
    tilesetpanel->getTabButton(i)->setLabelString(String() + i);
    //m_TexturePages.push_back(texture);
  }
  
  m_Gui.addWidget(tilesetpanel);
  m_Gui.forceUpdate(true);
  m_Gui.setVisible(false);
  
  return true;
}

GUIPanel* TileSet::GetGuiComponent()
{
  return &m_Gui;
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
  m_Gui.setVisible(visible);
}

const Tuple4i& TileSet::GetGuiBounds()
{
  return m_Gui.getWindowBounds();
}

bool TileSet::IsVisible()
{
    return m_Gui.isVisible();
}

void TileSet::actionPerformed(GUIEvent &evt)
{
}
