#include "GUI/GUIFontManager.h"
#include "TabbedPanel.h"
#include "GUI/GUIButton.h"

TabbedPanel::TabbedPanel(const char *cbs) : GUIRectangle(cbs)
{
  setTabButtonsBordersColor(0.0f, 0.0f, 0.0f);
  setTabButtonsColor(100, 150, 190);
  setFontScales(1.0f, 1.0f);
  setPosition(0.5, 0.5);

  upperPanel = new GUIPanel("tpup");
  lowerPanel = new GUIPanel("tpdown");
  mainPanel  = new GUIPanel("tpmp");
  
  mainPanel->setLayout(YAXIS_LAYOUT);
  mainPanel->setInterval(2, 2);
 
  mainPanel->setVisibleBounds(true);
  mainPanel->enableBGColor(true);

  upperPanel->setLayout(XAXIS_LAYOUT);
  upperPanel->setClipSize(0);

  lowerPanel->setInterval(0,0);
  lowerPanel->setClipSize(0);

  mainPanel->addWidget(upperPanel);
  mainPanel->addWidget(lowerPanel);

  widgetType = TABBED_PANEL;
  fontIndex  =            0;
  lockItems  =        false;
}

TabbedPanel::~TabbedPanel()
{
  deleteObject(mainPanel);
}

GUIEventListener  *TabbedPanel::getEventsListener()
{
  return this;
}

void  TabbedPanel::setFontScales(const Tuple2f &scales)
{
  setFontScales(scales.x, scales.y);
}

void  TabbedPanel::setFontScales(float wScale, float hScale)
{
  fontScales.x = clamp(hScale, 0.01f, 20.0f);
  fontScales.y = clamp(wScale, 0.01f, 20.0f);
}

const Tuple2f  &TabbedPanel::getFontScales()
{
  return fontScales;
}

bool TabbedPanel::loadXMLSettings(XMLElement *element)
{
  if(!element || element->getName() != "TabbedPanel")
    return Logger::writeErrorLog("Need a TabbedPanel node in the xml file");

  XMLElement *child                = NULL,
             *subChild             = NULL;
  Tuple3f     bordersColor         = upperPanel->getBordersColor();
  Tuple4f     bgColor              = upperPanel->getBGColor();
  int         count                = 0;

  if(child = element->getChildByName("hScale"))
    fontScales.y = child->getValuef();

  if(child = element->getChildByName("wScale"))
    fontScales.x = child->getValuef();
 
  if(child = element->getChildByName("TabButtonsBordersColor"))
    XMLElement::loadRX_GY_BZf(*child, tabButtonsBordersColor);

  if(child = element->getChildByName("TabButtonsColor"))
    XMLElement::loadRX_GY_BZf(*child, tabButtonsColor);

  if(child = element->getChildByName("BordersColor"))
    XMLElement::loadRX_GY_BZf(*child, bordersColor);

  if(child = element->getChildByName("BGColor"))
    XMLElement::loadRX_GY_BZ_AWf(*child, bgColor);

  if(child = element->getChildByName("fontIndex"))
    fontIndex =  clamp(child->getValuei(), 0, 50);

  setFontScales(fontScales);

  for(size_t i = 0; i < element->getChildrenCount(); i++)
  {
    if(!(child = element->getChild(i)))
      continue;

    const String &childName = child->getName();

    if(childName == "Panel")
    {
      GUIPanel *panel = new GUIPanel();
      if(!panel->loadXMLSettings(child) || !addPanel(panel))
        deleteObject(panel);
      continue;
    }
  }

  mainPanel->setBordersColor(bordersColor);
  mainPanel->setBGColor(bgColor);

  return GUIRectangle::loadXMLSettings(element) && lowerPanel->getWidgets().size();
}

bool TabbedPanel::addPanel(GUIPanel *panel)
{
  if(lockItems)
    return false;

  if(lowerPanel->addWidget(panel))
  {
    int count = int(lowerPanel->getWidgets().size()) - 1;
    GUIButton *tabButton = new GUIButton(String(" ") + count + " tb");
    tabButton->setBordersColor(tabButtonsBordersColor);
    tabButton->setColor(tabButtonsColor);
    tabButton->setLabelString(String("  ") + panel->getCallbackString() + "  ");
    tabButton->getLabel()->setFontIndex(fontIndex);
    tabButton->getLabel()->setScales(fontScales);
    tabButton->setMinAlpha((count == 0) ? 0.75f : 0.5f);
    tabButton->enableBounce(false);
    tabButton->setClipSize(0);
    upperPanel->addWidget(tabButton);
    panel->setVisible((count == 0));
    return true;
  }
  return false;
}

const   GUIPanel*  TabbedPanel::getCurrentPanel() const
{
  const Widgets &widgets = lowerPanel->getWidgets();
  int   count  = int(widgets.size());
 
  for(int t = 0; t < count; t++)
    if(widgets[t]->isVisible())
       return (GUIPanel*)widgets[t];

  return NULL;
}

const   int  TabbedPanel::getCurrentPanelIndex() const
{
  const Widgets &widgets = lowerPanel->getWidgets();
  int   count  = int(widgets.size());
 
  for(int t = 0; t < count; t++)
    if(widgets[t]->isVisible())
       return t;

  return -1;
}

void TabbedPanel::checkKeyboardEvents(KeyEvent evt, int extraInfo)
{
  upperPanel->checkKeyboardEvents(evt, extraInfo);
  lowerPanel->checkKeyboardEvents(evt, extraInfo);
}

void TabbedPanel::checkMouseEvents(MouseEvent &evt, int extraInfo, bool rBits)
{
  upperPanel->checkMouseEvents(evt, extraInfo, rBits);
  lowerPanel->checkMouseEvents(evt, extraInfo, rBits);
}

void TabbedPanel::actionPerformed(GUIEvent &evt)
{
  GUIEventListener *eventsListener  = parent->getEventsListener();
  const String     &cbs             = evt.getCallbackString();
  GUIRectangle     *sourceRectangle = evt.getEventSource();
  int               widgetType      = sourceRectangle->getWidgetType();

  if((widgetType == BUTTON) && sourceRectangle->isClicked())
  {
    const Widgets &widgets = lowerPanel->getWidgets(), 
                  &buttons = upperPanel->getWidgets();
    int   target = atoi(sourceRectangle->getCallbackString()),
          count  = int(widgets.size());
    for(int t = 0; t < count; t++)
    {
      widgets[t]->setVisible(t == target);
      ((GUIAlphaElement*)buttons[t])->setMinAlpha((t == target) ? 1.0f : 0.5f);
    }
  }
}

GUIButton *TabbedPanel::getTabButton(int index)
{
  const Widgets &widgets = upperPanel->getWidgets();
  int   count  = int(widgets.size());
 
  for(int t = 0; t < count; t++)
    if(t == index)
      return (GUIButton*)widgets[t];

  return NULL;
}

void TabbedPanel::render(float clockTick)
{
  if(!parent || !visible)
    return;

  const Tuple4i &buttonBounds = upperPanel->getWidgets()[0]->getWindowBounds(),
                &mainBounds   = mainPanel->getWindowBounds();

  const Tuple3f &borderClr    = mainPanel->getBordersColor();

  lockItems = true;
  mainPanel->render(clockTick);

  glColor3fv(borderClr);
  glBegin(GL_LINES);
  glVertex2i(mainBounds.x, buttonBounds.w);
  glVertex2i(mainBounds.z, buttonBounds.w);
  glEnd();
  glColor3f(1,1,1);
}

const void TabbedPanel::computeWindowBounds()
{
  if(parent && update)
  {
    upperPanel->setParent(mainPanel);

    mainPanel->setAnchorPoint(getAnchorPoint());
    mainPanel->setPosition(getPosition());
    mainPanel->setParent(parent);
    mainPanel->forceUpdate(true);

    windowBounds = mainPanel->getWindowBounds();

    const Widgets & widgets     = lowerPanel->getWidgets();
    float           height      = 0;
    int             widgetCount = (int)widgets.size();

    for(int i = 0; i < widgetCount; i++)
      height = height < widgets[i]->getHeight() ? widgets[i]->getHeight() : height;
    lowerPanel->setDimensions(1.0f, height);
    upperPanel->setParent(this);
  }
}

void  TabbedPanel::setTabButtonsColor(const Tuple3f& color)
{
  setTabButtonsColor(color.x, color.y, color.z);
}

void  TabbedPanel::setTabButtonsColor(float r, float g, float b)
{
  tabButtonsColor.set(clamp(r, 0.0f, 255.0f),
                        clamp(g, 0.0f, 255.0f),
                        clamp(b, 0.0f, 255.0f));

  tabButtonsColor.x /= (tabButtonsColor.x > 1.0) ? 255.0f : 1.0f;
  tabButtonsColor.y /= (tabButtonsColor.y > 1.0) ? 255.0f : 1.0f;
  tabButtonsColor.z /= (tabButtonsColor.z > 1.0) ? 255.0f : 1.0f;
}

const Tuple3f &TabbedPanel::getTabButtonsColor() const 
{
  return tabButtonsColor;
}

void  TabbedPanel::setTabButtonsBordersColor(const Tuple3f& color)
{
  setTabButtonsBordersColor(color.x, color.y, color.z);
}

void  TabbedPanel::setTabButtonsBordersColor(float r, float g, float b)
{
  tabButtonsBordersColor.set(clamp(r, 0.0f, 255.0f),
                             clamp(g, 0.0f, 255.0f),
                             clamp(b, 0.0f, 255.0f));
  tabButtonsBordersColor.x /= (tabButtonsBordersColor.x > 1.0) ? 255.0f : 1.0f;
  tabButtonsBordersColor.y /= (tabButtonsBordersColor.y > 1.0) ? 255.0f : 1.0f;
  tabButtonsBordersColor.z /= (tabButtonsBordersColor.z > 1.0) ? 255.0f : 1.0f;
}

GUIPanel*  TabbedPanel::getLowerPanel()
{
  return lowerPanel;
}

GUIPanel*  TabbedPanel::getUpperPanel()
{
  return mainPanel;
}

GUIPanel*  TabbedPanel::getMainPanel()
{
  return mainPanel;
}

const Tuple3f &TabbedPanel::getTabButtonsBordersColor() const 
{
  return tabButtonsBordersColor;
}