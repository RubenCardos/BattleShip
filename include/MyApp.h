#include <Ogre.h>
#include "MyFrameListener.h"
#include <CEGUI.h>
#include <RendererModules/Ogre/Renderer.h>
#include <string>
#include "TrackManager.h"
#include "SoundFXManager.h"

class MyApp {
  
private:
  Ogre::SceneManager* _sceneManager;
  Ogre::Root* _root;
  CEGUI::OgreRenderer* renderer; 
  MyFrameListener* _framelistener;

  // Manejadores del sonido.
  TrackManager* _pTrackManager;
  SoundFXManager* _pSoundFXManager;
  TrackPtr _mainTrack;
  SoundFXPtr _simpleEffect;
  bool _initSDL();
  
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
