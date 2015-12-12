#include "Tablero.h"

Tablero::Tablero(){
	//inicializacion del tablero
	board=new int*[9];
    for(int i=0;i<9;i++){
        board[i]=new int[9];
    }
    for(int i=0; i<9 ; i++){//Recorro las filas
  		for(int j=0; j<9; j++){//Recorro las columnas
        board[i][j]=0;//Cada casilla a 0
   		}
	}

	//inicializacion del vector de barcos
	Barco ship1(3);  // 0 en el vector
  Barco ship2(3);  // 1
  Barco ship3(2);  // 2
  Barco ship4(2);  // 3
  Barco ship5(1);  // 4
   	
  ships.push_back(ship1);//Añado el barco al vector, primero los mas grandes
  ships.push_back(ship2);//Añado el barco al vector
  ships.push_back(ship3);//Añado el barco al vector
  ships.push_back(ship4);//Añado el barco al vector
  ships.push_back(ship5);//Añado el barco al vector
}

Tablero::~Tablero(){//Destructor


}

bool Tablero::putShip(int i, int j,int shipSize,int pos){
	bool res=false;//Resultado
	if(i%2==0){//si i es par generamos un Barco en Vertical
		if(i+shipSize-1<9){//Comprobamos si el barco cabe(posicion+casillas que ocupa < tamaño)
      for(int n=0; n<shipSize && res==false; n++){//Recorro las casillas que ocupa
        if(board[i+n][j]==0){//Si la casilla esta disponible(0) la ocupamos
          board[i+n][j]=pos;//Ocupamos la casilla
        }else{//Hay un obstaculo 
          res=true;
        }
      }
      //Si hubiera un error tengo que poner todo lo modificado antes 
      if(res){
        for(int n=0;n<shipSize;n++){
          if(board[i+n][j]==pos){//Solo borro lo que he modificado en este barco, para no tocar otros barcos
            board[i+n][j]=0;//Casilla disponible de nuevo
          }
        }
      }
		}else{
			res=true;//Devuelvo un fallo
		}    
  }else{//Si es impar generamos el Barco en horizontal
		if(j+shipSize-1<9){//Comprobamos si el barco cabe(posicion+casillas que ocupa < tamaño)
      for(int n=0; n<shipSize && res==false ; n++){//Recorro las casillas que ocupa
        if(board[i][j+n]==0){//Si la casilla esta disponible(0) la ocupamos
        	board[i][j+n]=pos;//Ocupamos la casilla
        }else{//Hay un obstaculo
          res=true;
        }
      }
        //si hubiera un error tengo que poner todo a 0
        if(res){
          for(int n=0;n<shipSize;n++){
            if(board[i][j+n]==pos){//Solo borro lo que he modificado en este barco, para no tocar otros barcos
                board[i][j+n]=0;//Casilla disponible de nuevo
            }
          }
        }
		}else{
			res=true;//Devuelvo un error
		}
  }
  return res;
}

int Tablero::checkBox(int i, int j){//Devuelvo el contenido de la casilla[i][j]
	return board[i][j];
}

void Tablero::setBox(int i, int j, int cont){//Modifica el contenido de la casilla[i][j]
  board[i][j]=cont;
}

bool Tablero::checkPosition(int i, int j){//Comprueba si la casilla es valida.
  bool res=true;
  if(i < 0 || i > 8 || j < 0 || j > 8){//Limites del Tablero
    res=false;//la casilla no existe
  }else{//Si la casilla existe la compruebo
    if(board[i][j]!=0){//Ya hay algo en esa casilla
      res=false;//Error
    }
  }
  printBoard();//Muestro el tablero
  return res;
}

void Tablero::printBoard(){
    cout << "----------" << endl;
    for(int i=0; i<9 ; i++){//Recorro filas 
        for(int j=0; j<9; j++){//Recorro Columnas
            cout<<board[i][j] << " ";
        }
        cout << endl;
    }
    cout << "----------" << endl;
}

void Tablero::setBoard(int **modifyBoard){//Cambiar Tablero ¿USADO?
	board=modifyBoard;
}

void Tablero::generateRandomBoard(){//Generar Tablero Aleatorio (Para enemigos)
	srand((unsigned)time(0));//Semilla Aleatoria
	int pos=1;//Posicion que ocupa el Barco en el vector de Barcos +1
	vector<Barco>::iterator it;//Iterador del vector de Barcos
    for (it = ships.begin(); it != ships.end(); ++it) {
        int i; //Posicion
        int j; //Posicion
        bool puesto=true;//Si el Barco ha sido colocado o no
        while(puesto){//Lo sigo intentado poner hasta que se ponga
  		    i=rand()%9;//Genero de 0 a 9 para no salirse de los limites
          j=rand()%9;//Genero de 0 a 9 para no salirse de los limites
          Barco valor=*it;//Size del Barco a colocar
          puesto=putShip(i,j,valor.getSize(),pos);//Coloco el barco
        }
        pos++;//Aunmento la posicion, es decir, paso al siguiente barco
    }
    printBoard();//Muestro el tablero una vez generado entero
}

void Tablero::hitShip(int ship){//¿Usado?
  ships[ship].hit();
}

int Tablero::getSizeShip(int ship){//¿Usado?
  return ships[ship].getSize();
}

bool Tablero::anyShipAlive(){//Veo si queda alguna casilla con Barco aun
  bool res=false;
  for(int i=0; i<9 ; i++){//Recorro filas
    for(int j=0; j<9; j++){//Recorro Columnas
      if(board[i][j]!=0){//No es agua
        if(board[i][j]!=9){//Ignoro los ya pulsados
          res=true;//Queda algun barco vivo
        }
      }
    }
  }
  return res;
}

void Tablero::cleanBoard(){//Pongo de nuevo todo el tablero a cero
  for(int i=0; i<9 ; i++){//Recorro filas
    for(int j=0; j<9; j++){//Recorro Columnas
      board[i][j]=0;//Casilla a 0
    }
  }
}