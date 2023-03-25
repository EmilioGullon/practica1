#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
#include <cmath> 
#include <map>

using namespace std;



Action ComportamientoJugador::think(Sensores sensores){

	Action accion=actFORWARD;

	cout << "Posicion: fila " << sensores.posF << " columna " << sensores.posC << " ";
	switch(sensores.sentido){
		case 0: cout << "Norte" << endl; break;
		case 1: cout << "Noreste" << endl; break;
		case 2: cout << "Este" << endl; break;
		case 3: cout << "Sureste" << endl; break;
		case 4: cout << "Sur " << endl; break;
		case 5: cout << "Suroeste" << endl; break;
		case 6: cout << "Oeste" << endl; break;
		case 7: cout << "Noroeste" << endl; break;
	}
	cout << "Terreno: ";
	for (int i=0; i<sensores.terreno.size(); i++)
		cout << sensores.terreno[i];
	cout << endl;

	cout << "Superficie: ";
	for (int i=0; i<sensores.superficie.size(); i++)
		cout << sensores.superficie[i];
	cout << endl;

	cout << "Colisión: " << sensores.colision << endl;
	cout << "Reset: " << sensores.reset << endl;
	cout << "Vida: " << sensores.vida << endl;
	cout << endl;

	int a;
	// Determinar el efecto de la ultima accion enviada
	switch (last_action){
	case actFORWARD:
	switch (current_state.brujula){
	case norte: current_state.fil--; break;
	case noreste: current_state.fil--; current_state.col++; break;
	case este: current_state.col++; break;
	case sureste: current_state.fil++;current_state.col++; break;
	case sur: current_state.fil++; break;
	case suroeste: current_state.fil++;current_state.col--; break;
	case oeste: current_state.col--; break;
	case noroeste: current_state.fil--;current_state.col--; break;
	}	
	break;
	case actTURN_SL:
		a = current_state.brujula;
		a = (a+7)%8;
		current_state.brujula = static_cast<Orientacion>(a);
	break;
	case actTURN_SR:
		a = current_state.brujula;
		a = (a+1)%8;
		current_state.brujula = static_cast<Orientacion>(a);
	break;
	case actTURN_BL:
	// Actualización de girar 135º a la izquierda
		a = current_state.brujula;
		a = (a+5)%8;
		current_state.brujula = static_cast<Orientacion>(a);
	break;
	case actTURN_BR:
	// Actualización en caso de girar 135º a la derecha0
		a = current_state.brujula;
		a = (a+3)%8;
		current_state.brujula = static_cast<Orientacion>(a);
	break;
	}

	//
	//Metodo siguiente acción 
	//
	if (sensores.terreno[0]=='G' and !bien_situado){
		current_state.fil = sensores.posF;
		current_state.col= sensores.posC;
		current_state.brujula = sensores.sentido;
		bien_situado = true;
	}

	if (bien_situado){
		PonerTerrenoEnMatriz(sensores.terreno,current_state,mapaResultado);
	}

	if ((sensores.terreno[2]=='T' or sensores.terreno[2]=='S' or sensores.terreno[2]=='G')  and  sensores.superficie[2]=='_'){
	accion = actFORWARD;
	} else if (!girar_derecha){
	accion = actTURN_SL;
	girar_derecha = (rand()%2 ==0);
	} else{
	accion = actTURN_SR;
	girar_derecha = (rand()%2 ==0);
	}

	//todo
	last_action = accion;

	return accion;
}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}



CasillaVision BuscarCasillaObjetivo(const vector<unsigned char> &terreno, const state &st){
	map<int,CasillaVision> p;
	for (int i = 0; i < 16; i++){
		int dist = sqrt(i);
		int numero = dist*(dist+1);
		if(i!=numero){
			numero=i-(dist+1)*(dist+1);		
		}



		if('G'==terreno[i]){
			CasillaVision c;
			c.dist=dist;
			c.
		}
		
	}
	


	return ;
}




void PonerTerrenoEnMatriz(const vector<unsigned char> &terreno, const state &st, vector< vector<unsigned char> > &matriz){
	int fil, col;
	int cont=1;
	int fil1=0, col1=0;
	matriz[st.fil][st.col]  = terreno [0];
	switch (st.brujula){
	case norte: 
		fil=-1;
		cont=1;
		fil1=0, col1=0;
		for(int i=1;i<4;i++){
			fil1=i;
			for (col1 = -i; col1 <= i; col1++){
				cout<<fil1<<" and "<<col1<<" = "<< cont <<endl;
				matriz[st.fil+fil1*fil][st.col+col1]  = terreno [cont];
				cont++;
			}
		}
	break;
	case noreste: 
		fil=-1;
		col=1;
		cont=1;
		fil1=0, col1=0;
		for(int i=1;i<4;i++){
			fil1=i;
			for (col1 = 0; col1 < i; col1++){
				cout<<fil1<<" and "<<col1<<" = "<< cont <<endl;
				matriz[st.fil+fil1*fil][st.col+col1*col]  = terreno [cont];
				cont++;
			}
			
			for (fil1; fil1 >= 0; fil1--){
				cout<<fil1<<" and "<<col1<<" = "<< cont <<endl;
				matriz[st.fil+fil1*fil][st.col+col1*col]  = terreno [cont];
				cont++;
			}
		}		
	break;
	case este:  
		col=1;
		cont=1;
		fil1=0, col1=0;
		for(int i=1;i<4;i++){
			col1=i;
			for (fil1 = -i; fil1 <= i; fil1++){
				cout<<fil1<<" and "<<col1<<" = "<< cont <<endl;
				matriz[st.fil+fil1][st.col+col1*col]  = terreno [cont];
				cont++;
			}
		}
	break;
	case sureste: 
		fil=1;
		col=1;
		cont=1;
		fil1=0, col1=0;
		for(int i=1;i<4;i++){
			col1=i;
			for (fil1 = 0; fil1 < i; fil1++){
				cout<<fil1<<" and "<<col1<<" = "<< cont <<endl;
				matriz[st.fil+fil1*fil][st.col+col1*col]  = terreno [cont];
				cont++;
			}
			
			for (col1; col1 >= 0; col1--){
				cout<<fil1<<" and "<<col1<<" = "<< cont <<endl;
				matriz[st.fil+fil1*fil][st.col+col1*col]  = terreno [cont];
				cont++;
			}
		}		
	break;
	case sur:  
		fil=1;
		cont=1;
		fil1=0, col1=0;
		for(int i=1;i<4;i++){
			fil1=i;
			for (col1 = -i; col1 <= i; col1++){
				cout<<fil1<<" and "<<col1<<" = "<< cont <<endl;
				matriz[st.fil+fil1*fil][st.col+-col1]  = terreno [cont];
				cont++;
			}
		}
	break;
	case suroeste: 
		fil=1;
		col=-1;
		 cont=1;
		fil1=0, col1=0;
		for(int i=1;i<4;i++){
			fil1=i;
			for (col1 = 0; col1 < i; col1++){
				cout<<fil1<<" and "<<col1<<" = "<< cont <<endl;
				matriz[st.fil+fil1*fil][st.col+col1*col]  = terreno [cont];
				cont++;
			}
			
			for (fil1; fil1 >= 0; fil1--){
				cout<<fil1<<" and "<<col1<<" = "<< cont <<endl;
				matriz[st.fil+fil1*fil][st.col+col1*col]  = terreno [cont];
				cont++;
			}
		}		
	break;
	case oeste: 
		col=-1;
		cont=1;
		fil1=0, col1=0;
		for(int i=1;i<4;i++){
			col1=i;
			for (fil1 = -i; fil1 <= i; fil1++){
				cout<<fil1<<" and "<<col1<<" = "<< cont <<endl;
				matriz[st.fil+-fil1][st.col+col1*col]  = terreno [cont];
				cont++;
			}
		}
	break;
	case noroeste:  
		fil=-1;
		col=-1;
		cont=1;
		fil1=0, col1=0;
		for(int i=1;i<4;i++){
			col1=i;
			for (fil1 = 0; fil1 < i; fil1++){
				cout<<fil1<<" and "<<col1<<" = "<< cont <<endl;
				matriz[st.fil+fil1*fil][st.col+col1*col]  = terreno [cont];
				cont++;
			}
			
			for (col1; col1 >= 0; col1--){
				cout<<fil1<<" and "<<col1<<" = "<< cont <<endl;
				matriz[st.fil+fil1*fil][st.col+col1*col]  = terreno [cont];
				cont++;
			}
		}		
	break;
	}	

	
	
	


	
}

