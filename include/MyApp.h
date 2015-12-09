#include <Ogre.h>
#include "MyFrameListener.h"
#include <CEGUI.h>
#include <RendererModules/Ogre/Renderer.h>
#include <string>

class MyApp {
  
private:
  Ogre::SceneManager* _sceneManager;
  Ogre::Root* _root;
  CEGUI::OgreRenderer* renderer; 
  MyFrameListener* _framelistener;
  
public:
  MyApp();
  ~MyApp();  
  int start();
  void loadResources();
  void createScene();
  void createGUI();
  void initUI();
  void createDecorationLeft();
  void createDecorationRight();

};
