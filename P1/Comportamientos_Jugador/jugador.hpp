#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"
#include <stdlib.h>
using namespace std;

class ComportamientoJugador : public Comportamiento{

  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size){
      // Constructor de la clase
      // Dar el valor inicial a las variables de estado
      fil = col = 99;
      brujula = 0;
      ultimaAccion = actIDLE;
      girar_derecha = false;
      bien_situado = false;
      bikini = false;
      zapatillas = false;
      encontrarG = false;
      encontrarX = false;
      encontrarD = false;
      encontrarK = false;
      meChocoP = false;
      salirP = false;

      cont = 1;
      cont1 = 1;
      cont2 = 1;
      cont3 = 0;
      cont4 = 0;
    }

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);
    void guardarVisitado(Sensores sensores);
  
  private:
  
  // Declarar aquí las variables de estado
  int fil, col, brujula;
  Action ultimaAccion;
  bool girar_derecha;
  bool bien_situado;
  bool bikini;
  bool zapatillas;
  bool encontrarG;
  bool encontrarX;
  bool encontrarD;
  bool encontrarK;
  bool meChocoP;
  bool salirP;


  int cont;
  int cont1;
  int cont2;
  int cont3;
  int cont4;
};

#endif
