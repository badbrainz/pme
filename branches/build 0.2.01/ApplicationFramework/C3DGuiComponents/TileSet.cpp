#include "TileSet.h"
#include "Renderer/Renderer.h"
#include "../C3DParsers/PteObject.h"
#include "TabbedPanel.h"

bool TileSet::LoadTileSet(PteObject* pteObject)
{
  m_Gui.setCallbackString("TileSet");
  
  TabbedPanel* tabpanel = new TabbedPanel("TilePages");
  tabpanel->setTabButtonsBordersColor(Tuple3f(0,1,0));
  tabpanel->setTabButtonsColor(Tuple3f(0,0.4f,0));
  tabpanel->getMainPanel()->setClipSize(0);
  tabpanel->getMainPanel()->setBordersColor(Tuple3f(0.2f,0.2f,0.2f));
  tabpanel->getMainPanel()->setBGColor(Tuple4f(50.0f,50.0f,50.0f,0.75f));
  //*/
  m_Texture.create2DShell("TexturePage", 256, 256, GL_RGBA8, GL_RGBA, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

  glViewport(0, 0, 256, 256);
  
  Renderer::enter2DMode();
  
    pteObject->ActivateTexture(1);
    DrawFullScreenQuad(256, 256);
    m_Texture.copyCurrentBuffer();
    pteObject->DeactivateTexture();
  
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

  button1  = CreateButton("a", m_Texture, Tuple4f(xOff*0, yOff*0, xOff*1, yOff*1));
  button2  = CreateButton("b", m_Texture, Tuple4f(xOff*1, yOff*0, xOff*2, yOff*1));
  button3  = CreateButton("c", m_Texture, Tuple4f(xOff*2, yOff*0, xOff*3, yOff*1));
  button4  = CreateButton("d", m_Texture, Tuple4f(xOff*3, yOff*0, xOff*4, yOff*1));
  button5  = CreateButton("e", m_Texture, Tuple4f(xOff*0, yOff*1, xOff*1, yOff*2));
  button6  = CreateButton("f", m_Texture, Tuple4f(xOff*1, yOff*1, xOff*2, yOff*2));
  button7  = CreateButton("g", m_Texture, Tuple4f(xOff*2, yOff*1, xOff*3, yOff*2));
  button8  = CreateButton("h", m_Texture, Tuple4f(xOff*3, yOff*1, xOff*4, yOff*2));
  button9  = CreateButton("i", m_Texture, Tuple4f(xOff*0, yOff*2, xOff*1, yOff*3));
  button10 = CreateButton("j", m_Texture, Tuple4f(xOff*1, yOff*2, xOff*2, yOff*3));
  button11 = CreateButton("k", m_Texture, Tuple4f(xOff*2, yOff*2, xOff*3, yOff*3));
  button12 = CreateButton("l", m_Texture, Tuple4f(xOff*3, yOff*2, xOff*4, yOff*3));
  button13 = CreateButton("m", m_Texture, Tuple4f(xOff*0, yOff*3, xOff*1, yOff*4));
  button14 = CreateButton("n", m_Texture, Tuple4f(xOff*1, yOff*3, xOff*2, yOff*4));
  button15 = CreateButton("o", m_Texture, Tuple4f(xOff*2, yOff*3, xOff*3, yOff*4));
  button16 = CreateButton("p", m_Texture, Tuple4f(xOff*3, yOff*3, xOff*4, yOff*4));

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

  GUIPanel* tabPanel1 = new GUIPanel("1");
  tabPanel1->setInterval(0);
  tabPanel1->setLayout(YAXIS_CEN_LAYOUT);
  tabPanel1->addWidget(panelOne);
  tabPanel1->addWidget(panelTwo);
  tabPanel1->addWidget(panelThree);
  tabPanel1->addWidget(panelFour);
  tabpanel->addPanel(tabPanel1);
  //*/
  //GUIButton* button = new GUIButton("dave");
  //GUIPanel* panel = new GUIPanel("davepanel");
  //panel->addWidget(button);
  //tabpanel->addPanel(panel);
  m_Gui.addWidget(tabpanel);
  
  return true;
}

GUIPanel* TileSet::GetGuiComponent()
{
  return &m_Gui;
}

GUIButton* TileSet::CreateButton(const String& name, const Texture& tex, const Tuple4f& rect)
{
  GUIButton *button = new GUIButton(name);
  button->setTexture(tex);
  button->setMinAlpha(0.4f);
  button->setColor(Tuple3f(1,1,1));
  button->setDimensions(Tuple2f(64,64));
  button->setTextureRectangle(rect);
  return button;
}

GUIPanel* TileSet::CreatePanel(const String& name)
{
  GUIPanel *panel = new GUIPanel(name);
  panel->setLayout(XAXIS_LAYOUT);
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