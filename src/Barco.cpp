#include <iostream>
#include "Barco.h"


using namespace std;

Barco::Barco(const int size){
  _size=size;
}

Barco::~Barco(){

}

int Barco::getSize(){
	return _size;
}

void Barco::hit(){
	_size=_size-1;
}

