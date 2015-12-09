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
#include "MyFrameListener.h"
#include "Tablero.h"
#include "EntityMaterialInstance.h"

using namespace std;
//Estados

enum States {
  start,
  posicionando1,
  posicionando2,
  posicionando3,
  posicionando4,
  posicionando5,
  posicionando6,
  jugando,
  jugandoEnemy,
  gameOver
};

States state;
bool changeState;
bool showInfo=true;//Mostrar info
bool canRotate=true;//El barco puede Rotar(para la colocacion)

//Mostrar creditos
bool showCreditsFrame=true;

//Para posicionar los barcos
//Una variable para detectar cada boton
bool UPkeyDown=true;//Para pulsacion
bool DOWNkeyDown=true;//Para pulsacion
bool LEFTkeyDown=true;//Para pulsacion
bool RIGHTkeyDown=true;//Para pulsacion
bool ENTERkeyDown=true;//Para pulsacion
bool shipLocated=false;//Para ver si ya hemos colocado el barco
int rotation=1;//rotaciones de los barcos

//Para detectar click
bool mouseClick=true;

//Para la posicion de cada barco en cada momento
int i;
int j;

//Tablero Enemigo
Tablero boardEnemy;

//Tablero Jugador
Tablero boardPlayer;

//Para transparencia
EntityMaterialInstance *emi;

MyFrameListener::MyFrameListener(Ogre::RenderWindow* win,Ogre::Camera* cam, SceneManager* sm) {

  OIS::ParamList param;
  size_t windowHandle;  std::ostringstream wHandleStr;
  _sceneManager = sm; _win = win;
  _camera = cam;  
  _quit = false;
  
  win->getCustomAttribute("WINDOW", &windowHandle);
  wHandleStr << windowHandle;
  param.insert(std::make_pair("WINDOW", wHandleStr.str()));
  
  _inputManager = OIS::InputManager::createInputSystem(param);
  _keyboard = static_cast<OIS::Keyboard*>
    (_inputManager->createInputObject(OIS::OISKeyboard, true));
  _mouse = static_cast<OIS::Mouse*>
    (_inputManager->createInputObject(OIS::OISMouse, true));

  _keyboard->setEventCallback(this);
  _mouse->setEventCallback(this);

  //Estado
  state = start;
  changeState = true;

  //RayQuery--Ver objeto que se pulsa
  _raySceneQuery = _sceneManager->createRayQuery(Ray());
  _selectedNode = NULL;

  //Tableros
  boardEnemy.generateRandomBoard();//al generarse ya se pinta

}

MyFrameListener::~MyFrameListener() {
  //  _inputManager->destroyInputObject(_keyboard);
  //  _inputManager->destroyInputObject(_mouse);
  //  OIS::InputManager::destroyInputSystem(_inputManager);
}

bool MyFrameListener::frameStarted(const Ogre::FrameEvent& evt) {

  _timeSinceLastFrame = evt.timeSinceLastFrame;
 
  CEGUI::System::getSingleton().getDefaultGUIContext().injectTimePulse(_timeSinceLastFrame);

  _mouse->capture();
  _keyboard->capture();
  
  bool mbleft, mbmiddle, mbright; // Botones del raton pulsados
  
  //Posicion EN PIXELES EN LA PANTALLA, NO DEL SRU
  int posx = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition().d_x;   // Posicion del puntero
  int posy = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition().d_y;   //  en pixeles.
  uint32 mask;
  // Botones del raton pulsados? -------------------------------------
  mbleft = _mouse->getMouseState().buttonDown(OIS::MB_Left);
  mbmiddle = _mouse->getMouseState().buttonDown(OIS::MB_Middle);
  mbright = _mouse->getMouseState().buttonDown(OIS::MB_Right);

  //INFORMACION (pulsado la i)

  if(_keyboard->isKeyDown(OIS::KC_I) && showInfo==true){
    showInfo=false;
    cout << "----------INFO---------- \n" <<  endl;
    cout << "Estado: " << state << endl;
    cout << "------SCENE MANAGER----- \n" <<  endl;
    cout <<"Root "<<_sceneManager->getRootSceneNode()->getName() << endl;
    Ogre::SceneNode::ChildNodeIterator it = _sceneManager->getRootSceneNode()->getChildIterator();
    while ( it.hasMoreElements() ) {//Hijos del Root
      SceneNode* child = static_cast<SceneNode*>(it.getNext());
      cout <<"Nodo: "<<child->getName() << endl;
      Ogre::SceneNode::ChildNodeIterator it2 = child->getChildIterator();
      while ( it2.hasMoreElements() ) {//Hijos de cada hijo (solo tenemos dos niveles)
        SceneNode* child2 = static_cast<SceneNode*>(it2.getNext());
        cout <<"---Nodo: "<<child2->getName() << endl;
    }
   }

    cout << "------------------------ \n" <<  endl;
  }

  if(_keyboard->isKeyDown(OIS::KC_I) == false){
    showInfo=true;
  }

  switch(state) {
    

    case posicionando1://Un estado por cada barco, Barco1
      
      

      break;

    case posicionando2:

      
      //Si no he colocado el barco
      if(shipLocated==false){
        //Si no esta colocado lo puedo mover
        MoverBarco("Barco",_keyboard,_sceneManager,88,88,88,88,22);//Este va bien
        //Si pulso enter coloco el barco
        if(_keyboard->isKeyDown(OIS::KC_RETURN) && ENTERkeyDown==true){
          ENTERkeyDown=false;
          if(boardPlayer.checkPosition(i,j)){
            cout << "Posicion correcta" << endl;
            shipLocated=true;
            boardPlayer.setBox(i,j,1);
            boardPlayer.printBoard();
          }
        }
        if(_keyboard->isKeyDown(OIS::KC_RETURN) == false){
          ENTERkeyDown=true;
        }  
      }else{
        //Cambio el texto de la interfaz
        CEGUI::Window* Auxsheet=CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
        CEGUI::Window* AuxconfigWin2=Auxsheet->getChildAtIdx(0);
        AuxconfigWin2->getChild("Text1")->setText("Barco colocado, pulsa siguiente");
      }
      
      break;

    case posicionando3:
      // Puede que izquierda y derecha esten invertidas
      //int limSup,int limInf,int limDer,int limIzq,int desp
      //MoverBarco("Barco2",_keyboard,_sceneManager,88,88,88,88,22);
    //Si no he colocado el barco
      if(shipLocated==false){
        //Si no esta colocado lo puedo mover
        MoverBarco("Barco2",_keyboard,_sceneManager,88,88,88,88,22);//Este va bien
        //Si pulso enter coloco el barco
        if(_keyboard->isKeyDown(OIS::KC_RETURN) && ENTERkeyDown==true){
          ENTERkeyDown=false;
          if(rotation==1 || rotation==3){//horizontal
            if(boardPlayer.checkPosition(i,j)&&boardPlayer.checkPosition(i,j+1) && boardPlayer.checkPosition(i,j-1)){//compruebo ij, j+1,j-1,
              cout << "Posicion correcta" << endl;
              shipLocated=true;
              //pongo todas las posiciones
              boardPlayer.setBox(i,j,2);
              boardPlayer.setBox(i,j+1,2);
              boardPlayer.setBox(i,j-1,2);
              boardPlayer.printBoard();
            }else{
              //Cambio el texto de la interfaz
              CEGUI::Window* Auxsheet=CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
              CEGUI::Window* AuxconfigWin2=Auxsheet->getChildAtIdx(0);
              AuxconfigWin2->getChild("Text1")->setText("Posicion incorrecta");
            }
          }if(rotation==2 || rotation==4){//vertical
            if(boardPlayer.checkPosition(i,j)&&boardPlayer.checkPosition(i+1,j) && boardPlayer.checkPosition(i-1,j)){//compruebo ij, i+1,i-1,
              cout << "Posicion correcta" << endl;
              shipLocated=true;
              //pongo todas las posiciones
              boardPlayer.setBox(i,j,2);
              boardPlayer.setBox(i-1,j,2);
              boardPlayer.setBox(i+1,j,2);
              boardPlayer.printBoard();
            }else{
              //Cambio el texto de la interfaz
              CEGUI::Window* Auxsheet=CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
              CEGUI::Window* AuxconfigWin2=Auxsheet->getChildAtIdx(0);
              AuxconfigWin2->getChild("Text1")->setText("Posicion incorrecta");
            }
          }
        }
        if(_keyboard->isKeyDown(OIS::KC_RETURN) == false){
          ENTERkeyDown=true;
        }  
      }else{
        //Cambio el texto de la interfaz
        CEGUI::Window* Auxsheet=CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
        CEGUI::Window* AuxconfigWin2=Auxsheet->getChildAtIdx(0);
        AuxconfigWin2->getChild("Text1")->setText("Barco colocado, pulsa siguiente");
      }
        
      break;

    case posicionando4:

      //MoverBarco("Barco3",_keyboard,_sceneManager,88,88,66,66,22);//Desplazamiento raro
      //Si no he colocado el barco
      if(shipLocated==false){
        //Si no esta colocado lo puedo mover
        MoverBarco("Barco3",_keyboard,_sceneManager,88,88,88,88,22);//Este va bien
        //Si pulso enter coloco el barco
        if(_keyboard->isKeyDown(OIS::KC_RETURN) && ENTERkeyDown==true){
          ENTERkeyDown=false;
          if(rotation==1 || rotation==3){//horizontal
            if(boardPlayer.checkPosition(i,j)&&boardPlayer.checkPosition(i,j+1) && boardPlayer.checkPosition(i,j-1)){//compruebo ij, j+1,j-1,
              cout << "Posicion correcta" << endl;
              shipLocated=true;
              //pongo todas las posiciones
              boardPlayer.setBox(i,j,3);
              boardPlayer.setBox(i,j+1,3);
              boardPlayer.setBox(i,j-1,3);
              boardPlayer.printBoard();
            }else{
              //Cambio el texto de la interfaz
              CEGUI::Window* Auxsheet=CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
              CEGUI::Window* AuxconfigWin2=Auxsheet->getChildAtIdx(0);
              AuxconfigWin2->getChild("Text1")->setText("Posicion incorrecta");
            }
          }if(rotation==2 || rotation==4){//vertical
            if(boardPlayer.checkPosition(i,j)&&boardPlayer.checkPosition(i+1,j) && boardPlayer.checkPosition(i-1,j)){//compruebo ij, i+1,i-1,
              cout << "Posicion correcta" << endl;
              shipLocated=true;
              //pongo todas las posiciones
              boardPlayer.setBox(i,j,3);
              boardPlayer.setBox(i-1,j,3);
              boardPlayer.setBox(i+1,j,3);
              boardPlayer.printBoard();
            }else{
              //Cambio el texto de la interfaz
              CEGUI::Window* Auxsheet=CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
              CEGUI::Window* AuxconfigWin2=Auxsheet->getChildAtIdx(0);
              AuxconfigWin2->getChild("Text1")->setText("Posicion incorrecta");
            }
          }
        }
        if(_keyboard->isKeyDown(OIS::KC_RETURN) == false){
          ENTERkeyDown=true;
        }  
      }else{
        //Cambio el texto de la interfaz
        CEGUI::Window* Auxsheet=CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
        CEGUI::Window* AuxconfigWin2=Auxsheet->getChildAtIdx(0);
        AuxconfigWin2->getChild("Text1")->setText("Barco colocado, pulsa siguiente");
      }  
      break;

    case posicionando5:

      //MoverBarco("Barco4",_keyboard,_sceneManager,88,88,88,44,22);//Desplazamiento raro
      //Si no he colocado el barco
      if(shipLocated==false){
        //Si no esta colocado lo puedo mover
        MoverBarco("Barco4",_keyboard,_sceneManager,88,88,88,88,22);//Este va bien
        //Si pulso enter coloco el barco
        if(_keyboard->isKeyDown(OIS::KC_RETURN) && ENTERkeyDown==true){
          ENTERkeyDown=false;
          if(boardEnemy.checkPosition(i,j)){
            cout << "Posicion correcta" << endl;
            shipLocated=true;
            boardPlayer.setBox(i,j,4);
            boardPlayer.printBoard();
          }
        }
        if(_keyboard->isKeyDown(OIS::KC_RETURN) == false){
          ENTERkeyDown=true;
        }  
      }else{
        //Cambio el texto de la interfaz
        CEGUI::Window* Auxsheet=CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
        CEGUI::Window* AuxconfigWin2=Auxsheet->getChildAtIdx(0);
        AuxconfigWin2->getChild("Text1")->setText("Barco colocado, pulsa siguiente");
      }  
      break;

    case posicionando6:

      break;      

    case jugando:
          
      //cout << "Player: Algun barco vivo? " << boardPlayer.anyShipAlive() << endl;

      if (mbleft && mouseClick == true) {//Si pulso el boton izquierdo vemos donde pincho
        
        mouseClick=false;

        if (_selectedNode != NULL) {  // Si habia alguno seleccionado...
          _selectedNode->showBoundingBox(false);  _selectedNode = NULL;  
        }

        //MASCARA¿?
        mask = CLICK;  // Podemos elegir todo ¿?
        setRayQuery(posx,posy,mask);
        RaySceneQueryResult &result = _raySceneQuery->execute();
        RaySceneQueryResult::iterator it; //iterator para ver resultados
        it = result.begin();
        cout << "----------INFO---------- \n" <<  endl;
        cout << "Estado: " << state << endl;
        cout << "--RaySceneQueryResult--- \n" <<  endl;
        if (it != result.end()) {//si no hay nada no hago nada
          for (it = result.begin(); it != result.end(); it++){
            if (it->movable){
              SceneNode* aux =static_cast<SceneNode*>(it->movable->getParentSceneNode());
              cout << "It: " << aux->getName() << endl;
              //solo quiero quedarme con los nodos "Cubeij"
              if(Ogre::StringUtil::endsWith(aux->getName(),"NodeCube",true)){
                cout << "---Cubeij: " << aux->getName() << endl;
                Entity* auxEnt = static_cast<Entity*>(aux->getAttachedObject(0));
                auxEnt->setVisible(true);
                //Tengo que coger lo dos primeros caracteres coor[0] tiene dos numeros
                string coor = Ogre::StringUtil::split(aux->getName(), "NodeCube")[0];
                int x=Ogre::StringConverter::parseInt(coor.substr(0,1));
                int y=Ogre::StringConverter::parseInt(coor.substr(1,2));
                cout << "Tablero["<< x <<"]["<< y <<"]="<< boardEnemy.checkBox(x,y)<<endl;
                if(boardEnemy.checkBox(x,y)!=9){
                  if(boardEnemy.checkBox(x,y)!=0){
                    auxEnt->setMaterialName("cross");
                    boardEnemy.setBox(x,y,9);
                    //Cambio el texto de la interfaz
                    CEGUI::Window* Auxsheet=CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
                    CEGUI::Window* AuxconfigWin2=Auxsheet->getChildAtIdx(0);
                    AuxconfigWin2->getChild("Text1")->setText("Impacto!");
                    //boardEnemy.hitShip(boardEnemy.checkBox(x,y)-1);  
                  }else{
                    auxEnt->setMaterialName("fail");
                    boardEnemy.setBox(x,y,9);
                    //Cambio el texto de la interfaz
                    CEGUI::Window* Auxsheet=CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
                    CEGUI::Window* AuxconfigWin2=Auxsheet->getChildAtIdx(0);
                    AuxconfigWin2->getChild("Text1")->setText("Agua!");
                  }
                  //Paso al turno del enemigo
                  state=jugandoEnemy;
                }else{
                  //Cambio el texto de la interfaz
                  CEGUI::Window* Auxsheet=CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
                  CEGUI::Window* AuxconfigWin2=Auxsheet->getChildAtIdx(0);
                  AuxconfigWin2->getChild("Text1")->setText("Casilla ya pulsada, pulsa otra");
                } 
              }
            }
          }
        }
        cout << "------------------------ \n" <<  endl;
      }
      if (mbleft ==false) {
        mouseClick=true;
      }
        
      break;

    case jugandoEnemy:
      cout << "Enemigo: Algun barco vivo? " << boardEnemy.anyShipAlive() << endl;
      if(boardEnemy.anyShipAlive()==true){
        cout << "Si!" << endl;
      }else{//pierde el enemigo (RECORDS?)
        cout << "Flota Destruida" << endl;
        //Aqui pantalla game over (Gana el jugador)
      }
      srand((unsigned)time(0));
      int x=rand()%9; //genero de 0 a 9 para no salirse de los limites
      int y=rand()%9;
     
      Ogre::SceneNode::ChildNodeIterator it = _sceneManager->getRootSceneNode()->getChild("Tablero")->getChildIterator();
    	while ( it.hasMoreElements() ) {
	      SceneNode* child = static_cast<SceneNode*>(it.getNext());
	      //cout <<"Tablero jugador Nodo: "<<child->getName() << endl;
	      if(Ogre::StringUtil::endsWith(child->getName(),"NodeCubeJugador",true)){
	      	Entity* auxEnt = static_cast<Entity*>(child->getAttachedObject(0));
		      string coor = Ogre::StringUtil::split(child->getName(), "NodeCubeJugador")[0];
	        int i=Ogre::StringConverter::parseInt(coor.substr(0,1));
	        int j=Ogre::StringConverter::parseInt(coor.substr(1,2));
	        if(x==j && y==i){//que sea la casilla que a la que se e ha disparado
	      			if(boardPlayer.checkBox(x,y)!=9){ 	
		            if(boardPlayer.checkBox(x,y)!=0){
		              auxEnt->setMaterialName("cross");  
		            }else{
		              auxEnt->setMaterialName("fail");
		            }
          	}
        	}

      	}
	      
	    }
      if(boardPlayer.checkBox(x,y)==9){//Casilla ya pulsada
        cout << "Enemigo: Casilla ya disparada " <<endl;
      }else{//puedo disparar hay
        if(boardPlayer.checkBox(x,y)!=0){//impacto
          //Cambio el texto de la interfaz
          CEGUI::Window* Auxsheet=CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
          CEGUI::Window* AuxconfigWin2=Auxsheet->getChildAtIdx(0);
          AuxconfigWin2->getChild("Text1")->setText("Enemigo Impacto!");
          boardPlayer.setBox(x,y,9);
        }else{//Agua
          //Cambio el texto de la interfaz
          CEGUI::Window* Auxsheet=CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
          CEGUI::Window* AuxconfigWin2=Auxsheet->getChildAtIdx(0);
          AuxconfigWin2->getChild("Text1")->setText("Enemigo Agua!");
          boardPlayer.setBox(x,y,9);
        }
        //Paso al turno del jugador
        state=jugando;
        boardPlayer.printBoard();
      }

      break;
    
  }

  if(_quit) return false;

  return true;
}

bool MyFrameListener::keyPressed(const OIS::KeyEvent& evt)
{
  

  if(evt.key==OIS::KC_ESCAPE) return _quit=true;
 
  CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown(static_cast<CEGUI::Key::Scan>(evt.key));
  CEGUI::System::getSingleton().getDefaultGUIContext().injectChar(evt.text);

  return true;
}

bool MyFrameListener::keyReleased(const OIS::KeyEvent& evt)
{
  CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp(static_cast<CEGUI::Key::Scan>(evt.key));
  return true;
}

bool MyFrameListener::mouseMoved(const OIS::MouseEvent& evt)
{
  CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseMove(evt.state.X.rel, evt.state.Y.rel);  
  return true;
}

bool MyFrameListener::mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
{
  CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(convertMouseButton(id));
  return true;
}

bool MyFrameListener::mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
{
  CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(convertMouseButton(id));
  return true;
}

CEGUI::MouseButton MyFrameListener::convertMouseButton(OIS::MouseButtonID id)
{
  CEGUI::MouseButton ceguiId;
  switch(id)
    {
    case OIS::MB_Left:
      ceguiId = CEGUI::LeftButton;
      break;
    case OIS::MB_Right:
      ceguiId = CEGUI::RightButton;
      break;
    case OIS::MB_Middle:
      ceguiId = CEGUI::MiddleButton;
      break;
    default:
      ceguiId = CEGUI::LeftButton;
    }
  return ceguiId;
}

Ray MyFrameListener::setRayQuery(int posx, int posy, uint32 mask) {
  Ray rayMouse = _camera->getCameraToViewportRay
    (posx/float(_win->getWidth()), posy/float(_win->getHeight()));
  _raySceneQuery->setRay(rayMouse);
  _raySceneQuery->setSortByDistance(true);
  _raySceneQuery->setQueryMask(mask);
  return (rayMouse);
}




bool MyFrameListener::nextShip(const CEGUI::EventArgs &e)
{

	SceneNode* planePlayer =static_cast<SceneNode*>(_sceneManager->getRootSceneNode()->getChild("Tablero"));

  //Cambio de estado
  switch(state) {
    
    case start:{//Un estado por cada barco, Barco1
      cout << "\nEstoy en start por el reinico\n" <<endl;
      //Cambio el texto de la interfaz
      CEGUI::Window* Auxsheet=CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
      CEGUI::Window* AuxconfigWin2=Auxsheet->getChildAtIdx(0);
      AuxconfigWin2->getChild("Text1")->setText("Volvamos a jugar,pulsa siguiente");
      state=posicionando1;

      break;}

    case posicionando1:{//Un estado por cada barco, Barco1

      cout << "\nEstoy en posicionando1\n" <<endl;
      
      Ogre::Entity* entBarco1 = _sceneManager->createEntity("barco1.mesh");
      Ogre::SceneNode* nodeBarco1 = _sceneManager->createSceneNode("Barco");
      nodeBarco1->attachObject(entBarco1);
      nodeBarco1->setPosition(0,0,0);
      nodeBarco1->setScale(10,10,10);
      emi=new EntityMaterialInstance(entBarco1);//Para alpha
      planePlayer->addChild(nodeBarco1);

      state=posicionando2;

      //Posicion en la que aparece el primer barco
      i=4;
      j=4;

      //Rotacion inicial
      rotation=1;
      
      //Cambio el texto de la interfaz
      CEGUI::Window* Auxsheet=CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
      CEGUI::Window* AuxconfigWin2=Auxsheet->getChildAtIdx(0);
      AuxconfigWin2->getChild("Text1")->setText("Colocando 1º Barco. \n Pulsa ENTER para colocar");

      break;}

    case posicionando2:{//Un estado por cada barco, Barco2

      if(shipLocated==true){
        cout << "\nEstoy en posicionando2\n" <<endl;
        Entity* entBarco2 = _sceneManager->createEntity("barco2.mesh");
        SceneNode* nodeBarco2 = _sceneManager->createSceneNode("Barco2");
        nodeBarco2->attachObject(entBarco2);
        nodeBarco2->setPosition(0,22,0); //x,y,z
        nodeBarco2->setScale(2,25,1);
        nodeBarco2->yaw(Ogre::Degree(-180));
        emi=new EntityMaterialInstance(entBarco2);//Para alpha
        planePlayer->addChild(nodeBarco2);
    
        state=posicionando3;

        //Siguiente barco aparece en el 4,4 
        //Consideramos la posicion como la popa (El de atras) (PUNTO DE APLICACION)
        i=4;
        j=4;

        //Rotacion inicial
        rotation=1;

        //Cambio el texto de la interfaz
      	CEGUI::Window* Auxsheet=CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
      	CEGUI::Window* AuxconfigWin2=Auxsheet->getChildAtIdx(0);
      	AuxconfigWin2->getChild("Text1")->setText("Colocando 2º Barco. \n Pulsa ENTER para colocar");

        shipLocated=false;
      }
      break;}

    case posicionando3:{//Un estado por cada barco, Barco1

      if(shipLocated==true){
        cout << "\nEstoy en posicionando3\n" <<endl;
        Entity* entBarco3 = _sceneManager->createEntity("barco3.mesh");
        SceneNode* nodeBarco3 = _sceneManager->createSceneNode("Barco3");
        nodeBarco3->attachObject(entBarco3);
        nodeBarco3->setPosition(0,0,0); //x,y,z
        nodeBarco3->setScale(8,8,8);
        nodeBarco3->yaw(Ogre::Degree(-180));
        emi=new EntityMaterialInstance(entBarco3);//Para alpha
        planePlayer->addChild(nodeBarco3);

        //Siguiente barco aparece en el 5,4 
        //Consideramos la posicion como la popa (El de atras) (PUNTO DE APLICACION)
        i=4;
        j=4;

        //Rotacion inicial
        rotation=1;

        //Cambio el texto de la interfaz
        CEGUI::Window* Auxsheet=CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
        CEGUI::Window* AuxconfigWin2=Auxsheet->getChildAtIdx(0);
        AuxconfigWin2->getChild("Text1")->setText("Colocando 3º Barco. \n Pulsa ENTER para colocar");
        
        state=posicionando4;
        shipLocated=false;
      }
      break;}

    case posicionando4:{//Un estado por cada barco, Barco1

      if(shipLocated==true){

      cout << "\nEstoy en posicionando4\n" <<endl;
      Entity* entBarco4 = _sceneManager->createEntity("barco4.mesh");
      SceneNode* nodeBarco4 = _sceneManager->createSceneNode("Barco4");
      nodeBarco4->attachObject(entBarco4);
      nodeBarco4->setPosition(0,0,0); //x,y,z
      nodeBarco4->setScale(8,8,8);
      nodeBarco4->yaw(Ogre::Degree(-180));
      emi=new EntityMaterialInstance(entBarco4);//Para alpha
      planePlayer->addChild(nodeBarco4);

      //Siguiente barco aparece en el 5,4 
      //Consideramos la posicion como la popa (El de atras) (PUNTO DE APLICACION)
      i=4;
      j=4;

      //Rotacion inicial
      rotation=1;

      //Cambio el texto de la interfaz
      CEGUI::Window* Auxsheet=CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
      CEGUI::Window* AuxconfigWin2=Auxsheet->getChildAtIdx(0);
      AuxconfigWin2->getChild("Text1")->setText("Colocando 4º Barco. \n Pulsa ENTER para colocar");

    
      state=posicionando5;
      shipLocated=false;
      }
      break;}

    case posicionando5:{//Un estado por cada barco, Barco1

      if(shipLocated==true){
    
        cout << "\nPasamos a posicionando6\n" <<endl;
        state=posicionando6;
        shipLocated=false;
      }
      break;}

    case posicionando6:{

      //Cambio el texto de la interfaz
      CEGUI::Window* Auxsheet=CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
      CEGUI::Window* AuxconfigWin2=Auxsheet->getChildAtIdx(0);
      AuxconfigWin2->getChild("Text1")->setText("Juguemos");

      state=jugando;

      break;}
    }

    

  return true;
}

bool MyFrameListener::quit(const CEGUI::EventArgs &e)
{

  _quit = true;
  return true;
}

bool MyFrameListener::showRecords(const CEGUI::EventArgs &e)
{
	resetGame();
  if(_records==true){
    _records=false;
  }
  else{
    _records=true;
  }
    cout << "\nRecords\n" << _records <<endl;
  return true;
}

bool MyFrameListener::showControls(const CEGUI::EventArgs &e)
{

	cout << "\nControls\n" <<endl;

  //Selecciono la principal
  CEGUI::Window* sheet=CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
  sheet->getChild("controlsFrame")->setVisible(showCreditsFrame);
  if(showCreditsFrame==true){
    showCreditsFrame=false;
  }else{
    showCreditsFrame=true;
  }



	return true;
}
bool MyFrameListener::showCredits(const CEGUI::EventArgs &e)
{
  
  cout << "\nCreditos\n" <<endl;

  //Selecciono la principal
  CEGUI::Window* sheet=CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
  sheet->getChild("creditFrame")->setVisible(showCreditsFrame);
  if(showCreditsFrame==true){
    showCreditsFrame=false;
  }else{
    showCreditsFrame=true;
  }
  return true;
}

bool MyFrameListener::restart(const CEGUI::EventArgs &e)
{
  
  cout << "Restart " << endl;
  
	  CEGUI::Window* sheet2=CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
	  sheet2->getChild("gameoverFrame")->setVisible(false);

	  CEGUI::Window* sheet=CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
	  sheet->getChild("Cfg")->getChild("exitButton")->setVisible(true);
	  sheet->getChild("Cfg")->getChild("recordsButton")->setVisible(true);
	  sheet->getChild("Cfg")->getChild("Text1")->setVisible(true);
	  sheet->getChild("Cfg")->getChild("nextShipButton")->setVisible(true);
	  sheet->getChild("Cfg")->getChild("controlsButton")->setVisible(true);

	  //Resetear matriz de barcos
	  boardEnemy.generateRandomBoard();


  return true;
}

bool MyFrameListener::play(const CEGUI::EventArgs &e)
{
  _quit = false;
  cout << "\nJugemos\n" <<endl;
  state = posicionando1;


  CEGUI::Window* sheet=CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
  
  sheet->setVisible(false);

  //Nueva Pantalla
  CEGUI::Window* sheet2 = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage","Cfg");
  sheet2->setPosition(CEGUI::UVector2(CEGUI::UDim(0.01, 0),CEGUI::UDim(0.03, 0)));
  sheet2->setSize(CEGUI::USize(CEGUI::UDim(0.96, 0), CEGUI::UDim(0.90, 0)));
  sheet2->setProperty("BackgroundEnabled", "False"); //Para que se vea transparente
  sheet2->setProperty("FrameEnabled","False"); //Para quitarle el borde
  
  //Nuevo Layout
  CEGUI::Window* configWin2 = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("playstate.layout");
  
  //Setting Text!
  configWin2->getChild("Text1")->setText("Colocando Barcos. \n Pulsa siguiente");

  //Nuevo Boton y suscripcion
  CEGUI::Window* exitButton = configWin2->getChild("exitButton");
  exitButton->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&MyFrameListener::quit,this));  
  exitButton->setXPosition(CEGUI::UDim(0.01f, 0.0f)); 
  exitButton->setYPosition(CEGUI::UDim(0.45f, 0.0f));

  CEGUI::Window* recordButton = configWin2->getChild("recordsButton");
  recordButton->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&MyFrameListener::showRecords,this));
  recordButton->setXPosition(CEGUI::UDim(0.01f, 0.0f)); 
  recordButton->setYPosition(CEGUI::UDim(0.35f, 0.0f));

  CEGUI::Window* nextShipButton = configWin2->getChild("nextShipButton");
  nextShipButton->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&MyFrameListener::nextShip,this));
  nextShipButton->setXPosition(CEGUI::UDim(0.01f, 0.0f)); 
  nextShipButton->setYPosition(CEGUI::UDim(0.25f, 0.0f));

  CEGUI::Window* controlsButton = configWin2->getChild("controlsButton");
  controlsButton->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&MyFrameListener::showControls,this));
  controlsButton->setXPosition(CEGUI::UDim(0.01f, 0.0f)); 
  controlsButton->setYPosition(CEGUI::UDim(0.90f, 0.0f));

  //CONTROLS

  CEGUI::ImageManager::getSingleton().addFromImageFile("BackgroundImageControls","controles.png");
  CEGUI::Window* sheetControls = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage","controlsFrame");
  sheetControls->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0),CEGUI::UDim(0.6, 0)));
  sheetControls->setSize(CEGUI::USize(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.35, 0)));
  sheetControls->setProperty("Image","BackgroundImageControls");
  sheetControls->setProperty("BackgroundEnabled", "False");
  sheetControls->setProperty("FrameEnabled","False");

  //Nuevo Layout
  CEGUI::Window* configWinControls = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("controls.layout");
  sheetControls->setVisible(false);
  sheetControls->addChild(configWinControls);


  //RECORDS

  CEGUI::ImageManager::getSingleton().addFromImageFile("BackgroundImageRecords","creditos.jpg");
  CEGUI::Window* sheetRecords = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage","recordsFrame");
  sheetRecords->setPosition(CEGUI::UVector2(CEGUI::UDim(0.01, 0),CEGUI::UDim(0.6, 0)));
  sheetRecords->setSize(CEGUI::USize(CEGUI::UDim(0.70, 0), CEGUI::UDim(0.35, 0)));
  sheetRecords->setProperty("Image","BackgroundImageRecords");

  //Nuevo Layout
  CEGUI::Window* configWinRecords = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("records.layout");

  CEGUI::Window* exitButtonRecord = configWinRecords->getChild("exitButton");
  exitButtonRecord->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&MyFrameListener::quit,this));
  sheetRecords->addChild(configWinRecords);


  //GAME OVER (JUGADOR)
  CEGUI::ImageManager::getSingleton().addFromImageFile("BackgroundImageGameOver","gameover.png");
  CEGUI::Window* sheetGameOver = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage","gameoverFrame");
  sheetGameOver->setPosition(CEGUI::UVector2(CEGUI::UDim(0.06, 0),CEGUI::UDim(0, 0)));
  sheetGameOver->setSize(CEGUI::USize(CEGUI::UDim(0.90, 0), CEGUI::UDim(0.95, 0)));
  sheetGameOver->setProperty("Image","BackgroundImageGameOver");
  sheetGameOver->setProperty("BackgroundEnabled", "False");
  sheetGameOver->setProperty("FrameEnabled","False");

  //Nuevo Layout
  CEGUI::Window* configWinGameOver = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("gameoverPlayer.layout");
  

  CEGUI::Window* exitButtonGameOver = configWinGameOver->getChild("exitButton");
  exitButtonGameOver->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&MyFrameListener::quit,this));
  exitButtonGameOver->setXPosition(CEGUI::UDim(0.60f, 0.0f)); 
  exitButtonGameOver->setYPosition(CEGUI::UDim(0.85f, 0.0f));

  CEGUI::Window* restartButton = configWinGameOver->getChild("restartButton");
  restartButton->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&MyFrameListener::restart,this));
  restartButton->setXPosition(CEGUI::UDim(0.20f, 0.0f)); 
  restartButton->setYPosition(CEGUI::UDim(0.85f, 0.0f));
  sheetGameOver->setVisible(false);
  sheetGameOver->addChild(configWinGameOver);
	
  //Añado los botones a la pantalla
  sheet2->addChild(configWin2);
  sheet2->addChild(sheetControls);
  sheet2->addChild(sheetGameOver);
  
  CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet2);
  
  return true;
}

void MyFrameListener::MoverBarco(string Barco,OIS::Keyboard* _keyboard,SceneManager* _sceneManager,int limSup,int limInf,int limDer,int limIzq,int desp){
  
  SceneNode* aux =static_cast<SceneNode*>(_sceneManager->getRootSceneNode()->getChild("Tablero")->getChild(Barco));

  if(_keyboard->isKeyDown(OIS::KC_R) && canRotate==true ){//Si pulso la R rotamos 90 grados
        canRotate=false;
        aux->yaw(Ogre::Degree(90));
        rotation++;
        if(rotation>4){
          rotation=1;
        }
      }
      if(_keyboard->isKeyDown(OIS::KC_R) == false){//La tecla se ha levantado
        canRotate=true;
      }
      //Movimiento del Barco
      //Arriba
      if(_keyboard->isKeyDown(OIS::KC_UP) && UPkeyDown == true){//Si la tecla esta pulsada se detecta una unica pulsacion
        UPkeyDown=false;//Para detectar una unica pulsacion
        if(i>0){//mientras no llegue al limite cambio las posicion
          i--;//cambio la posicion
        }
       
        //cout << "UP" << Vector3(aux->getPosition()) << "Pos ->" <<"i:" << i << "j:" << j <<endl;//INFO
        if(Vector3(aux->getPosition()).z>-limSup || Vector3(aux->getPosition()).z==0){//Si estoy en los limites
            aux->setPosition(Vector3(aux->getPosition())+=Vector3(0,0,-desp));//Me muevo
            emi->setTransparency(0.0f);
        }
        if(Vector3(aux->getPosition()).z==-limSup){//Para la transparencia
            emi->setTransparency(0.7f);
        }
      }
      if(_keyboard->isKeyDown(OIS::KC_UP)==false){//La tecla se ha levantado
        UPkeyDown=true;//Puedo volver a pulsar 
      }
      //Abajo
      if(_keyboard->isKeyDown(OIS::KC_DOWN) && DOWNkeyDown == true){//Si la tecla esta pulsada se detecta una unica pulsacion
        DOWNkeyDown=false;
        if(i<8){//mientras no llegue al limite cambio las posicion
          i++;//cambio la posicion
        }
        
        //cout << "DOWN"<<Vector3(aux->getPosition()) << "Pos ->" <<"i:" << i << "j:" << j << endl;
        if(Vector3(aux->getPosition()).z<limInf || Vector3(aux->getPosition()).z==0){//Si estoy en los limites
            aux->setPosition(Vector3(aux->getPosition())+=Vector3(0,0,desp));//Me muevo
            emi->setTransparency(0.0f);
        }
      }
      if(_keyboard->isKeyDown(OIS::KC_DOWN)==false){//La tecla se ha levantado
        DOWNkeyDown=true;
      }
      //Izquierda
      if(_keyboard->isKeyDown(OIS::KC_LEFT) && LEFTkeyDown == true){//Si la tecla esta pulsada se detecta una unica pulsacion
        LEFTkeyDown=false;
        if(j>0){//mientras no llegue al limite cambio las posicion
          j--;//cambio la posicion
        }
        
        //cout << "LEFT"<<Vector3(aux->getPosition()) << "Pos ->" <<"i:" << i << "j:" << j<< endl;
        if(Vector3(aux->getPosition()).x>-limDer || Vector3(aux->getPosition()).x==0){//Si estoy en los limites
            aux->setPosition(Vector3(aux->getPosition())+=Vector3(-desp,0,0));//Me muevo

        }
      }
      if(_keyboard->isKeyDown(OIS::KC_LEFT)==false){//La tecla se ha levantado
        LEFTkeyDown=true;
      }
      //Derecha
      if(_keyboard->isKeyDown(OIS::KC_RIGHT) && RIGHTkeyDown == true){//Si la tecla esta pulsada se detecta una unica pulsacion
        RIGHTkeyDown=false;
        if(j<8){//mientras no llegue al limite cambio las posicion
          j++;//cambio la posicion
        }
        
        //cout << "RIGHT"<< Vector3(aux->getPosition()) << "Pos ->" <<"i:" << i << "j:" << j<< endl;
        if(Vector3(aux->getPosition()).x<limIzq || Vector3(aux->getPosition()).x==0){//Si estoy en los limites
            aux->setPosition(Vector3(aux->getPosition())+=Vector3(desp,0,0));//Me muevo
        }
      }
      if(_keyboard->isKeyDown(OIS::KC_RIGHT)==false){//La tecla se ha levantado
        RIGHTkeyDown=true;
      }
}


  void MyFrameListener::resetGame(){

  	


  	CEGUI::Window* sheet=CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
  	sheet->getChild("gameoverFrame")->setVisible(true);
  	sheet->getChild("gameoverFrame")->getChild("gameOver")->getChild("restartButton")->activate();
  	sheet->getChild("gameoverFrame")->getChild("gameOver")->getChild("exitButton")->activate();
	  sheet->getChild("Cfg")->getChild("exitButton")->setVisible(false);
	  sheet->getChild("Cfg")->getChild("recordsButton")->setVisible(false);
	  sheet->getChild("Cfg")->getChild("Text1")->setVisible(false);
	  sheet->getChild("Cfg")->getChild("nextShipButton")->setVisible(false);
	  sheet->getChild("Cfg")->getChild("controlsButton")->setVisible(false);

	  SceneNode* auxPlayer =static_cast<SceneNode*>(_sceneManager->getRootSceneNode()->getChild("Tablero"));

	  auxPlayer->getCreator()->destroySceneNode("Barco");
	  auxPlayer->getCreator()->destroySceneNode("Barco2");
	 // auxPlayer->getCreator()->destroySceneNode("Barco3");
	  //auxPlayer->getCreator()->destroySceneNode("Barco4");
	 // auxPlayer->getCreator()->destroySceneNode("Barco5");

		Ogre::SceneNode::ChildNodeIterator itPlayer = _sceneManager->getRootSceneNode()->getChild("Tablero")->getChildIterator();
		Ogre::SceneNode::ChildNodeIterator itEnemy = _sceneManager->getRootSceneNode()->getChild("Tablero Enemigo")->getChildIterator();
    while ( itPlayer.hasMoreElements() || itEnemy.hasMoreElements() ) {
	     SceneNode* childPlayer = static_cast<SceneNode*>(itPlayer.getNext());
	     SceneNode* childEnemy = static_cast<SceneNode*>(itEnemy.getNext());

	    if(Ogre::StringUtil::endsWith(childPlayer->getName(),"NodeCubeJugador",true) 
	    	|| Ogre::StringUtil::endsWith(childEnemy->getName(),"NodeCube",true)){
	      Entity* entPlayer = static_cast<Entity*>(childPlayer->getAttachedObject(0));
	      Entity* entEnemy = static_cast<Entity*>(childEnemy->getAttachedObject(0));
		    entPlayer->setMaterialName("agua");
		    entEnemy->setMaterialName("agua");
	        
      }
    }

    state=posicionando1;
    shipLocated=false,
    //Falta reiniciar las matrices de los tableros
    boardPlayer.cleanBoard();
    cout << "Tablero Jugador reset"<<endl;
    boardPlayer.printBoard();

    boardEnemy.cleanBoard();
    cout << "Tablero Enemy reset"<<endl;
    boardEnemy.printBoard();

    //Cambio el texto de la interfaz
    CEGUI::Window* Auxsheet=CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
    CEGUI::Window* AuxconfigWin2=Auxsheet->getChildAtIdx(0);
    AuxconfigWin2->getChild("Text1")->setText("Juguemos de nuevo!\nPulsa Siguiente");

		
  }