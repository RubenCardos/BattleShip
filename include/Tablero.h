#ifndef TABLERO_H
#define TABLERO_H
#include <iostream>
#include <time.h> 
#include <cstdlib>
#include <vector>
#include "Barco.h"

using namespace std;

class Tablero {

 public:
  Tablero();
  ~Tablero();
  
  int checkBox(int i, int j);
  void setBox(int i, int j, int cont);
  bool checkPosition(int i, int j);
  void printBoard();
  void setBoard(int **modifyBoard);
  void generateRandomBoard();
  void hitShip(int ship);
  int getSizeShip(int ship);
  bool anyShipAlive();
  void cleanBoard();
 
 private:
 	int **board;
 	std::vector <Barco> ships;
  bool putShip(int i, int j,int shipSize,int pos );
   
};
 
#endif
