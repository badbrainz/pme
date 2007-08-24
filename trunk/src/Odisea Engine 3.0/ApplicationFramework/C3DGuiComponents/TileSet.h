#ifndef TILESET
#define TILESET

#include "GUI/GUIUtils.h"
#include "Tools/Logger.h"
#include "Appearance/Appearance.h"
#include "Events/GUIEventListener.h"
#include "../C3DManagers/ResourceManager.h"

#include <map>

class PteObject;
class TabbedPanel;
class EditorScene;

struct TextureTileInfo
{
  Texture *texture;
  unsigned int pageIndex;
  unsigned int coordsIndex;
  Tuple4f      coords;
};

class TileSet
{
  public:
    TileSet();
    //~TileSet(){}

  public:
    bool                    CreateTileSet(unsigned int textureID);

    const TextureTileInfo*  GetTileInfo(const String& name);
    GUIPanel*               GetGuiComponent();
    const Tuple4i&          GetGuiBounds();
    
    void                    SetVisible(bool visible);
    bool                    IsVisible();
  
  private:
    void                    DrawFullScreenQuad(int width, int height);
    
    GUIButton*              CreateTileButton(const String& name, const Texture& texture, const Tuple4f& rectangle);
    GUIPanel*               CreateTilePage(const String& name);
    GUIPanel*               CreateTileRow(const String& name);
  
  protected:
    GUIPanel       *m_pGui;
    TabbedPanel    *m_pTilesetpanel;

    int             m_uiTilePageCount;

    std::map        <String, TextureTileInfo> m_TextureInfoList;
    ResourceManager <Texture, 20>             m_ManagedTextures;
};

#endif