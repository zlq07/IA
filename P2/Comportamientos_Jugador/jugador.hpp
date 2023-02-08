#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"

#include <list>

struct estado
{
  int fila;
  int columna;
  int orientacion;
  bool llevarBikini;
  bool llevarZapatillas;
};

struct nodo
{
  estado st;
  list<Action> secuencia;
  int costeUniforme;
  inline bool operator<(const nodo &otro) const
  {
    return otro.costeUniforme < this->costeUniforme;
  }
};

class ComportamientoJugador : public Comportamiento
{
public:
  ComportamientoJugador(unsigned int size) : Comportamiento(size)
  {
    // Inicializar Variables de Estado
    hayPlan = false;
    fil = col = 99;
    brujula = 0;
    ultimaAccion = actIDLE;
    bien_situado = false;
    objetivo_n3.fila = 3;
    objetivo_n3.columna = 3;
    plan1 = 0;
    cont = false;
    encontrarX = false;
  }
  ComportamientoJugador(std::vector<std::vector<unsigned char>> mapaR) : Comportamiento(mapaR)
  {
    // Inicializar Variables de Estado
    hayPlan = false;
    actual.llevarBikini = false;
    actual.llevarZapatillas = false;
    cont = false;
  }
  ComportamientoJugador(const ComportamientoJugador &comport) : Comportamiento(comport) {}
  ~ComportamientoJugador() {}

  Action think(Sensores sensores);
  int interact(Action accion, int valor);
  void VisualizaPlan(const estado &st, const list<Action> &plan);
  ComportamientoJugador *clone() { return new ComportamientoJugador(*this); }
  void EsK_D(estado &st);
  int CosteBateria(nodo &n, const Action &accion);
  void guardarVisitado(Sensores sensores);

private:
  // Declarar Variables de Estado
  estado actual;
  list<estado> objetivos;
  list<Action> plan;
  bool hayPlan;
  estado objetivo_n3;
  int plan1;
  bool cont;
  bool encontrarX;
  
  // Declarar aquí las variables de estado
  int fil, col, brujula;
  Action ultimaAccion;
  bool bien_situado;

  // Métodos privados de la clase
  bool pathFinding(int level, const estado &origen, const list<estado> &destino, list<Action> &plan);
  bool pathFinding_Profundidad(const estado &origen, const estado &destino, list<Action> &plan);
  bool pathFinding_Anchura(const estado &origen, const estado &destino, list<Action> &plan);
  bool pathFinding_CosteUniforme(const estado &origen, const estado &destino, list<Action> &plan);
  bool pathFinding_CosteUniforme_R2(const estado &origen, const estado &destino, list<Action> &plan);

  void PintaPlan(list<Action> plan);
  bool HayObstaculoDelante(estado &st);
};

#endif
