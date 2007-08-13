#ifndef STAGE
#define STAGE

#include "Events/InputEventListener.h"
#include "Tools/Benchmark.h"
#include "Window/Window.h"

class Stage : public InputEventListener, public IOXMLObject
{
  public:
     Stage(HINSTANCE instance = 0);
    ~Stage();
  
  public:
		bool loadXMLSettings(XMLElement *element);
    bool exportXMLSettings(ofstream &xmlFile){return true;}
    
    void windowPositionChanged(WindowEvent evt);
    void windowSizeChanged    (WindowEvent evt);
    void windowMaximized      (WindowEvent evt);
    void windowMinimized      (WindowEvent evt);
    void windowClosing        (WindowEvent evt);
    
    void mouseDoubleClicked(MouseEvent evt){}
    void mouseScrolled     (MouseEvent evt){}
    void mouseReleased     (MouseEvent evt);
    void mouseClicked      (MouseEvent evt);
    void mouseDragged      (MouseEvent evt);
    void mouseMoved        (MouseEvent evt);
    void mouseExit         (MouseEvent evt){}
    
    void keyReleased(KeyEvent evt);
    void keyPressed (KeyEvent evt);
  
  public:
		bool Initialize();
    void Destroy();
		
		void SetActive(bool state);
    bool IsActive();
    bool Run();
		
    void ForceUpdate();
  
  private:
		Benchmark m_Benchmark;
		Window    m_Window;
		bool      m_bActiveState;
};

#endif