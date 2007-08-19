#ifndef TILESET
#define TILESET

#include "GUI/GUIUtils.h"
#include "Tools/Logger.h"
#include "Appearance/Appearance.h"
#include "../C3DManagers/ResourceManager.h"
//#include "../OC/ocarray.h"
#include <map>

class PteObject;

struct TextureTileDescriptor
{
  Texture *texture;
  unsigned int pageIndex;
  unsigned int coordsIndex;
  Tuple4f      coords;
};

class TileSet// : public GUIEventListener
{
  public:
    bool            LoadTileSet(PteObject* pteObject);
    GUIPanel*       GetGuiComponent();
    //void            actionPerformed(GUIEvent &evt);
    void            SetVisible(bool visible);
    bool            IsVisible();
    const Tuple4i&  GetGuiBounds();
    
    const TextureTileDescriptor* GetTileInfo(const String& name);
  
  private:
    void            DrawFullScreenQuad(int width, int height);
    GUIButton*      CreateTileButton(const String& name, const Texture& texture, const Tuple4f& rectangle);
    GUIPanel*       CreateTilePage(const String& name);
    GUIPanel*       CreateTileRow(const String& name);
  
  protected:
    GUIPanel        m_Gui;
    Texture         m_Texture;
    //Array <Texture> m_TexturePages;
    //Array <Texture> m_TexturePages;
    std::map <String, TextureTileDescriptor> m_TextureInfoList;
    ResourceManager <Texture, 20>  m_ManagedTextures;
};

#endif