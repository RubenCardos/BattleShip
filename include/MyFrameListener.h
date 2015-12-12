#include <Ogre.h>
#include <OIS/OIS.h>
#include <CEGUI.h>
#include <string>

using namespace Ogre;
using namespace std;

#define CLICK 1 << 0  // Mascara para objetos clickables


class MyFrameListener : public Ogre::FrameListener, OIS::KeyListener, OIS::MouseListener {
private:
  OIS::InputManager* _inputManager;
  OIS::Keyboard* _keyboard;
  OIS::Mouse* _mouse;
  Ogre::Camera* _camera;
  RenderWindow* _win;
  SceneManager* _sceneManager;
  RaySceneQuery *_raySceneQuery;
  SceneNode *_selectedNode;

  bool keyPressed(const OIS::KeyEvent& evt);
  bool keyReleased(const OIS::KeyEvent& evt);
  bool mouseMoved(const OIS::MouseEvent& evt);
  bool mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
  bool mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
  void setNextShip(string barco);



  bool _quit;
  float _timeSinceLastFrame;

  bool _records;

  CEGUI::MouseButton convertMouseButton(OIS::MouseButtonID id);

  Ray setRayQuery(int posx, int posy, uint32 mask);

  string readRecords();
  void updateRecords(int puntuacion);


public:
  MyFrameListener(Ogre::RenderWindow* win, Ogre::Camera* cam,SceneManager* sm);
  ~MyFrameListener();
  bool frameStarted(const Ogre::FrameEvent& evt);  
  bool quit(const CEGUI::EventArgs &e);
  bool play(const CEGUI::EventArgs &e);
  bool next(const CEGUI::EventArgs &e);
  bool restart(const CEGUI::EventArgs &e);
  bool nextShip(const CEGUI::EventArgs &e);
  bool showRecords(const CEGUI::EventArgs &e);
  bool showControls(const CEGUI::EventArgs &e);
  bool showCredits(const CEGUI::EventArgs &e);
  void resetGame();

  void MoverBarco(string Barco,OIS::Keyboard* _keyboard,SceneManager* _sceneManager,int limSup,int limInf,int limDer,int limIzq,int desp);
};
