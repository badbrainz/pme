#include "OptionsScene.h"
#include "../Kernel/Gateway.h"
#include "../Managers/SceneManager.h"
#include "../Stage.h"

OptionsScene::OptionsScene(const char* name) : Scene(name)
{
}

bool OptionsScene::initialize()
{
  return Scene::initialize();
}

void OptionsScene::beginScene()
{
  Scene::beginScene();
  glClearColor(0,0,0,1);
  SceneManager::getStage()->setCursor("FlechaMenu.tga");
}

void OptionsScene::actionPerformed(GUIEvent &evt)
{
  const String &callbackString  = evt.getCallbackString();
  GUIRectangle *sourceRectangle = evt.getEventSource();
  int           widgetType      = sourceRectangle->getWidgetType();
  
  //
  ///GUIButton
  //
  if (widgetType == BUTTON)
  {
    GUIButton *button = (GUIButton*) sourceRectangle;
    
    if (button->isClicked())
    {
      if (callbackString == "AcceptButton")
      {
        String item;
        
        GUIComboBox *combobox;
        GUISlider *slider;
        GUICheckBox* checkbox;
        
        combobox= (GUIComboBox*) gui.getWidgetByCallbackString("CellSizes");
        item = combobox->getSelectedItem();
        Gateway::setCellSize((item == "4") ? 4 : (item == "8") ? 8 : (item == "16") ? 16 : 8);
        
        combobox = (GUIComboBox*) gui.getWidgetByCallbackString("BranchSizes");
        item = combobox->getSelectedItem();
        Gateway::setBranchSize((item == "2") ? 2 : (item == "4") ? 4 : 2);
        
        slider = (GUISlider*) gui.getWidgetByCallbackString("CameraSpeed");
        Gateway::getConfiguration().camSpeed = slider->getProgress();
        
        slider = (GUISlider*) gui.getWidgetByCallbackString("FogDensitySlider");
        Gateway::getConfiguration().fogDensity = slider->getProgress() / 10.0f;
        
        slider = (GUISlider*) gui.getWidgetByCallbackString("FogRedSlider");
        Gateway::getConfiguration().fogColor.x = slider->getProgress();
        
        slider = (GUISlider*) gui.getWidgetByCallbackString("FogGreenSlider");
        Gateway::getConfiguration().fogColor.y = slider->getProgress();
        
        slider = (GUISlider*) gui.getWidgetByCallbackString("FogBlueSlider");
        Gateway::getConfiguration().fogColor.z = slider->getProgress();
        
        slider = (GUISlider*) gui.getWidgetByCallbackString("LogicRedSlider");
        Gateway::getConfiguration().logicColor.x = slider->getProgress();
        
        slider = (GUISlider*) gui.getWidgetByCallbackString("LogicGreenSlider");
        Gateway::getConfiguration().logicColor.y = slider->getProgress();
        
        slider = (GUISlider*) gui.getWidgetByCallbackString("LogicBlueSlider");
        Gateway::getConfiguration().logicColor.z = slider->getProgress();
        
        slider = (GUISlider*) gui.getWidgetByCallbackString("FlagRedSlider");
        Gateway::getConfiguration().flagColor.x = slider->getProgress();
        
        slider = (GUISlider*) gui.getWidgetByCallbackString("FlagGreenSlider");
        Gateway::getConfiguration().flagColor.y = slider->getProgress();
        
        slider = (GUISlider*) gui.getWidgetByCallbackString("FlagBlueSlider");
        Gateway::getConfiguration().flagColor.z = slider->getProgress();
        
        checkbox = (GUICheckBox*) gui.getWidgetByCallbackString("DensityCheckbox");
        Gateway::getConfiguration().enableFog = checkbox->isChecked();
        
        checkbox = (GUICheckBox*) gui.getWidgetByCallbackString("InfiniteViewCheckbox");
        Gateway::getConfiguration().infiniteView = checkbox->isChecked() ? 1000.0f : 0.0f;
        slider = (GUISlider*) gui.getWidgetByCallbackString("CameraRadius");
        Gateway::getConfiguration().camRadius = slider->getProgress() * 100.0f + Gateway::getConfiguration().infiniteView;
      }
      
      sceneController.execute(callbackString);
    }
  }
  
  //
  ///GUISlider
  //
  if (widgetType == SLIDER)
  {
    GUISlider *slider = (GUISlider*) sourceRectangle;
    
    if (slider->isPressed())
    {
      float cs = slider->getProgress();
      
      if (callbackString == "CameraSpeed")
      {
        slider->setLabelString(String("Speed: ") + cs + " * 100 units/s");
        return;
      }
      
      if (callbackString == "CameraRadius")
      {
        slider->setLabelString(String("View distance: ") + cs + " * 100 units");
        return;
      }
      
      if (callbackString == "FogDensitySlider")
      {
        slider->setLabelString(String("Density: f = 1/10^((") + cs + " * 0.1 * z)^2)");
        return;
      }
      
      if (callbackString == "FogRedSlider")
      {
        GUIButton* btn = (GUIButton*) gui.getWidgetByCallbackString("FogColorPreview");
        GUILabel* lbl = (GUILabel*) gui.getWidgetByCallbackString("FGRLabel");
        Tuple4f btncolor = btn->getColor();
        btn->setColor(cs, btncolor.y, btncolor.z);
        slider->setLabelString(String("Red: ") + cs);
        lbl->setLabelString(String("R: ") + cs);
        return;
      }
      
      if (callbackString == "FogGreenSlider")
      {
        GUIButton* btn = (GUIButton*) gui.getWidgetByCallbackString("FogColorPreview");
        GUILabel* lbl = (GUILabel*) gui.getWidgetByCallbackString("FGGLabel");
        Tuple4f btncolor = btn->getColor();
        btn->setColor(btncolor.x, cs, btncolor.z);
        slider->setLabelString(String("Green: ") + cs);
        lbl->setLabelString(String("G: ") + cs);
        return;
      }
      
      if (callbackString == "FogBlueSlider")
      {
        GUIButton* btn = (GUIButton*) gui.getWidgetByCallbackString("FogColorPreview");
        GUILabel* lbl = (GUILabel*) gui.getWidgetByCallbackString("FGBLabel");
        Tuple4f btncolor = btn->getColor();
        btn->setColor(btncolor.x, btncolor.y, cs);
        slider->setLabelString(String("Blue: ") + cs);
        lbl->setLabelString(String("B: ") + cs);
        return;
      }
      
      if (callbackString == "LogicRedSlider")
      {
        GUIButton* btn = (GUIButton*) gui.getWidgetByCallbackString("LogicColorPreview");
        GUILabel* lbl = (GUILabel*) gui.getWidgetByCallbackString("LRLabel");
        Tuple4f btncolor = btn->getColor();
        btn->setColor(cs, btncolor.y, btncolor.z);
        slider->setLabelString(String("Red: ") + cs);
        lbl->setLabelString(String("R: ") + cs);
        return;
      }
      
      if (callbackString == "LogicGreenSlider")
      {
        GUIButton* btn = (GUIButton*) gui.getWidgetByCallbackString("LogicColorPreview");
        GUILabel* lbl = (GUILabel*) gui.getWidgetByCallbackString("LGLabel");
        Tuple4f btncolor = btn->getColor();
        btn->setColor(btncolor.x, cs, btncolor.z);
        slider->setLabelString(String("Green: ") + cs);
        lbl->setLabelString(String("G: ") + cs);
        return;
      }
      
      if (callbackString == "LogicBlueSlider")
      {
        GUIButton* btn = (GUIButton*) gui.getWidgetByCallbackString("LogicColorPreview");
        GUILabel* lbl = (GUILabel*) gui.getWidgetByCallbackString("LBLabel");
        Tuple4f btncolor = btn->getColor();
        btn->setColor(btncolor.x, btncolor.y, cs);
        slider->setLabelString(String("Blue: ") + cs);
        lbl->setLabelString(String("B: ") + cs);
        return;
      }
      
      if (callbackString == "FlagRedSlider")
      {
        GUIButton* btn = (GUIButton*) gui.getWidgetByCallbackString("FlagColorPreview");
        GUILabel* lbl = (GUILabel*) gui.getWidgetByCallbackString("FLRLabel");
        Tuple4f btncolor = btn->getColor();
        btn->setColor(cs, btncolor.y, btncolor.z);
        slider->setLabelString(String("Red: ") + cs);
        lbl->setLabelString(String("R: ") + cs);
        return;
      }
      
      if (callbackString == "FlagGreenSlider")
      {
        GUIButton* btn = (GUIButton*) gui.getWidgetByCallbackString("FlagColorPreview");
        GUILabel* lbl = (GUILabel*) gui.getWidgetByCallbackString("FLGLabel");
        Tuple4f btncolor = btn->getColor();
        btn->setColor(btncolor.x, cs, btncolor.z);
        slider->setLabelString(String("Green: ") + cs);
        lbl->setLabelString(String("G: ") + cs);
        return;
      }
      
      if (callbackString == "FlagBlueSlider")
      {
        GUIButton* btn = (GUIButton*) gui.getWidgetByCallbackString("FlagColorPreview");
        GUILabel* lbl = (GUILabel*) gui.getWidgetByCallbackString("FLBLabel");
        Tuple4f btncolor = btn->getColor();
        btn->setColor(btncolor.x, btncolor.y, cs);
        slider->setLabelString(String("Blue: ") + cs);
        lbl->setLabelString(String("B: ") + cs);
        return;
      }
    }
  }
}

OptionsScene::~OptionsScene()
{}
