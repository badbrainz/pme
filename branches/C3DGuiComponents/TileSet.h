#ifndef TILESET
#define TILESET

#include "GUI/GUIUtils.h"
#include "Tools/Logger.h"
#include "Appearance/Appearance.h"
#include "Events/GUIEventListener.h"
#include "../C3DManagers/ResourceManager.h"
class EditorScene;
#include <map>

class PteObject;
class TabbedPanel;

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
    bool            LoadTileSet(PteObject* pteObject);
    bool            CreateTileSet(unsigned int textureID);

    GUIPanel*       GetGuiComponent();
    void            SetVisible(bool visible);
    bool            IsVisible();
    const Tuple4i&  GetGuiBounds();
    
    const TextureTileInfo* GetTileInfo(const String& name);
  
  private:
    void            DrawFullScreenQuad(int width, int height);
    
    GUIButton*      CreateTileButton(const String& name, const Texture& texture, const Tuple4f& rectangle);
    GUIPanel*       CreateTilePage(const String& name);
    GUIPanel*       CreateTileRow(const String& name);
  
  protected:
    GUIPanel       *m_pGui;
    TabbedPanel    *m_pTilesetpanel;

    int             m_uiTilePageCount;

    std::map        <String, TextureTileInfo> m_TextureInfoList;
    ResourceManager <Texture, 20>             m_ManagedTextures;
};

#endif