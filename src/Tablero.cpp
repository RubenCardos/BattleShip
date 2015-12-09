#include "Tablero.h"

Tablero::Tablero(){
	//inicializacion del tablero
	board=new int*[9];
    for(int i=0;i<9;i++){
        board[i]=new int[9];
    }
    for(int i=0; i<9 ; i++){
		for(int j=0; j<9; j++){
           board[i][j]=0;
 		}
	}

	//inicializacion del vector de barcos
	Barco ship1(3);  // 0 en el vector
   	Barco ship2(3);  // 1
   	Barco ship3(2);  // 2
   	Barco ship4(2);  // 3
   	Barco ship5(1);  // 4
   	
   	ships.push_back(ship1);
   	ships.push_back(ship2);
   	ships.push_back(ship3);
   	ships.push_back(ship4);
   	ships.push_back(ship5);
}

Tablero::~Tablero(){


}

bool Tablero::putShip(int i, int j,int shipSize,int pos){
	bool res=false;
    int busy=0;
	if(i%2==0){
    //cout << "Genero Vertical" << " i:" << i << " j:" << j << " Tamaño:" << shipSize << endl;
		if(i+shipSize-1<9){
            for(int n=0; n<shipSize && res==false; n++){
                if(board[i+n][j]==0){
                    board[i+n][j]=pos;
                   //cout <<"---["<< i+n <<"]["<<j <<"]="<< pos <<endl;
                    busy++;
                }else{
                    //cout << "Error, hay un obstaculo" << endl;
                    res=true;
                }
            }
            //si hubiera un error tengo que poner todo lo modificado antes 
            if(res){
                //cout << "Limpio el tablero" << endl;
                for(int n=0;n<shipSize;n++){
                    if(board[i+n][j]==pos){
                        board[i+n][j]=0;
                    }
                }
            }
		}else{
            //cout << "Error, no cabe en el tablero" << endl;
			//tablero[i][j]=0;
			res=true;
		}
        
    }else{
        //cout << "Genero Horizontal" << "i:" << i << " j:" << j << " Tamaño:" << shipSize << endl;
		if(j+shipSize-1<9){
            for(int n=0; n<shipSize && res==false ; n++){
        	   if(board[i][j+n]==0){
        		  board[i][j+n]=pos;
                  //cout <<"---["<< i <<"]["<<j+n <<"]="<< pos <<endl;
                }else{
                    //cout << "Error, hay un obstaculo" << endl;
                    res=true;
                }
            }
            //si hubiera un error tengo que poner todo a 0
            if(res){
              //cout << "Limpio el tablero" << endl;
              for(int n=0;n<shipSize;n++){
                if(board[i][j+n]==pos){
                    board[i][j+n]=0;
                }
              }
            }
		}else{
            //cout << "Error, no cabe en el tablero" << endl;
			//tablero[i][j]=0;
			res=true;
		}
    
  }
  return res;
}

int Tablero::checkBox(int i, int j){
	return board[i][j];
}

void Tablero::setBox(int i, int j, int cont){
  board[i][j]=cont;
}

bool Tablero::checkPosition(int i, int j){
  bool res=true;
  cout << "Aqui" <<" i:" << i << " j:" << j << endl;
  if(i < 0 || i > 8 || j < 0 || j > 8){//limites
    res=false;//la casilla no existe
    cout <<"Fuera limites" << endl;
  }else{//si la casilla existe la compruebo
    if(board[i][j]!=0){// ya hay algo en esa casilla
      res=false;
      cout <<"obstaculo" << endl;
    }
  }
  printBoard();
  return res;
}

void Tablero::printBoard(){
    cout << "----------" << endl;
    for(int i=0; i<9 ; i++){
        for(int j=0; j<9; j++){
            cout<<board[i][j] << " ";
        }
        cout << endl;
    }
    cout << "----------" << endl;
}

void Tablero::setBoard(int **modifyBoard){
	board=modifyBoard;
}

void Tablero::generateRandomBoard(){
	srand((unsigned)time(0));
	int pos=1;
	vector<Barco>::iterator it;
    for (it = ships.begin(); it != ships.end(); ++it) {
        int i; //imprime de 0 a 6 para no salirse de los limites
        int j;
        bool puesto=true;
        while(puesto){
  		    i=rand()%9; //genero de 0 a 9 para no salirse de los limites
            j=rand()%9;
            Barco valor=*it;
            //Metodo para saber si va a ser en horizontal o vertical
            puesto=putShip(i,j,valor.getSize(),pos);
        }
        pos++;
    }
    printBoard();
}

void Tablero::hitShip(int ship){//int ship es la posicion en el vector
  ships[ship].hit();
}

int Tablero::getSizeShip(int ship){
  return ships[ship].getSize();
}

bool Tablero::anyShipAlive(){
  bool res=false;
  for(int i=0; i<9 ; i++){
    for(int j=0; j<9; j++){
      if(board[i][j]!=0){//No es agua
        if(board[i][j]!=9){//ignoro los ya pulsados
          res=true;
        }
      }
    }
  }
  return res;
}

void Tablero::cleanBoard(){
  for(int i=0; i<9 ; i++){
    for(int j=0; j<9; j++){
      board[i][j]=0;
    }
  }
}