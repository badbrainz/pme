#ifndef TILESET
#define TILESET

#include "GUI/GUIUtils.h"
#include "Tools/Logger.h"
#include "Appearance/Appearance.h"

class PteObject;

class TileSet
{
  public:
    bool        LoadTileSet(PteObject* pteObject);
    GUIPanel*   GetGuiComponent();
  
  private:
    void        DrawFullScreenQuad(int width, int height);
    
    GUIButton*  CreateButton(const String& name, const Texture& texture, const Tuple4f& rectangle);
    GUIPanel*   CreatePanel(const String& name);
  
  protected:
    GUIPanel  m_Gui;
    Texture   m_Texture;
};

#endif