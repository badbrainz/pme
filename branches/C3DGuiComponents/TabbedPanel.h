#ifndef TABBEDPANEL
#define TABBEDPANEL

#include "GUI/GUIPanel.h"

class GUIButton;

class TabbedPanel : public GUIRectangle, public GUIEventListener
{
  public:
    TabbedPanel(const char *callbackString = NULL);
   ~TabbedPanel();

    virtual GUIEventListener  *getEventsListener();
    virtual const void         computeWindowBounds();
    virtual bool               loadXMLSettings(XMLElement *node);
    virtual void               render(float clockTick);
    virtual void               checkMouseEvents(MouseEvent &evt, int extraInfo, bool reservedBits = false);
    virtual void               checkKeyboardEvents(KeyEvent evt, int extraInfo);
    virtual void               actionPerformed(GUIEvent &evt);

    void               setTabButtonsColor(float r, float g, float b);
    void               setTabButtonsColor(const Tuple3f& color);
    const Tuple3f     &getTabButtonsColor() const;

    void               setTabButtonsBordersColor(float r, float g, float b);
    void               setTabButtonsBordersColor(const Tuple3f& color);
    const Tuple3f     &getTabButtonsBordersColor() const;

    void               setFontScales(float w, float h);
    void               setFontScales(const Tuple2f &scales);
    const   Tuple2f   &getFontScales();

    bool               addPanel(GUIPanel *panel);
    const   GUIPanel*  getCurrentPanel()      const;
    const   int        getCurrentPanelIndex() const;

    GUIButton         *getTabButton(int index);
    GUIPanel          *getLowerPanel();
    GUIPanel          *getUpperPanel();
    GUIPanel          *getMainPanel();
  
  private:
    GUIPanel      *upperPanel,
                  *lowerPanel,
                  *mainPanel;

    Tuple3f        tabButtonsBordersColor,
                   tabButtonsColor;
    Tuple2f        fontScales;
    bool           lockItems;
    int            fontIndex;
};

#endif
