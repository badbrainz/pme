#ifndef NEWMAPSCENE
#define NEWMAPSCENE

#include "Scene.h"
#include "../OC/ocarray.h"
#include "Appearance/Appearance.h"

struct MapDescriptor
{
  String mapName;
  String mapVisual;
  String mapMinimap;
  String mapTextures;
};

class TerrainDatabase;

class NewMapScene : public Scene
{
  public:
  	 NewMapScene(const String &name = "NewMapScene");
  	~NewMapScene();
    
    bool  Initialize();
    void  actionPerformed(GUIEvent &evt);
    
    void SetTerrainDatabase(TerrainDatabase* database){m_pTerrainDatabase = database;}
  
  private:
    bool LoadMapNames();
    void ShowMap(unsigned int index);
  
  private:
    GUIButton* m_pButtonSurface;
    
    Array <String>        m_MapNames;
    Array <MapDescriptor> m_MapDescriptorList;
    
    TerrainDatabase *m_pTerrainDatabase;
    Texture texture;
    int currentItem;
};

#endif