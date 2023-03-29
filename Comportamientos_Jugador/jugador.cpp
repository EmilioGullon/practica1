#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
#include <cmath> 
#include <map>
#include <vector>
#include <utility>
#include <queue>

using namespace std;




Action ComportamientoJugador::think(Sensores sensores){

	Action accion=actIDLE;

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
	/*
	if ((sensores.terreno[2]=='T' or sensores.terreno[2]=='S' or sensores.terreno[2]=='G')  and  sensores.superficie[2]=='_'){
	accion = actFORWARD;
	} else if (!girar_derecha){
	accion = actTURN_SL;
	girar_derecha = (rand()%2 == 0);
	} else{
	accion = actTURN_SR;
	girar_derecha = (rand()%2 == 0);
	}
	*/
	//todo
	//Terminar cola de acciones 
	if(Cola_acciones.empty()){
		cout<<"Buscado movimiento"<<endl;
		Cola_acciones=BuscarMovimientos(BuscarCasillaObjetivo(sensores,current_state));
	}
	//Si se ha terminado hacer una nueva	
	
	cout<<"Accion a realizar: "<<accion<<" Acciones que quedan : "<<Cola_acciones.size()<<endl;
	accion=Cola_acciones.front();
	Cola_acciones.pop();
	last_action = accion;

	return accion;
}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}



pair<map<int,CasillaVision>,vector<CasillaVision>> ComportamientoJugador::BuscarCasillaObjetivo(Sensores s, const state &st){
	map<int,CasillaVision> p;
	vector<CasillaVision> muros;
	for (int i = 1; i < 16; i++){
		int dist = sqrt(i);
		CasillaVision c;
		c.dist=dist;
		c.pos=i-(c.dist+1)*c.dist;		//La mitad
		c.tipo=s.terreno[i];
		switch (c.tipo)
		{
		case 'G':
			if(!this->bien_situado){
				p[1]=c;
				cout<<"Tipo G: "<<i<<" -> "<<c.dist<<" "<<c.pos<<endl;
			}
			break;
		case 'X':
			if(s.bateria<2500)
				p[2]=c;
			break;
		case 'K':
			if(!this->con_bikini)
				p[3]=c;
			break;
		case 'D':
			if(!this->con_zapatillas)
				p[4]=c;	
			break;
		case 'S':
			p[5]=c;
			break;
		case 'T':
			p[6]=c;
			break;
		case 'B':
			if(this->con_zapatillas)
				p[6]=c;	
			else
				p[7]=c;
			break;
		case 'A':
			/* si tiene el bikini bien  */
			if(this->con_bikini)
				p[6]=c;
			else
				p[8]=c;
			break;
		case 'M':
			//Guardar como muro
			muros.push_back(c);
			break;
		case 'P':
			//guardar como muro
			muros.push_back(c);
			break;																	
		default:
			break;
		}		
	}
	return make_pair(p,muros);
}

queue<Action> BuscarMovimientos(pair<map<int,CasillaVision>,vector<CasillaVision>> p){
	//SI HAY 1 MUROS EN EL CENTRO DEL NIVEL 1 --> GIRAR A LA DERECHA 
	//SI HAY HAY TRES MUROS EN LOS 3 DEL CENTRO EN EL NIVEL DOS --> SI EL OBJETIVO ESTA EN EL NIVEL 1 IR HACIA EL
	//															--> SI NO GIRAR A LA DERECHA
	int lvl[3]={0,0,0};
	bool lvl3=false;
	int dist;
	queue<Action> pp;
	int pos;
	for (int i = 0; i < p.second.size(); i++)
		lvl[p.second[i].dist]++;
	
	if(lvl[1]==3)
		pp = GirarDetras(); //Girar pa tras
	else{
		if(lvl[2]==5)
			lvl3=true;	
		bool enc=false;
		for (auto it = p.first.begin(); it != p.first.end()&&!enc; it++)
		{
			//Si es del lvl3 y el lvl3 esta activado no se mete.
			cout<<it->first<<" es "<<"  "<<it->second.dist<<"  "<<it->second.pos<<"  "<<endl; 
			if (it->second.pos == 3 && !lvl3) {
				pos = it->second.pos;
				dist = it->second.dist;
				enc = true;
			}
			else if (it->second.pos != 3) {
				pos = it->second.pos;
				dist = it->second.dist;
				enc = true;
			}
		}
		// SE DECIDE UN CASO CON EL NVL(DIST) Y POS 	
		// SEGUN LA POSICION SI ES -3,-2 ES CASO IZQ
		// SI ES DE -1 A 1 ES CASO CENTRO 
		// EL RESTO CASO DRCH
		cout<<"Se encontro casilla"<<endl;
		cout<<" | "<<dist<<" | "<<pos<<" | "<<endl;
		if(pos<-1)
			pp = CasoIzq(pos,dist,p.second,false);
		else if(pos<2)
				pp = CasoCentro(pos,dist,p.second,false);
			else
				pp = CasoDrch(pos,dist,p.second,false);
	}
	cout<<endl;
	cout<<pp.front()<<endl;

	return pp;
}

queue<Action> GirarDetras(){
	queue<Action> devolver;
	switch (rand()%2)
	{
	case 0:
		devolver.push(actTURN_BL);
		break;
	case 1:
		devolver.push(actTURN_BR);		
		break;	
	default:
		break;
	}
	cout<<"SE DIO LA VUELTA"<<endl;
	return devolver;
}

//SI HAY ALGUN BLOQUE DE MURO HASTA LLEGAR AL NIVEL 
//		--> SI NO HAY SE COMPRUEBA QUE SE PUEDE GIRAR A LA IZQ O DRCH DEPENDIENDO DE LA POS 
//		--> SI NO SE PUEDE CUALQUIERA DE LAS DOS CONDICIONES SE VA AL CASO IZQ EN CASO DE POS SEA - O DRCH EN EL CASO +
//SI SE VIENE DE UN CASO YA FALLADO EN VEZ DE GIRAR A OTRO CASO SE GIRA DETRAS
queue<Action> CasoCentro(int pos,int Nvl,vector<CasillaVision> p,bool darVuelta){
	cout<<"Se busca por el CENTRO"<<endl;
	bool enc=false;
	queue<Action> devolver;
		for (int i = 0; i < p.size()&&!enc; i++){
		if(p[i].pos==0&&p[i].dist<=Nvl){
			enc=true;
			}
		}
	//Si pudo llegar al nvl en una linea recta
	if(!enc)
		if(pos!=0){
			if(pos<0){
				for (int i = 0; i < p.size()&&!enc; i++)
					if(p[i].pos>=pos&&p[i].dist==Nvl)
						enc=true;
				}
			else{
				for (int i = 0; i < p.size()&&!enc; i++)
					if(p[i].pos<=pos&&p[i].dist==Nvl)
						enc=true;
				}
		}
	//Si pudo llegar a la casilla objetivo si no intentamos con caso izq o drch, o giro hacia detras
	if(enc){
		if(darVuelta)
			devolver=GirarDetras();
		else
			if(pos<0)
				devolver=CasoIzq(pos,Nvl,p,true);
			else
				devolver=CasoDrch(pos,Nvl,p,true);
	}
	else{
		// BUCLE QUE AÑADA INSTRUCCIONES DE SEGUIR RECTO HASTA LLEGAR AL NLV 	
		// SI NO ES CERO GIRAR 90º DOS INST DE GIRAR Y AVANZAR HASTA LA CASILLA DE POS
		// SI ES CERO HA LLEGADO
		for (int i = 0; i < Nvl; i++){
			cout<<"Metemos un FORWARD"<<"--";			
			devolver.push(actFORWARD);
		}
		if(pos<0){
			cout<<"Metemos dos TURN_SL"<<"--";			
			devolver.push(actTURN_SL);
			devolver.push(actTURN_SL);
		}
		else if(pos>0){
			cout<<"Metemos dos TURN_SR"<<"--";			
			devolver.push(actTURN_SR);
			devolver.push(actTURN_SR);			
		}
		for (int i = 0; i < abs(pos); i++) //abs devuelve el valor absoluto
		{
			cout<<"Metemos un FORWARD"<<"--";			
			devolver.push(actFORWARD);
		}	
	}
	return devolver;
}

queue<Action> CasoIzq(int pos,int Nvl,vector<CasillaVision> p,bool darVuelta){
	cout<<"Se busca por el IZQ"<<endl;
	bool enc=false;
	queue<Action> devolver;
	for (int i = 0; i < p.size()&&!enc; i++)
		if(p[i].pos==-p[i].dist&&p[i].dist<=Nvl)
			enc=true;
	//Si pudo llegar al nvl en una linea recta
	if(!enc)
		if(pos!=-Nvl)
			for (int i = 0; i < p.size()&&!enc; i++)
				if(p[i].pos<=pos&&p[i].dist==Nvl)
					enc=true;
	//Si pudo llegar a la casilla objetivo si no intentamos con caso izq o drch, o giro hacia detras
	if(enc){
		if(darVuelta)
			devolver=GirarDetras();
		else
			devolver=CasoCentro(pos,Nvl,p,true);
	}
	else{
		// En esta ocasión primero gira.
		cout<<"Metemos un actTURN_SL"<<endl;
		devolver.push(actTURN_SL);
		// BUCLE QUE AÑADA INSTRUCCIONES DE SEGUIR RECTO HASTA LLEGAR AL NLV 	
		// SI NO ES CERO GIRAR 90º DOS INST DE GIRAR Y AVANZAR HASTA LA CASILLA DE POS
		// SI ES CERO HA LLEGADO
		for (int i = 0; i < Nvl; i++)
		{
			cout<<"Metemos un FORWARD"<<"--";			
			devolver.push(actFORWARD);
		}
		if(pos=!-Nvl){
			cout<<"Metemos un TURN_BR"<<"--";			
			devolver.push(actTURN_BR);
		}
		for (int i = 0; i < abs(pos); i++) //abs devuelve el valor absoluto
		{
			cout<<"Metemos un FORWARD"<<"--";			
			devolver.push(actFORWARD);
		}	
	}
	return devolver;
}

queue<Action> CasoDrch(int pos,int Nvl,vector<CasillaVision> p,bool darVuelta){
	cout<<"Se busca por el DRCH"<<endl;	
	bool enc=false;
	queue<Action> devolver;
	for (int i = 0; i < p.size()&&!enc; i++)
		if(p[i].pos==p[i].dist&&p[i].dist<=Nvl)
			enc=true;
	//Si pudo llegar al nvl en una linea recta
	if(!enc)
		if(pos!=Nvl)
			for (int i = 0; i < p.size()&&!enc; i++)
				if(p[i].pos>=pos&&p[i].dist==Nvl)
					enc=true;
	//Si pudo llegar a la casilla objetivo si no intentamos con caso izq o drch, o giro hacia detras
	if(enc){
		if(darVuelta)
			devolver=GirarDetras();
		else
			devolver=CasoCentro(pos,Nvl,p,true);
	}
	else{
		// En esta ocasión primero gira.
		cout<<"Metemos un actTURN_SR"<<endl;		
		devolver.push(actTURN_SR);
		// BUCLE QUE AÑADA INSTRUCCIONES DE SEGUIR RECTO HASTA LLEGAR AL NLV 	
		// SI NO ES CERO GIRAR 90º DOS INST DE GIRAR Y AVANZAR HASTA LA CASILLA DE POS
		// SI ES CERO HA LLEGADO
		for (int i = 0; i < Nvl; i++)
		{
			cout<<"Metemos un FORWARD"<<"--";
			devolver.push(actFORWARD);
		}
		if(pos=!Nvl){
			cout<<"Metemos un TURN_BL"<<"--";			
			devolver.push(actTURN_BL);
		}
		for (int i = 0; i < abs(pos); i++) //abs devuelve el valor absoluto
		{
			cout<<"Metemos un FORWARD"<<"--";			
			devolver.push(actFORWARD);
		}	
	}
	return devolver;
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
				matriz[st.fil+fil1*fil][st.col+col1*col]  = terreno [cont];
				cont++;
			}
			
			for (fil1; fil1 >= 0; fil1--){
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
				matriz[st.fil+fil1*fil][st.col+col1*col]  = terreno [cont];
				cont++;
			}
			
			for (col1; col1 >= 0; col1--){
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
				matriz[st.fil+fil1*fil][st.col+col1*col]  = terreno [cont];
				cont++;
			}
			
			for (fil1; fil1 >= 0; fil1--){
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
				matriz[st.fil+fil1*fil][st.col+col1*col]  = terreno [cont];
				cont++;
			}
			
			for (col1; col1 >= 0; col1--){
				matriz[st.fil+fil1*fil][st.col+col1*col]  = terreno [cont];
				cont++;
			}
		}		
	break;
	}	

	
	
	


	
}

