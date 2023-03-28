#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"
#include <map>
using namespace std;

struct state{
  int fil;
  int col;
  Orientacion brujula;
};

struct CasillaVision{
  int dist, pos; 
  char tipo;
};


class ComportamientoJugador : public Comportamiento{
  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size){
      // Constructor de la clase
      // Dar el valor inicial a las variables de estado
      current_state.fil=current_state.col=99;
      last_action = actIDLE;
      current_state.brujula = norte;
      girar_derecha=false;
      bien_situado= false,
      con_zapatillas=false,
      con_bikini=false;
    }

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}

    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);
    pair<map<int,CasillaVision>,vector<CasillaVision>> BuscarCasillaObjetivo(Sensores s, const state &st);

  private:
  /*
  // Declarar aquí las variables de estado
  int vida;
  int bateria;
  bool colision;
  bool reset;
  int posF;
  int posC;
  Orientacion sentido; // Orientacion
  double tiempo;
  vector< unsigned char> terreno;
  vector< unsigned char> superficie;
  */

  state current_state;
  Action last_action;
  queue<Action> Cola_acciones;
	bool girar_derecha,con_zapatillas,con_bikini,
  bien_situado;
  
  
};



void PonerTerrenoEnMatriz(const vector<unsigned char> &terreno, const state &st, vector< vector<unsigned char> > &matriz);


#endif
