#include "NewMapScene.h"
#include "../C3DDatabase/TerrainDatabase.h"

NewMapScene::NewMapScene(const String &name) : m_MapNames(5), m_MapDescriptorList(20), Scene(name)
{
  currentItem = 0;
}

bool NewMapScene::Initialize()
{
  Scene::Initialize();
  
  m_pButtonSurface = (GUIButton*) m_Gui.getWidgetByCallbackString("PreviewSurface");
  
  LoadMapNames();
  
  ShowMap(0);

  return true;
}

bool NewMapScene::LoadMapNames()
{
  XMLElement  *root = 0, *child = 0, *subchild = 0;
  XMLStack     xmlStack;
  
  const char *verified = MediaPathManager::lookUpMediaPath("Mapas.xml");
  
  if (!verified)
    return Logger::writeErrorLog(String("Couldn't locate the <Mapas> XML file"));
  
  //load the entire xml tree
  if (!xmlStack.loadXMLFile(verified))
    return Logger::writeErrorLog(String("Invalid XML file -> ") + verified);
  
  //now browse through the loaded tree
  if (root = xmlStack.getChildByName("Mapas"))
  {
    for (unsigned int i = 0; i < root->getChildrenCount(); i++)
    {
      if (!(child = root->getChild(i)))
        continue;
      
      MapDescriptor descriptor;
      
      descriptor.mapName      = child->getValuec();
      
      subchild = child->getChildByName("Visual");
      descriptor.mapVisual    = subchild->getChildByName("name")->getValuec();
      
      subchild = child->getChildByName("Minimap");
      descriptor.mapMinimap   = subchild->getChildByName("name")->getValuec();
      
      subchild = child->getChildByName("Texturas");
      descriptor.mapTextures  = subchild->getChildByName("name")->getValuec();
      
      m_MapDescriptorList.append(descriptor);
    }
  }
  
  return true;
}

void NewMapScene::ShowMap(unsigned int index)
{
  texture.load2D(m_MapDescriptorList[index].mapMinimap);
  m_pButtonSurface->setTexture(texture);
  GUILabel* label = (GUILabel*) m_Gui.getWidgetByCallbackString("PreviewMapName");
  label->setLabelString(m_MapDescriptorList[index].mapName);
}

void NewMapScene::actionPerformed(GUIEvent &evt)
{
  const String &callbackString  = evt.getCallbackString();
  GUIRectangle *sourceRectangle = evt.getEventSource();
  int           widgetType      = sourceRectangle->getWidgetType();

  if (widgetType == BUTTON)
  {
    GUIButton *button = (GUIButton*) sourceRectangle;
		
		if (button->isClicked())
    {
      if (callbackString == "PreviewLeft")
      {
        currentItem = clamp(--currentItem, 0, (int)m_MapDescriptorList.length()-1);
        ShowMap(currentItem);
        return;
      }
      
      if (callbackString == "PreviewRight")
      {
        currentItem = clamp(++currentItem, 0, (int)m_MapDescriptorList.length()-1);
        ShowMap(currentItem);
        return;
      }
      
      if (callbackString == "Editor")
      {
        m_pTerrainDatabase->LoadGameData(m_MapDescriptorList[currentItem]);
      }
      
      m_SceneController.Execute(callbackString);
    }
  }
}

NewMapScene::~NewMapScene()
{
}