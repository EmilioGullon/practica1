#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"
#include <map>
#include <queue>
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
      pintar_bordes=true;
      bien_situado= false,
      con_zapatillas=false,
      con_bikini=false;
      girar_estrella=false;
    }

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}

    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);
    pair<map<int,CasillaVision>,vector<CasillaVision>> BuscarCasillaObjetivo(Sensores s, const state &st,bool &CasillaEspecial);

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
	bool pintar_bordes,con_zapatillas,con_bikini,girar_estrella,
  bien_situado;
  
  
};


queue<Action> BuscarMovimientos(pair<map<int,CasillaVision>,vector<CasillaVision>> p);
queue<Action> GirarDetras();
queue<Action> CasoCentro(int pos,int Nvl,vector<CasillaVision> p,bool darVuelta);
queue<Action> CasoIzq(int pos,int Nvl,vector<CasillaVision> p,bool darVuelta);
queue<Action> CasoDrch(int pos,int Nvl,vector<CasillaVision> p,bool darVuelta);

void PonerTerrenoEnMatriz(const vector<unsigned char> &terreno, const state &st, vector< vector<unsigned char> > &matriz);


#endif
