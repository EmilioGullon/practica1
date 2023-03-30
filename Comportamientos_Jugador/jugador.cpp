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

	if(sensores.reset){
		this->bien_situado=false;
		this->con_bikini=false;
		this->con_zapatillas=false;
		while (!this->Cola_acciones.empty())
		{
			this->Cola_acciones.pop();
		}
		last_action=actIDLE;
	}
	//Cuando haya un sensor.choque que se active 
	//Se vuelve a meter la ultima accion en la cola de acciones pero al principio
	if(sensores.colision){
		int N=this->Cola_acciones.size();
		this->Cola_acciones.push(last_action);
		for (int i = 0; i < N; i++)
		{
			this->Cola_acciones.push(Cola_acciones.front());
			this->Cola_acciones.pop();
		}
		last_action=actIDLE;
	}

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

	if(sensores.terreno[0]=='X' and sensores.bateria<1500){
		last_action=actIDLE;
		return actIDLE;
	}
	if(sensores.terreno[0]=='X' and sensores.bateria>=1500){
		if ((sensores.terreno[2]=='T' or sensores.terreno[2]=='S' or sensores.terreno[2]=='G' or sensores.terreno[2]=='B' or sensores.terreno[2]=='X' or sensores.terreno[2]=='D' or sensores.terreno[2]=='K' )  and  sensores.superficie[2]=='_')
			Cola_acciones.push(actFORWARD);	
		else if(sensores.terreno[2]=='A' and con_bikini)
			Cola_acciones.push(actFORWARD);				
			else
				Cola_acciones.push(actTURN_SR);

		if(rand()%2==0)
			Cola_acciones.push(actTURN_SR);
		else
			Cola_acciones.push(actTURN_BR);
	}
	//
	//Metodo siguiente acción 
	//
	if ((sensores.terreno[0]=='G' and !bien_situado) or sensores.nivel==0){
		current_state.fil = sensores.posF;
		current_state.col= sensores.posC;
		current_state.brujula = sensores.sentido;
		bien_situado = true;
	}
	if(sensores.terreno[0]=='K' and !con_bikini)
		con_bikini=true;
	
	if(sensores.terreno[0]=='D' and !con_zapatillas)
		con_zapatillas=true;
	

	if(pintar_bordes){
			// set top and bottom rows to zero
		int N = mapaResultado.size();
		int M = mapaResultado[0].size();
		for (int j = 0; j < N; j++) {
			mapaResultado[0][j] = 'P'; // set to zero
			mapaResultado[1][j] = 'P'; // set to zero
			mapaResultado[2][j] = 'P'; // set to zero
			mapaResultado[M-3][j] = 'P'; // set to zero
			mapaResultado[M-2][j] = 'P'; // set to zero
			mapaResultado[M-1][j] = 'P'; // set to zero
		}
		// set left and right columns to zero
		
		for (int i = 3; i < M-3; i++) {
			mapaResultado[i][0] = 'P'; // set to zero
			mapaResultado[i][1] = 'P'; // set to zero
			mapaResultado[i][2] = 'P'; // set to zero
			mapaResultado[i][N-3] = 'P'; // set to zero
			mapaResultado[i][N-2] = 'P'; // set to zero
			mapaResultado[i][N-1] = 'P'; // set to zero
   		}
		pintar_bordes=false;
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

	bool CasillaEspecial = false;
	bool SeguirRecto = false;
	if((sensores.terreno[2]=='T' or sensores.terreno[2]=='S' or sensores.terreno[2]=='G' or sensores.terreno[2]=='B' or sensores.terreno[2]=='X' or sensores.terreno[2]=='D' or sensores.terreno[2]=='K' )  and  sensores.superficie[2]=='_')
		SeguirRecto = true;
	else if(sensores.terreno[2]=='A' and con_bikini)
		SeguirRecto = true;

	if(Cola_acciones.empty()){
		cout<<"Buscando casillas..."<<endl;
		pair<map<int,CasillaVision>,vector<CasillaVision>> movimientos = BuscarCasillaObjetivo(sensores,current_state,CasillaEspecial);
		if(CasillaEspecial){
			Cola_acciones = BuscarMovimientos(movimientos);
			girar_estrella = false;	
		}
		else{
			cout<<"No hay casillas especiales"<<endl;

			if(SeguirRecto)
				if(!girar_estrella){
					Cola_acciones.push(actTURN_BL);	
					girar_estrella=true;
				}
				else{
					Cola_acciones.push(actFORWARD);
				}

			else
				Cola_acciones.push(actTURN_BR);			
		}
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



pair<map<int,CasillaVision>,vector<CasillaVision>> ComportamientoJugador::BuscarCasillaObjetivo(Sensores s, const state &st,bool &CasillaEspecial){
	map<int,CasillaVision> p;
	vector<CasillaVision> muros;
	for (int i = 1; i < 16; i++){
		int dist = sqrt(i);
		CasillaVision c;
		c.dist=dist;
		c.pos=i-(c.dist+1)*c.dist;		//La mitad
		c.tipo=s.terreno[i];
		cout<<"Casilla que se analiza es: "<<c.tipo<<" "<<i<<" tipo = "<<" dist = "<<c.dist<<" pos = "<<c.pos<<endl;
		switch (c.tipo)
		{
		// POSICION G
		case 'G': 
			if(!this->bien_situado){
				p[1]=c;
				cout<<"Tipo G: "<<i<<" -> "<<c.dist<<" "<<c.pos<<endl;
				CasillaEspecial=true;
			}
			break;
		// RECARGA X
		case 'X':
			if(s.bateria<1150){
				p[2]=c;
				CasillaEspecial=true;				
			}
			break;
		//  BIKINI
		case 'K':
			if(!this->con_bikini){
				p[3]=c;
				CasillaEspecial=true;			
			}
			break;
		//  ZAPATILLAS
		case 'D':
			if(!this->con_zapatillas){				
				CasillaEspecial=true;
				p[4]=c;
				}	
			break;
		// SUELO
		case 'S':
			if(p.find(5)==p.end())
				p[5]=c;
			if (c.dist==2&&c.pos==3)
			{
				/* code */
			}
			
			break;
		// TIERRA
		case 'T':
			if(p.find(5)==p.end())
			p[5]=c;
			break;
		// BOSQUE
		case 'B':
			if(this->con_zapatillas)
				if(p.find(6)==p.end())	
					p[6]=c;				
			else
				if(p.find(7)==p.end())
					p[7]=c;
			break;
		case 'A':
		// AGUA
			/* si tiene el bikini bien  */
			if(this->con_bikini)
				if(p.find(6)==p.end())
					p[6]=c;
			else
				if(p.find(8)==p.end())
					p[8]=c;
			break;
		// MURO
		case 'M':
			//Guardar como muro
			cout<<"Entra el muro "<<endl;
			muros.push_back(c);
			break;
		// PRECIPICIO
		case 'P':
			//guardar como muro
			cout<<"Entra el muro "<<endl;			
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
	cout<<"Tamaño del vector muros "<<p.second.size()<<endl;
	int lvl[4]={0,0,0,0};    //Hay 4 el primero siempre estara vacio me ahorro cambiar el código 
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
	cout<<pp.front()<<"Tamaño de :  "<<pp.size()<<endl;

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
		if(pos != -Nvl){
			cout<<"Metemos un TURN_BR"<<"--";			
			devolver.push(actTURN_BR);
		}
		if (pos<0)
		{
			pos=-pos;
		}

		pos=3-pos;
		

		for (int i = 0; i < pos; i++) //abs devuelve el valor absoluto
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
		if(pos!=Nvl){
			cout<<"Metemos un TURN_BL"<<"--";			
			devolver.push(actTURN_BL);
		}

		pos=3-pos;

		for (int i = 0; i < pos; i++) //abs devuelve el valor absoluto
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

