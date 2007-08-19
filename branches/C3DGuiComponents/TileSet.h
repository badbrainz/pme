#ifndef TILESET
#define TILESET

#include "GUI/GUIUtils.h"
#include "Tools/Logger.h"
#include "Appearance/Appearance.h"
//#include "../OC/ocarray.h"
//#include <vector>

class PteObject;

class TileSet
{
  public:
    bool            LoadTileSet(PteObject* pteObject);
    GUIPanel*       GetGuiComponent();
    void            actionPerformed(GUIEvent &evt);
    void            SetVisible(bool visible);
    bool            IsVisible();
    const Tuple4i&  GetGuiBounds();
  
  private:
    void            DrawFullScreenQuad(int width, int height);
    GUIButton*      CreateTileButton(const String& name, const Texture& texture, const Tuple4f& rectangle);
    GUIPanel*       CreateTilePage(const String& name);
    GUIPanel*       CreateTileRow(const String& name);
  
  protected:
    GUIPanel        m_Gui;
    Texture         m_Texture;
    //Array <Texture> m_TexturePages;
    //std::vector <Texture> m_TexturePages;
};

#endif