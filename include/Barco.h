#ifndef BARCO_H
#define BARCO_H
 
class Barco {
 public:
  Barco (int size);
  ~Barco ();
 
  int getSize ();
  void hit () ;

 private:
  int _size;
};
 
#endif
