#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>
#include <set>
#include <stack>
#include <queue>

estado colocarObjetivo(const vector<vector<unsigned char>> &mapa);
bool EsObstaculo(unsigned char casilla);
// Este es el método principal que se piden en la practica.
// Tiene como entrada la información de los sensores y devuelve la acción a realizar.
// Para ver los distintos sensores mirar fichero "comportamiento.hpp"
Action ComportamientoJugador::think(Sensores sensores)
{
	if (sensores.reset)
	{
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

	if (sensores.terreno[0] == 'K')
	{
		actual.llevarBikini = true;
	}

	if (sensores.terreno[0] == 'D')
	{
		actual.llevarZapatillas = true;
	}

	if (sensores.nivel == 3 || sensores.nivel == 4)
	{
		if (EsObstaculo(sensores.terreno[2]))
		{
			hayPlan = false;
		}

		if (sensores.superficie[2] == 'a' || sensores.superficie[2] == 'j')
		{
			hayPlan = false;
		}

		switch (ultimaAccion)
		{
		case actFORWARD:
			switch (brujula)
			{
			case 0:
				fil--;
				break;
			case 1:
				fil--;
				col++;
				break;
			case 2:
				col++;
				break;
			case 3:
				fil++;
				col++;
				break;
			case 4:
				fil++;
				break;
			case 5:
				fil++;
				col--;
				break;
			case 6:
				col--;
				break;
			case 7:
				fil--;
				col--;
				break;
			}
			break;
		case actTURN_R:
			brujula = (brujula + 2) % 8;
			break;
		case actSEMITURN_R:
			brujula = (brujula + 1) % 8;
			break;
		case actTURN_L:
			brujula = (brujula + 6) % 8;
			break;
		case actSEMITURN_L:
			brujula = (brujula + 7) % 8;
			break;
		}

		// Activa sensores de orientación
		if (sensores.posF != -1 && !bien_situado)
		{
			fil = sensores.posF;
			col = sensores.posC;
			brujula = sensores.sentido;
			bien_situado = true;
		}

		if (bien_situado)
		{
			// mapaResultado[fil][col] = sensores.terreno[0];
			guardarVisitado(sensores);
		}
	}

	actual.fila = sensores.posF;
	actual.columna = sensores.posC;
	actual.orientacion = sensores.sentido;

	cout << "Fila: " << actual.fila << endl;
	cout << "Col : " << actual.columna << endl;
	cout << "Ori : " << actual.orientacion << endl;
	if (sensores.nivel != 3)
	{
		// Capturo los destinos
		cout << "sensores.num_destinos : " << sensores.num_destinos << endl;
		objetivos.clear();
		for (int i = 0; i < sensores.num_destinos; i++)
		{
			estado aux;
			aux.fila = sensores.destino[2 * i];
			aux.columna = sensores.destino[2 * i + 1];
			objetivos.push_back(aux);
		}
	}

	// Si no hay plan, contruyen uno

	if (sensores.nivel == 3)
	{
		if (actual.llevarZapatillas == false and sensores.terreno[2] == 'B')
		{
			hayPlan = false;
		}

		if (actual.llevarBikini == false and sensores.terreno[2] == 'A')
		{
			hayPlan = false;
		}

		if (objetivo_n3.fila == -1 or mapaResultado[objetivo_n3.fila][objetivo_n3.columna] != '?' or plan1 == 10)
		{
			objetivo_n3 = colocarObjetivo(mapaResultado);
			if (objetivo_n3.fila == -1)
			{
				cout << "mapa descubierto por completo" << endl;
				ultimaAccion = actIDLE;
				return actIDLE;
			}

			cout << "seleccionando nuevo objetivo" << endl;
			plan1++;
			cout << objetivo_n3.fila << endl;
			cout << objetivo_n3.columna << endl;
			objetivos.clear();
			objetivos.push_back(objetivo_n3);
			hayPlan = false;
		}

		if (sensores.terreno[0] == 'X')
		{
			if (sensores.bateria < 3000)
			{
				sensores.bateria = sensores.bateria + 10;
				ultimaAccion = actIDLE;
				return actIDLE;
			}
			else
			{
				hayPlan = false;
			}
		}

		// buscar pila
		if (encontrarX)
		{
			switch (ultimaAccion)
			{
			case actFORWARD:
				ultimaAccion = actTURN_L;
				encontrarX = false;
				return actTURN_L;
				break;
			case actTURN_L:
				ultimaAccion = actFORWARD;
				return actFORWARD;
				break;
			case actTURN_R:
				ultimaAccion = actFORWARD;
				return actFORWARD;
				break;
			}
		}
		if ((sensores.sentido == 0 || sensores.sentido == 2 || sensores.sentido == 4 || sensores.sentido == 6) && sensores.bateria < 2000)
		{
			if (sensores.terreno[5] == 'X' || sensores.terreno[4] == 'X')
			{
				ultimaAccion = actTURN_L;
				encontrarX = true;
				return actTURN_L;
			}
			if (sensores.terreno[3] == 'X' || sensores.terreno[8] == 'X')
			{
				ultimaAccion = actTURN_R;
				encontrarX = true;
				return actTURN_R;
			}
			if (sensores.terreno[6] == 'X' || sensores.terreno[12] == 'X')
			{
				ultimaAccion = actFORWARD;
				return actFORWARD;
			}
		}
		if (sensores.terreno[2] == 'X')
		{
			ultimaAccion = actFORWARD;
			encontrarX = false;
			return actFORWARD;
		}

		if (sensores.terreno[0] == 'X')
		{
			if (sensores.tiempo > 1000 and sensores.bateria < 3000)
			{
				sensores.bateria = sensores.bateria + 10;
				ultimaAccion = actIDLE;
				return actIDLE;
			}
			else
			{
				hayPlan = false;
			}
		}
		// colocar la casilla precipicio
		if ((objetivo_n3.fila > 3 || objetivo_n3.columna > 3 || plan1 == 2) and !cont)
		{
			for (int c = 0; c < mapaResultado.size(); c++)
			{
				mapaResultado[0][c] = 'P';
				mapaResultado[1][c] = 'P';
				mapaResultado[2][c] = 'P';
				mapaResultado[c][0] = 'P';
				mapaResultado[c][1] = 'P';
				mapaResultado[c][2] = 'P';
			}

			for (int c = 0; c < mapaResultado.size(); c++)
			{
				mapaResultado[mapaResultado.size() - 3][c] = 'P';
				mapaResultado[mapaResultado.size() - 2][c] = 'P';
				mapaResultado[mapaResultado.size() - 1][c] = 'P';
				mapaResultado[c][mapaResultado.size() - 3] = 'P';
				mapaResultado[c][mapaResultado.size() - 2] = 'P';
				mapaResultado[c][mapaResultado.size() - 1] = 'P';
			}
			cont = true;
		}
	}

	if (!hayPlan)
	{

		hayPlan = pathFinding(sensores.nivel, actual, objetivos, plan);
	}

	Action sigAccion;
	if (hayPlan && plan.size() > 0)
	{
		sigAccion = plan.front();
		plan.erase(plan.begin());
	}
	else
	{
		cout << "no se pudo encontrar un plan\n";
	}

	ultimaAccion = sigAccion;
	return sigAccion;
}

// Llama al algoritmo de busqueda que se usara en cada comportamiento del agente
// Level representa el comportamiento en el que fue iniciado el agente.
bool ComportamientoJugador::pathFinding(int level, const estado &origen, const list<estado> &destino, list<Action> &plan)
{
	estado un_objetivo;
	un_objetivo = objetivos.front();

	switch (level)
	{
	case 0:
		cout << "Demo\n";
		// estado un_objetivo;
		// un_objetivo = objetivos.front();
		cout << "fila: " << un_objetivo.fila << " col:" << un_objetivo.columna << endl;
		return pathFinding_Profundidad(origen, un_objetivo, plan);
		break;
	case 1:
		cout << "Optimo numero de acciones\n";
		// Incluir aqui la llamada al busqueda en anchura
		cout << "fila: " << un_objetivo.fila << " col:" << un_objetivo.columna << endl;
		return pathFinding_Anchura(origen, un_objetivo, plan);
		break;
	case 2:
		cout << "Optimo en coste\n";
		// Incluir aqui la llamada al busqueda de costo uniforme/A*
		cout << "fila: " << un_objetivo.fila << " col:" << un_objetivo.columna << endl;
		return pathFinding_CosteUniforme(origen, un_objetivo, plan);
		cout << "No implementado aun\n";
		break;
	case 3:
		cout << "Reto 1: Descubrir el mapa\n";
		// Incluir aqui la llamada al algoritmo de busqueda para el Reto 1
		cout << "fila: " << un_objetivo.fila << " col:" << un_objetivo.columna << endl;
		return pathFinding_CosteUniforme(origen, un_objetivo, plan);
		cout << "No implementado aun\n";
		break;

	case 4:
		cout << "Reto 2: Maximizar objetivos\n";
		// Incluir aqui la llamada al algoritmo de busqueda para el Reto 2
		cout << "fila: " << un_objetivo.fila << " col:" << un_objetivo.columna << endl;
		return pathFinding_CosteUniforme(origen, un_objetivo, plan);
		cout << "No implementado aun\n";
		break;
	}
	return false;
}

//---------------------- Implementación de la busqueda en profundidad ---------------------------

// Dado el codigo en caracter de una casilla del mapa dice si se puede
// pasar por ella sin riegos de morir o chocar.
bool EsObstaculo(unsigned char casilla)
{
	if (casilla == 'P' or casilla == 'M')
		return true;
	else
		return false;
}

// Comprueba si la casilla que hay delante es un obstaculo. Si es un
// obstaculo devuelve true. Si no es un obstaculo, devuelve false y
// modifica st con la posición de la casilla del avance.
bool ComportamientoJugador::HayObstaculoDelante(estado &st)
{
	int fil = st.fila, col = st.columna;

	// calculo cual es la casilla de delante del agente
	switch (st.orientacion)
	{
	case 0:
		fil--;
		break;
	case 1:
		fil--;
		col++;
		break;
	case 2:
		col++;
		break;
	case 3:
		fil++;
		col++;
		break;
	case 4:
		fil++;
		break;
	case 5:
		fil++;
		col--;
		break;
	case 6:
		col--;
		break;
	case 7:
		fil--;
		col--;
		break;
	}

	// Compruebo que no me salgo fuera del rango del mapa
	if (fil < 0 or fil >= mapaResultado.size())
		return true;
	if (col < 0 or col >= mapaResultado[0].size())
		return true;

	// Miro si en esa casilla hay un obstaculo infranqueable
	if (!EsObstaculo(mapaResultado[fil][col]))
	{
		// No hay obstaculo, actualizo el parametro st poniendo la casilla de delante.
		st.fila = fil;
		st.columna = col;
		return false;
	}
	else
	{
		return true;
	}
}

struct ComparaEstados
{
	bool operator()(const estado &a, const estado &n) const
	{
		if ((a.fila > n.fila) or (a.fila == n.fila and a.columna > n.columna) or
			(a.fila == n.fila and a.columna == n.columna and a.orientacion > n.orientacion))
			return true;
		else
			return false;
	}
};
// Implementación de la busqueda en profundidad.
// Entran los puntos origen y destino y devuelve la
// secuencia de acciones en plan, una lista de acciones.
bool ComportamientoJugador::pathFinding_Profundidad(const estado &origen, const estado &destino, list<Action> &plan)
{
	// Borro la lista
	cout << "Calculando plan\n";
	plan.clear();
	set<estado, ComparaEstados> Cerrados; // Lista de Cerrados
	stack<nodo> Abiertos;				  // Lista de Abiertos

	nodo current;
	current.st = origen;
	// current.secuencia.empty();
	current.secuencia.clear();

	Abiertos.push(current);

	while (!Abiertos.empty() and (current.st.fila != destino.fila or current.st.columna != destino.columna))
	{

		Abiertos.pop();
		Cerrados.insert(current.st);

		// Generar descendiente de girar a la derecha 90 grados
		nodo hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion + 2) % 8;
		if (Cerrados.find(hijoTurnR.st) == Cerrados.end())
		{
			hijoTurnR.secuencia.push_back(actTURN_R);
			Abiertos.push(hijoTurnR);
		}

		// Generar descendiente de girar a la derecha 45 grados
		nodo hijoSEMITurnR = current;
		hijoSEMITurnR.st.orientacion = (hijoSEMITurnR.st.orientacion + 1) % 8;
		if (Cerrados.find(hijoSEMITurnR.st) == Cerrados.end())
		{
			hijoSEMITurnR.secuencia.push_back(actSEMITURN_R);
			Abiertos.push(hijoSEMITurnR);
		}

		// Generar descendiente de girar a la izquierda 90 grados
		nodo hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion + 6) % 8;
		if (Cerrados.find(hijoTurnL.st) == Cerrados.end())
		{
			hijoTurnL.secuencia.push_back(actTURN_L);
			Abiertos.push(hijoTurnL);
		}

		// Generar descendiente de girar a la izquierda 45 grados
		nodo hijoSEMITurnL = current;
		hijoSEMITurnL.st.orientacion = (hijoSEMITurnL.st.orientacion + 7) % 8;

		if (Cerrados.find(hijoSEMITurnL.st) == Cerrados.end())
		{
			hijoSEMITurnL.secuencia.push_back(actSEMITURN_L);
			Abiertos.push(hijoSEMITurnL);
		}

		// Generar descendiente de avanzar
		nodo hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st))
		{
			if (Cerrados.find(hijoForward.st) == Cerrados.end())
			{
				hijoForward.secuencia.push_back(actFORWARD);
				Abiertos.push(hijoForward);
			}
		}

		// Tomo el siguiente valor de la Abiertos
		if (!Abiertos.empty())
		{
			current = Abiertos.top();
		}
	}

	cout << "Terminada la busqueda\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna)
	{
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else
	{
		cout << "No encontrado plan\n";
	}

	return false;
}

//---------------------- Implementación de la busqueda en anchura ---------------------------

bool ComportamientoJugador::pathFinding_Anchura(const estado &origen, const estado &destino, list<Action> &plan)
{
	// Borro la lista
	cout << "Calculando plan\n";
	plan.clear();
	set<estado, ComparaEstados> Cerrados; // Lista de Cerrados
	queue<nodo> Abiertos;				  // Lista de Abiertos

	nodo current;
	current.st = origen;
	// current.secuencia.empty();
	current.secuencia.clear();

	Abiertos.push(current);

	while (!Abiertos.empty() && (current.st.fila != destino.fila || current.st.columna != destino.columna))
	{

		Abiertos.pop();
		Cerrados.insert(current.st);

		// Generar descendiente de girar a la derecha 90 grados
		nodo hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion + 2) % 8;
		if (Cerrados.find(hijoTurnR.st) == Cerrados.end())
		{
			hijoTurnR.secuencia.push_back(actTURN_R);
			Abiertos.push(hijoTurnR);
		}

		// Generar descendiente de girar a la derecha 45 grados
		nodo hijoSEMITurnR = current;
		hijoSEMITurnR.st.orientacion = (hijoSEMITurnR.st.orientacion + 1) % 8;
		if (Cerrados.find(hijoSEMITurnR.st) == Cerrados.end())
		{
			hijoSEMITurnR.secuencia.push_back(actSEMITURN_R);
			Abiertos.push(hijoSEMITurnR);
		}

		// Generar descendiente de girar a la izquierda 90 grados
		nodo hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion + 6) % 8;
		if (Cerrados.find(hijoTurnL.st) == Cerrados.end())
		{
			hijoTurnL.secuencia.push_back(actTURN_L);
			Abiertos.push(hijoTurnL);
		}

		// Generar descendiente de girar a la izquierda 45 grados
		nodo hijoSEMITurnL = current;
		hijoSEMITurnL.st.orientacion = (hijoSEMITurnL.st.orientacion + 7) % 8;
		if (Cerrados.find(hijoSEMITurnL.st) == Cerrados.end())
		{
			hijoSEMITurnL.secuencia.push_back(actSEMITURN_L);
			Abiertos.push(hijoSEMITurnL);
		}

		// Generar descendiente de avanzar
		nodo hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st))
		{
			if (Cerrados.find(hijoForward.st) == Cerrados.end())
			{
				hijoForward.secuencia.push_back(actFORWARD);
				Abiertos.push(hijoForward);
			}
		}

		// Tomo el siguiente valor de la Abiertos
		if (!Abiertos.empty())
		{
			current = Abiertos.front();
		}
	}

	cout << "Terminada la busqueda\n";

	if (current.st.fila == destino.fila && current.st.columna == destino.columna)
	{
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else
	{
		cout << "No encontrado plan\n";
	}

	return false;
}

//---------------------- Implementación de la busqueda en coste uniforme ---------------------------

struct ComparaEstadosN2
{
	bool operator()(const estado &a, const estado &n) const
	{
		if ((a.fila > n.fila) or
			(a.fila == n.fila and a.columna > n.columna) or
			(a.fila == n.fila and a.columna == n.columna and a.orientacion > n.orientacion) or
			(a.fila == n.fila and a.columna == n.columna and a.orientacion == n.orientacion and a.llevarBikini > n.llevarBikini) or
			(a.fila == n.fila and a.columna == n.columna and a.orientacion == n.orientacion and a.llevarBikini == n.llevarBikini and a.llevarZapatillas > n.llevarZapatillas))
			return true;
		else
			return false;
	}
};
// Estructura para criterio de ordenar para cola con prioridad
struct ComparaCosteBateria
{
	bool operator()(const nodo &nodo1, const nodo &nodo2)
	{
		return nodo1 < nodo2;
	}
};

// Comproba si es casilla especial
void ComportamientoJugador::EsK_D(estado &st)
{
	char flag = mapaResultado[st.fila][st.columna];

	if (flag == 'K')
	{
		st.llevarBikini = true;
	}

	if (flag == 'D')
	{
		st.llevarZapatillas = true;
	}
}

// devolver coste de bateria segun el accion tomado
int ComportamientoJugador::CosteBateria(nodo &n, const Action &accion)
{
	char flag = mapaResultado[n.st.fila][n.st.columna];
	int coste = 0;

	if (accion == actIDLE)
	{
		coste = 0;
	}

	if (accion == actWHEREIS)
	{
		coste = 200;
	}

	if (accion == actFORWARD)
	{
		switch (flag)
		{
		case 'A':
			if (n.st.llevarBikini)
			{
				coste = 10;
			}
			else
			{
				coste = 200;
			}
			break;
		case 'B':
			if (n.st.llevarZapatillas)
			{
				coste = 15;
			}
			else
			{
				coste = 100;
			}
			break;
		case 'T':
			coste = 2;
			break;
		default:
			coste = 1;
			break;
		}
	}

	if (accion == actTURN_L || accion == actTURN_R)
	{
		switch (flag)
		{
		case 'A':
			if (n.st.llevarBikini)
			{
				coste = 5;
			}
			else
			{
				coste = 500;
			}
			break;
		case 'B':
			if (n.st.llevarZapatillas)
			{
				coste = 1;
			}
			else
			{
				coste = 3;
			}
			break;
		case 'T':
			coste = 2;
			break;
		default:
			coste = 1;
			break;
		}
	}
	if (accion == actSEMITURN_L || accion == actSEMITURN_R)
	{
		switch (flag)
		{
		case 'A':
			if (n.st.llevarBikini)
			{
				coste = 2;
			}
			else
			{
				coste = 300;
			}
			break;
		case 'B':
			if (n.st.llevarZapatillas)
			{
				coste = 1;
			}
			else
			{
				coste = 2;
			}
			break;
		case 'T':
			coste = 1;
			break;
		default:
			coste = 1;
			break;
		}
	}
	return coste;
}

bool ComportamientoJugador::pathFinding_CosteUniforme(const estado &origen, const estado &destino, list<Action> &plan)
{
	// Borro la lista
	cout << "Calculando plan\n";
	plan.clear();
	set<estado, ComparaEstadosN2> Cerrados;							  // Lista de Cerrados
	priority_queue<nodo, vector<nodo>, ComparaCosteBateria> Abiertos; // Lista de Abiertos

	nodo current;
	current.st = origen;
	current.secuencia.empty();
	current.costeUniforme = 0;

	EsK_D(current.st);
	CosteBateria(current, actIDLE);

	Abiertos.push(current);

	while (!Abiertos.empty() && (current.st.fila != destino.fila || current.st.columna != destino.columna))
	{
		Abiertos.pop();

		if (mapaResultado[current.st.fila][current.st.columna] == 'K')
		{
			current.st.llevarZapatillas = false;
			current.st.llevarBikini = true;
		}
		else if (mapaResultado[current.st.fila][current.st.columna] == 'D')
		{
			current.st.llevarZapatillas = true;
			current.st.llevarBikini = false;
		}

		Cerrados.insert(current.st);

		// Generar descendiente de girar a la derecha 90 grados
		nodo hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion + 2) % 8;
		if (Cerrados.find(hijoTurnR.st) == Cerrados.end())
		{
			hijoTurnR.costeUniforme = CosteBateria(hijoTurnR, actTURN_R) + current.costeUniforme;
			hijoTurnR.secuencia.push_back(actTURN_R);
			Abiertos.push(hijoTurnR);
		}

		// Generar descendiente de girar a la derecha 45 grados
		nodo hijoSEMITurnR = current;
		hijoSEMITurnR.st.orientacion = (hijoSEMITurnR.st.orientacion + 1) % 8;
		if (Cerrados.find(hijoSEMITurnR.st) == Cerrados.end())
		{
			hijoSEMITurnR.costeUniforme = CosteBateria(hijoSEMITurnR, actSEMITURN_R) + current.costeUniforme;
			hijoSEMITurnR.secuencia.push_back(actSEMITURN_R);
			Abiertos.push(hijoSEMITurnR);
		}

		// Generar descendiente de girar a la izquierda 90 grados
		nodo hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion + 6) % 8;
		if (Cerrados.find(hijoTurnL.st) == Cerrados.end())
		{
			hijoTurnL.costeUniforme = CosteBateria(hijoTurnL, actTURN_L) + current.costeUniforme;
			hijoTurnL.secuencia.push_back(actTURN_L);
			Abiertos.push(hijoTurnL);
		}

		// Generar descendiente de girar a la izquierda 45 grados
		nodo hijoSEMITurnL = current;
		hijoSEMITurnL.st.orientacion = (hijoSEMITurnL.st.orientacion + 7) % 8;
		if (Cerrados.find(hijoSEMITurnL.st) == Cerrados.end())
		{
			hijoSEMITurnL.costeUniforme = CosteBateria(hijoSEMITurnL, actSEMITURN_L) + current.costeUniforme;
			hijoSEMITurnL.secuencia.push_back(actSEMITURN_L);
			Abiertos.push(hijoSEMITurnL);
		}

		// Generar descendiente de avanzar
		nodo hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st))
		{
			if (Cerrados.find(hijoForward.st) == Cerrados.end())
			{
				hijoForward.costeUniforme = CosteBateria(hijoForward, actFORWARD) + current.costeUniforme;
				hijoForward.secuencia.push_back(actFORWARD);
				Abiertos.push(hijoForward);
			}
		}

		// Tomo el siguiente valor de la Abiertos
		if (!Abiertos.empty())
		{
			current = Abiertos.top();
			while (!Abiertos.empty() and Cerrados.find(current.st) != Cerrados.end())
			{
				Abiertos.pop();
				current = Abiertos.top();
			}
		}
	}

	cout << "Terminada la busqueda\n";

	if (current.st.fila == destino.fila && current.st.columna == destino.columna)
	{
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else
	{
		cout << "No encontrado plan\n";
	}

	return false;
}

// Sacar por la consola la secuencia del plan obtenido
void ComportamientoJugador::PintaPlan(list<Action> plan)
{
	auto it = plan.begin();
	while (it != plan.end())
	{
		if (*it == actFORWARD)
		{
			cout << "A ";
		}
		else if (*it == actTURN_R)
		{
			cout << "D ";
		}
		else if (*it == actSEMITURN_R)
		{
			cout << "d ";
		}
		else if (*it == actTURN_L)
		{
			cout << "I ";
		}
		else if (*it == actSEMITURN_L)
		{
			cout << "i ";
		}
		else
		{
			cout << "- ";
		}
		it++;
	}
	cout << endl;
}

// Funcion auxiliar para poner a 0 todas las casillas de una matriz
void AnularMatriz(vector<vector<unsigned char>> &m)
{
	for (int i = 0; i < m[0].size(); i++)
	{
		for (int j = 0; j < m.size(); j++)
		{
			m[i][j] = 0;
		}
	}
}

// Pinta sobre el mapa del juego el plan obtenido
void ComportamientoJugador::VisualizaPlan(const estado &st, const list<Action> &plan)
{
	AnularMatriz(mapaConPlan);
	estado cst = st;

	auto it = plan.begin();
	while (it != plan.end())
	{
		if (*it == actFORWARD)
		{
			switch (cst.orientacion)
			{
			case 0:
				cst.fila--;
				break;
			case 1:
				cst.fila--;
				cst.columna++;
				break;
			case 2:
				cst.columna++;
				break;
			case 3:
				cst.fila++;
				cst.columna++;
				break;
			case 4:
				cst.fila++;
				break;
			case 5:
				cst.fila++;
				cst.columna--;
				break;
			case 6:
				cst.columna--;
				break;
			case 7:
				cst.fila--;
				cst.columna--;
				break;
			}
			mapaConPlan[cst.fila][cst.columna] = 1;
		}
		else if (*it == actTURN_R)
		{
			cst.orientacion = (cst.orientacion + 2) % 8;
		}
		else if (*it == actSEMITURN_R)
		{
			cst.orientacion = (cst.orientacion + 1) % 8;
		}
		else if (*it == actTURN_L)
		{
			cst.orientacion = (cst.orientacion + 6) % 8;
		}
		else if (*it == actSEMITURN_L)
		{
			cst.orientacion = (cst.orientacion + 7) % 8;
		}
		it++;
	}
}

int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}

void ComportamientoJugador::guardarVisitado(Sensores sensores)
{
	mapaResultado[fil][col] = sensores.terreno[0];
	if (brujula == 0)
	{
		mapaResultado[fil - 1][col - 1] = sensores.terreno[1];
		mapaResultado[fil - 1][col] = sensores.terreno[2];
		mapaResultado[fil - 1][col + 1] = sensores.terreno[3];

		mapaResultado[fil - 2][col - 2] = sensores.terreno[4];
		mapaResultado[fil - 2][col - 1] = sensores.terreno[5];
		mapaResultado[fil - 2][col] = sensores.terreno[6];
		mapaResultado[fil - 2][col + 1] = sensores.terreno[7];
		mapaResultado[fil - 2][col + 2] = sensores.terreno[8];

		mapaResultado[fil - 3][col - 3] = sensores.terreno[9];
		mapaResultado[fil - 3][col - 2] = sensores.terreno[10];
		mapaResultado[fil - 3][col - 1] = sensores.terreno[11];
		mapaResultado[fil - 3][col] = sensores.terreno[12];
		mapaResultado[fil - 3][col + 1] = sensores.terreno[13];
		mapaResultado[fil - 3][col + 2] = sensores.terreno[14];
		mapaResultado[fil - 3][col + 3] = sensores.terreno[15];
	}
	else if (brujula == 1) // noreste
	{
		mapaResultado[fil - 1][col] = sensores.terreno[1];
		mapaResultado[fil - 2][col] = sensores.terreno[4];
		mapaResultado[fil - 3][col] = sensores.terreno[9];

		mapaResultado[fil][col + 1] = sensores.terreno[3];
		mapaResultado[fil - 1][col + 1] = sensores.terreno[2];
		mapaResultado[fil - 2][col + 1] = sensores.terreno[5];
		mapaResultado[fil - 3][col + 1] = sensores.terreno[10];

		mapaResultado[fil][col + 2] = sensores.terreno[8];
		mapaResultado[fil - 1][col + 2] = sensores.terreno[7];
		mapaResultado[fil - 2][col + 2] = sensores.terreno[6];
		mapaResultado[fil - 3][col + 2] = sensores.terreno[11];

		mapaResultado[fil][col + 3] = sensores.terreno[15];
		mapaResultado[fil - 1][col + 3] = sensores.terreno[14];
		mapaResultado[fil - 2][col + 3] = sensores.terreno[13];
		mapaResultado[fil - 3][col + 3] = sensores.terreno[12];
	}
	else if (brujula == 2) // este
	{
		mapaResultado[fil - 1][col + 1] = sensores.terreno[1];
		mapaResultado[fil][col + 1] = sensores.terreno[2];
		mapaResultado[fil + 1][col + 1] = sensores.terreno[3];

		mapaResultado[fil - 2][col + 2] = sensores.terreno[4];
		mapaResultado[fil - 1][col + 2] = sensores.terreno[5];
		mapaResultado[fil][col + 2] = sensores.terreno[6];
		mapaResultado[fil + 1][col + 2] = sensores.terreno[7];
		mapaResultado[fil + 2][col + 2] = sensores.terreno[8];

		mapaResultado[fil - 3][col + 3] = sensores.terreno[9];
		mapaResultado[fil - 2][col + 3] = sensores.terreno[10];
		mapaResultado[fil - 1][col + 3] = sensores.terreno[11];
		mapaResultado[fil][col + 3] = sensores.terreno[12];
		mapaResultado[fil + 1][col + 3] = sensores.terreno[13];
		mapaResultado[fil + 2][col + 3] = sensores.terreno[14];
		mapaResultado[fil + 3][col + 3] = sensores.terreno[15];
	}
	else if (brujula == 3) // sureste
	{
		mapaResultado[fil][col + 1] = sensores.terreno[1];
		mapaResultado[fil][col + 2] = sensores.terreno[4];
		mapaResultado[fil][col + 3] = sensores.terreno[9];

		mapaResultado[fil + 1][col] = sensores.terreno[3];
		mapaResultado[fil + 2][col] = sensores.terreno[8];
		mapaResultado[fil + 3][col] = sensores.terreno[15];

		mapaResultado[fil + 1][col + 1] = sensores.terreno[2];
		mapaResultado[fil + 2][col + 1] = sensores.terreno[7];
		mapaResultado[fil + 3][col + 1] = sensores.terreno[14];

		mapaResultado[fil + 1][col + 2] = sensores.terreno[5];
		mapaResultado[fil + 2][col + 2] = sensores.terreno[6];
		mapaResultado[fil + 3][col + 2] = sensores.terreno[13];

		mapaResultado[fil + 1][col + 3] = sensores.terreno[10];
		mapaResultado[fil + 2][col + 3] = sensores.terreno[11];
		mapaResultado[fil + 3][col + 3] = sensores.terreno[12];
	}
	else if (brujula == 4) // sur
	{
		mapaResultado[fil + 1][col + 1] = sensores.terreno[1];
		mapaResultado[fil + 1][col] = sensores.terreno[2];
		mapaResultado[fil + 1][col - 1] = sensores.terreno[3];

		mapaResultado[fil + 2][col + 2] = sensores.terreno[4];
		mapaResultado[fil + 2][col + 1] = sensores.terreno[5];
		mapaResultado[fil + 2][col] = sensores.terreno[6];
		mapaResultado[fil + 2][col - 1] = sensores.terreno[7];
		mapaResultado[fil + 2][col - 2] = sensores.terreno[8];

		mapaResultado[fil + 3][col + 3] = sensores.terreno[9];
		mapaResultado[fil + 3][col + 2] = sensores.terreno[10];
		mapaResultado[fil + 3][col + 1] = sensores.terreno[11];
		mapaResultado[fil + 3][col] = sensores.terreno[12];
		mapaResultado[fil + 3][col - 1] = sensores.terreno[13];
		mapaResultado[fil + 3][col - 2] = sensores.terreno[14];
		mapaResultado[fil + 3][col - 3] = sensores.terreno[15];
	}
	else if (brujula == 5) // suroeste
	{

		mapaResultado[fil + 1][col] = sensores.terreno[1];
		mapaResultado[fil + 2][col] = sensores.terreno[4];
		mapaResultado[fil + 3][col] = sensores.terreno[9];

		mapaResultado[fil][col - 1] = sensores.terreno[3];
		mapaResultado[fil + 1][col - 1] = sensores.terreno[2];
		mapaResultado[fil + 2][col - 1] = sensores.terreno[5];
		mapaResultado[fil + 3][col - 1] = sensores.terreno[10];

		mapaResultado[fil][col - 2] = sensores.terreno[8];
		mapaResultado[fil + 1][col - 2] = sensores.terreno[7];
		mapaResultado[fil + 2][col - 2] = sensores.terreno[6];
		mapaResultado[fil + 3][col - 2] = sensores.terreno[11];

		mapaResultado[fil][col - 3] = sensores.terreno[15];
		mapaResultado[fil + 1][col - 3] = sensores.terreno[14];
		mapaResultado[fil + 2][col - 3] = sensores.terreno[13];
		mapaResultado[fil + 3][col - 3] = sensores.terreno[12];
	}

	else if (brujula == 6) // oeste
	{
		mapaResultado[fil + 1][col - 1] = sensores.terreno[1];
		mapaResultado[fil][col - 1] = sensores.terreno[2];
		mapaResultado[fil - 1][col - 1] = sensores.terreno[3];

		mapaResultado[fil + 2][col - 2] = sensores.terreno[4];
		mapaResultado[fil + 1][col - 2] = sensores.terreno[5];
		mapaResultado[fil][col - 2] = sensores.terreno[6];
		mapaResultado[fil - 1][col - 2] = sensores.terreno[7];
		mapaResultado[fil - 2][col - 2] = sensores.terreno[8];

		mapaResultado[fil + 3][col - 3] = sensores.terreno[9];
		mapaResultado[fil + 2][col - 3] = sensores.terreno[10];
		mapaResultado[fil + 1][col - 3] = sensores.terreno[11];
		mapaResultado[fil][col - 3] = sensores.terreno[12];
		mapaResultado[fil - 1][col - 3] = sensores.terreno[13];
		mapaResultado[fil - 2][col - 3] = sensores.terreno[14];
		mapaResultado[fil - 3][col - 3] = sensores.terreno[15];
	}
	else if (brujula == 7) // noroeste
	{
		mapaResultado[fil][col - 1] = sensores.terreno[1];
		mapaResultado[fil][col - 2] = sensores.terreno[4];
		mapaResultado[fil][col - 3] = sensores.terreno[9];

		mapaResultado[fil - 1][col] = sensores.terreno[3];
		mapaResultado[fil - 2][col] = sensores.terreno[8];
		mapaResultado[fil - 3][col] = sensores.terreno[15];

		mapaResultado[fil - 1][col - 1] = sensores.terreno[2];
		mapaResultado[fil - 2][col - 1] = sensores.terreno[7];
		mapaResultado[fil - 3][col - 1] = sensores.terreno[14];

		mapaResultado[fil - 1][col - 2] = sensores.terreno[5];
		mapaResultado[fil - 2][col - 2] = sensores.terreno[6];
		mapaResultado[fil - 3][col - 2] = sensores.terreno[13];

		mapaResultado[fil - 1][col - 3] = sensores.terreno[10];
		mapaResultado[fil - 2][col - 3] = sensores.terreno[11];
		mapaResultado[fil - 3][col - 3] = sensores.terreno[12];
	}
}

// funcion auxiliar para colocar objetivo;
estado colocarObjetivo(const vector<vector<unsigned char>> &mapa)
{
	bool salida = false;
	estado solu;
	solu.fila = -1;
	solu.columna = -1;
	for (int f = 3; f < mapa.size() - 3 && !salida; f++)
	{
		for (int col = 3; col < mapa[0].size() - 3 && !salida; col++)
		{
			if (mapa[f][col] == '?')
			{
				salida = true;
				solu.fila = f;
				solu.columna = col;
			}
		}
	}
	return solu;
}
