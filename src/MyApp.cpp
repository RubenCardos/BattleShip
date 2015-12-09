/*********************************************************************
 * Módulo 2. Curso de Experto en Desarrollo de Videojuegos
 * Autor: Carlos González Morcillo     Carlos.Gonzalez@uclm.es
 *
 * You can redistribute and/or modify this file under the terms of the
 * GNU General Public License ad published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * and later version. See <http://www.gnu.org/licenses/>.
 *
 * This file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.  
 *********************************************************************/

#include "MyApp.h" 
#include "CEGUI.h"

using namespace CEGUI;


MyApp::MyApp() {
  _sceneManager = NULL;
  _framelistener = NULL;
}

MyApp::~MyApp() {
}

int MyApp::start() {
  _root = new Ogre::Root();
  
  if(!_root->restoreConfig()) {
    _root->showConfigDialog();
    _root->saveConfig();
  }
  
  Ogre::RenderWindow* window = _root->initialise(true,"BattleShip");
  _sceneManager = _root->createSceneManager(Ogre::ST_GENERIC);
  
  //Camara
  Ogre::Camera* cam = _sceneManager->createCamera("MainCamera");
  cam->setPosition(Ogre::Vector3(0,155,305));//el tercer parametro hace que se aleje mas la camara, el segundo para que rote hacia arriba o hacia abajo
  cam->lookAt(Ogre::Vector3(0,67,0));//bajar el 60 un poco
  cam->setNearClipDistance(5);
  cam->setFarClipDistance(10000);
  
  //Pantalla OGRE
  Ogre::Viewport* viewport = window->addViewport(cam);
  viewport->setBackgroundColour(Ogre::ColourValue(0.0,0.0,0.0));
  double width = viewport->getActualWidth();
  double height = viewport->getActualHeight();
  cam->setAspectRatio(width / height);
  
  //Cargamos los recursos->carpeta Media etc..
  loadResources();
  //Creamos la escena 
  createScene();
  
  _framelistener = new MyFrameListener(window, cam, _sceneManager);
  _root->addFrameListener(_framelistener);
  
  //Creamos interfaces en CEGUI
  createGUI();

  _root->startRendering();

  return 0;
}

void MyApp::loadResources() {
  Ogre::ConfigFile cf;
  cf.load("resources.cfg");
  
  Ogre::ConfigFile::SectionIterator sI = cf.getSectionIterator();
  Ogre::String sectionstr, typestr, datastr;
  while (sI.hasMoreElements()) {
    sectionstr = sI.peekNextKey();
    Ogre::ConfigFile::SettingsMultiMap *settings = sI.getNext();
    Ogre::ConfigFile::SettingsMultiMap::iterator i;
    for (i = settings->begin(); i != settings->end(); ++i) {
      typestr = i->first;    datastr = i->second;
      Ogre::ResourceGroupManager::getSingleton().addResourceLocation
            (datastr, typestr, sectionstr);	
    }
  }
  Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void MyApp::createScene() {

  _sceneManager -> setAmbientLight ( Ogre :: ColourValue ( 0 ,  0 ,  0 ) ) ; 
  _sceneManager -> setShadowTechnique ( Ogre :: SHADOWTYPE_STENCIL_ADDITIVE ) ; 

  //PLANO 1 (Jugador)
  Ogre::Plane plane1(Ogre::Vector3::UNIT_Y, -5);
  Ogre::MeshManager::getSingleton().createPlane("plane1", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane1,
  200,200,1,1,true,1,1,1,Ogre::Vector3::UNIT_Z);

  Ogre::SceneNode* nodePlane1 = _sceneManager->createSceneNode("Tablero");
  Ogre::Entity* groundEnt1 = _sceneManager->createEntity("planeEnt1", "plane1");
  groundEnt1->setMaterialName("Ground"); //Material ground.material
  nodePlane1->attachObject(groundEnt1);
  
  //Luz del plano 1
  /*Ogre :: Luz :: LT_POINT - Esta Luz speads por igual en todas las direcciones a partir de un punto.
    Ogre :: Luz :: LT_SPOTLIGHT - Esta luz funciona como una linterna. Produce un cilindro sólido de luz que es más brillante en el centro y desvanece.
    Ogre :: Luz :: LT_DIRECTIONAL - Esta luz simula una enorme fuente que está muy lejos - al igual que la luz del día. Luz golpea toda la escena en el mismo ángulo en todas partes.*/
  Ogre::Light* light = _sceneManager->createLight("Light1");
  light->setType(Ogre::Light::LT_POINT);
  light -> setPosition (Ogre::Vector3(-10, 100, 100));
  light->setSpecularColour(Ogre::ColourValue::White);
  light->setCastShadows(false);

  //Agremos la luz al Plano 2 y lo posicionamos
  nodePlane1->attachObject(light);
  nodePlane1->setPosition(0,0,0);
  _sceneManager->getRootSceneNode()->addChild(nodePlane1);

  //PLANO 2 (Enemigo)
  Ogre::Plane plane2(Ogre::Vector3::UNIT_Y, -5);
  Ogre::MeshManager::getSingleton().createPlane("plane2",
  Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane2,
  200,200,1,1,true,1,1,1,Ogre::Vector3::UNIT_Z);

  Ogre::SceneNode* nodePlane2 = _sceneManager->createSceneNode("Tablero Enemigo");
  Ogre::Entity* groundEnt2 = _sceneManager->createEntity("planeEnt2", "plane2");
  groundEnt2->setCastShadows(false);
  groundEnt2->setMaterialName("Ground");
  nodePlane2->attachObject(groundEnt2);
  
  //Luz del plano 2
  /*
  Ogre::Light* light2 = _sceneManager->createLight("Light2");
  light2->setType(Ogre::Light::LT_POINT); 
  light2->setCastShadows(false);
  light2->setSpecularColour(Ogre::ColourValue::White);
  light2->setPosition (Ogre::Vector3(10, -100, -100));

  //Agremos la luz al Plano 2 y lo posicionamos
  nodePlane2->attachObject(light2);*/
  nodePlane2->setPosition(0,95,-95);
  nodePlane2->yaw(Ogre::Degree(90));
  nodePlane2->roll(Ogre::Degree(90));
  _sceneManager->getRootSceneNode()->addChild(nodePlane2);



  //Cuadraditos en Plano 2 (Enemigo)
  for(int i=0;i<9;i++){
    for(int j=0;j<9;j++){
      Ogre::Entity* entCube = _sceneManager->createEntity("Cube.mesh");
      entCube->setMaterialName("agua");
      Ogre::SceneNode* nodeCube = _sceneManager->createSceneNode(StringConverter::toString(j)+StringConverter::toString(i)+"NodeCube");
      nodeCube->attachObject(entCube);
      nodeCube->setPosition(87-22*j,-5,-88+22*i);
      nodeCube->setScale(8,1,8);
      nodePlane2->addChild(nodeCube);

      
      //Cuadraditos en Plano 1 (Jugador)
      Ogre::Entity* entCube1 = _sceneManager->createEntity("Cube.mesh");
      Ogre::SceneNode* nodeCube1 = _sceneManager->createSceneNode(StringConverter::toString(j)+StringConverter::toString(i)+"NodeCubeJugador");
      entCube1->setMaterialName("agua");
      nodeCube1->attachObject(entCube1);
      //nodeCube1->setPosition(-87+22*j,-5,88-22*i);
      nodeCube1->setPosition(-87+22*j,-5,-88+22*i);
      nodeCube1->setScale(8,1,8);
      nodePlane1->addChild(nodeCube1);
    }
  }



  //Agregamos la decoration de la escena
  MyApp::createDecorationLeft();
  MyApp::createDecorationRight();
  
}



void MyApp::createDecorationLeft(){

  //PLANO DECORACION IZQUIERDA ARRIBA 
  Ogre::Plane planeDecorationIzq1(Ogre::Vector3::UNIT_Y, -5);
  Ogre::MeshManager::getSingleton().createPlane("planeDecorationIzq1",
  Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, planeDecorationIzq1,
  200,200,1,1,true,1,1,1,Ogre::Vector3::UNIT_Z);

  Ogre::SceneNode* nodeDecorationIzq1=_sceneManager->createSceneNode("Tablero islaIzq1");
  Ogre::Entity* planeDecorationEntIzq1=_sceneManager->createEntity("planeDecorationEntIzq1","planeDecorationIzq1");
  planeDecorationEntIzq1->setMaterialName("isla"); // nombre del material
  nodeDecorationIzq1->attachObject(planeDecorationEntIzq1);
  nodeDecorationIzq1->setPosition(-200,95,-95);
  nodeDecorationIzq1->yaw(Ogre::Degree(90));
  nodeDecorationIzq1->roll(Ogre::Degree(90));
  _sceneManager->getRootSceneNode()->addChild(nodeDecorationIzq1);

  //PLANO DECORACION IZQUIERDA ABAJO
  Ogre::Plane planeDecorationIzq2(Ogre::Vector3::UNIT_Y, -5);
  Ogre::MeshManager::getSingleton().createPlane("planeDecorationIzq2", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, planeDecorationIzq2,
  200,200,1,1,true,1,1,1,Ogre::Vector3::UNIT_Z);
 
  Ogre::SceneNode* nodeDecorationIzq2 = _sceneManager->createSceneNode("Tablero arenaIzq2");
  Ogre::Entity* planeDecorationEntIzq2 = _sceneManager->createEntity("planeDecorationEntIzq2", "planeDecorationIzq2");
  planeDecorationEntIzq2->setMaterialName("arena"); //Material ground.material
  nodeDecorationIzq2->attachObject(planeDecorationEntIzq2);
  nodeDecorationIzq2->setPosition(-200,0,0);
  _sceneManager->getRootSceneNode()->addChild(nodeDecorationIzq2);


  //AÑADIMOS PALMERA
  Ogre::Entity* entPalmIzq = _sceneManager->createEntity("palmera.mesh");
  Ogre::SceneNode* nodePalmIzq = _sceneManager->createSceneNode("palmera2");
  nodePalmIzq->attachObject(entPalmIzq);
  nodePalmIzq->setScale(15,15,15);
  nodePalmIzq->setPosition(60,0,30);
  nodePalmIzq->yaw(Degree(70));
  nodeDecorationIzq2->addChild(nodePalmIzq);

  
}


void MyApp::createDecorationRight(){

  //PLANO DECORACION DERECHA ARRIBA 
  Ogre::Plane planeDecoration(Ogre::Vector3::UNIT_Y, -5);
  Ogre::MeshManager::getSingleton().createPlane("planeDecoration",
  Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, planeDecoration,
  200,200,1,1,true,1,1,1,Ogre::Vector3::UNIT_Z);

  Ogre::SceneNode* nodeDecoration=_sceneManager->createSceneNode("Tablero isla");
  Ogre::Entity* planeDecorationEnt=_sceneManager->createEntity("planeDecorationEnt","planeDecoration");
  planeDecorationEnt->setMaterialName("isla"); // nombre del material
  nodeDecoration->attachObject(planeDecorationEnt);
  nodeDecoration->setPosition(200,95,-95);
  nodeDecoration->yaw(Ogre::Degree(90));
  nodeDecoration->roll(Ogre::Degree(90));
  _sceneManager->getRootSceneNode()->addChild(nodeDecoration);

  //PLANO DECORACION DERECHA ABAJO
  Ogre::Plane planeDecoration2(Ogre::Vector3::UNIT_Y, -5);
  Ogre::MeshManager::getSingleton().createPlane("planeDecoration2", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, planeDecoration2,
  200,200,1,1,true,1,1,1,Ogre::Vector3::UNIT_Z);
 
  Ogre::SceneNode* nodeDecoration2 = _sceneManager->createSceneNode("Tablero arena");
  Ogre::Entity* planeDecorationEnt2 = _sceneManager->createEntity("planeDecorationEnt2", "planeDecoration2");
  planeDecorationEnt2->setMaterialName("arena"); //Material ground.material
  nodeDecoration2->attachObject(planeDecorationEnt2);
  nodeDecoration2->setPosition(200,0,0);
  _sceneManager->getRootSceneNode()->addChild(nodeDecoration2);

  //AÑADIMOS PALMERA
  Ogre::Entity* entPalm = _sceneManager->createEntity("palmera.mesh");
  Ogre::SceneNode* nodePalm = _sceneManager->createSceneNode("palmera");
  nodePalm->attachObject(entPalm);
  nodePalm->setScale(17,17,17);
  nodePalm->setPosition(-55,0,60);
  nodePalm->yaw(Degree(20));
  nodeDecoration2->addChild(nodePalm);

  Ogre::Light* lightPalm = _sceneManager->createLight();
  lightPalm->setType(Ogre::Light::LT_DIRECTIONAL);
  lightPalm->setSpecularColour(Ogre::ColourValue::White);
  lightPalm->setDirection(Ogre::Vector3(1,-1,0));
  nodePalm->attachObject(lightPalm);

}


void MyApp::createGUI()
{
  //CEGUI
  renderer = &OgreRenderer::bootstrapSystem();
  Scheme::setDefaultResourceGroup("Schemes");
  ImageManager::setImagesetDefaultResourceGroup("Imagesets");
  Font::setDefaultResourceGroup("Fonts");
  WindowManager::setDefaultResourceGroup("Layouts");
  WidgetLookManager::setDefaultResourceGroup("LookNFeel");
  SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
  System::getSingleton().getDefaultGUIContext().setDefaultFont("PirateOne-12");
  System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");
  FontManager::getSingleton().createAll("*.font", "Fonts");

  //Cargo la imagen
  ImageManager::getSingleton().addFromImageFile("BackgroundImage","battleshipAgua.jpg");

  //Sheet
  Window* sheet =  WindowManager::getSingleton().createWindow("TaharezLook/StaticImage","background_wnd");
  sheet->setPosition( UVector2(cegui_reldim(0),cegui_reldim(0)));
  sheet->setSize( USize(cegui_reldim(1),cegui_reldim(1)));
  sheet->setProperty("Image","BackgroundImage");
  sheet->setProperty("FrameEnabled","False");

  //Config Window
  Window* configWin =  WindowManager::getSingleton().loadLayoutFromFile("inicio.layout");
  
  Window* playButton = configWin->getChild("playButton");
  playButton->subscribeEvent( PushButton::EventClicked,Event::Subscriber(&MyFrameListener::play,_framelistener));
  playButton->setXPosition(UDim(0.40f, 0.0f));
  playButton->setYPosition(UDim(0.60f, 0.0f));
  playButton->setText("[font='PirateOne-30'] Play ");

  Window* creditButton = configWin->getChild("creditButton");
  creditButton->subscribeEvent( PushButton::EventClicked,Event::Subscriber(&MyFrameListener::showCredits,_framelistener));
  creditButton->setXPosition(UDim(0.40f, 0.0f));
  creditButton->setYPosition(UDim(0.80f, 0.0f));

  
  CEGUI::ImageManager::getSingleton().addFromImageFile("BackgroundImageCredits","creditos.jpg");
  CEGUI::Window* sheetCredits = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage","creditFrame");
  sheetCredits->setPosition( UVector2(cegui_reldim(0),cegui_reldim(0)));
  sheetCredits->setSize( USize(cegui_reldim(1),cegui_reldim(1)));
  sheetCredits->setProperty("Image","BackgroundImageCredits");

  //Nuevo Layout
  CEGUI::Window* configWinCredits = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("creditos.layout");
  
  sheetCredits->setVisible(false);
  sheetCredits->addChild(configWinCredits);

  //Attaching buttons
  sheet->addChild(configWin);
  sheet->addChild(sheetCredits);
  System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);
  
}
