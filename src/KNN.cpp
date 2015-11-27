/*
 * KNN.cpp
 *
 *  Created on: Nov 25, 2015
 *      Author: flor
 */

#include "KNN.h"

KNN::KNN() {
	// TODO Auto-generated constructor stub
	KNN::k=0;
}

KNN::~KNN() {
	// TODO Auto-generated destructor stub
}

//Pregunta: que pasa con los datos de cordenadas erroneas??? (x:-120.5,y:90.0)
void KNN::aplicarKNN(int valorK/*, setTrain, setTest*/){
	//Ingresar Set de Entrenamiento (Agregar a declaracion) y Testeo (Agregar a declaracion)
	//y valor de k a evaluar
 	//Devuelve por cada instancia probabilidades por categoria(?)(Agregar a declaracion)
	//Solo se necesita Categoria, Distrito PD, x, y

	KNN::k = valorK;
	entrenar(/*setTrain*/);
	//return evaluar(/*setTest */);
}

void KNN::entrenar(/*setTrain*/) {
	//Ingresar Set de Entrenamiento (Agregar a declaracion)

	//Separa por PdDistric (aunque no se si no habra una forma de separalos en grupos + chicos)
	//No se en que formato tratarlo dos opciones:
	//Mismo formato que como recibo los datos separando en los 10 districtos,
	//quedandome solo con x,y,Category (pensar que tengo que hacer muchos accesos);
	//o la otra opcion es armar 10 vectores por distrito que contengan x,y,Category
	//(es mucho para la memoria de la maquina??)
}

void KNN::evaluar(/*setTest */) {
	//Ingresar Set de Testeo (Agregar a declaracion)
	//Devuelve por cada instancia probabilidades por categoria(?)(Agregar a declaracion)
	//Busca los k vecinos mas cercanos (distancia euclediana)
	//y de ellos obtiene probabilidades de clasificar en cada categoria

	//no se como recibo el test
	//Recorrido TestSet:
	//Obtengo una instacia del set Test
	//Vector de K estructuras con Categoria y distancia:
	vector<distVecino> vecinos = buscarVecinos(/*instanciaTest*/);
	//Vector con frecuencias por categoria (en orden alfabetico de las categorias|para
	//obtener probabilidades solo hay que dividirlo por k):
	vector<int> frecuenciasCategoria = contarCategoria(vecinos);
	//armar Instancia de salida con numero de instancia de test y vector fecuenciasCategoria
	//almacenar resultado en la estructura/formato que corresponda
	//Buscar siguiente instancia y repetir

	//return estructuraResultado; (poner nombre que convenga)
}

/*formatoTrain obtenerTrainNecesario(string pdDistrict) {
	//Ajustar segun formato y descomentar

	if(pdDistrict == "NORTHERN") return trainNorthern;
 	if(pdDistrict == "PARK") return trainPark;
  	if(pdDistrict == "INGLESIDE") return trainIngleside;
  	if(pdDistrict == "BAYVIEW") return trainBayview;
  	if(pdDistrict == "RICHMOND") return trainRichmond;
  	if(pdDistrict == "CENTRAL") return trainCentral;
  	if(pdDistrict == "TARAVAL") return trainTaraval;
  	if(pdDistrict == "TENDERLOIN") return trainTenderloin;
  	if(pdDistrict == "MISSION") return trainMission;
  	if(pdDistrict == "SOUTHERN") return trainSouthern;
 }
 */

float calcularDistancia(float xtest, float xtrain, float ytest, float ytrain){
	return sqrt(pow(xtrain-xtest,2)+pow(ytrain-ytest,2));
}

int buscarMayor(vector<KNN::distVecino> vecinos) {
	int i,posMayor;
	KNN::distVecino vecino;
	float distanciaMayor = 0;
	for (i = 0; i < KNN::k; i++) {
		vecino = vecinos[i];
		if (distanciaMayor > vecino.distancia){
			posMayor = i;
			distanciaMayor = vecino.distancia;
		}
	}
	return posMayor;
}

vector<KNN::distVecino> buscarVecinos(/* InstanciaTest*/) {
	//float xtest = xdeInstanciaTest;
	//float ytest = ydeInstanciaTest;
	//string pdDistrict = pdDistrictDeInstanciaTest;

	KNN::distVecino vecinoTrain;
	vector<KNN::distVecino> vecinos;
	//formatoTrain trainActual = obtenerTrainNecesario(pdDistrict);
	//Recorrido trainActual:
		//obtengo instancia trainActual
		//float xtrain = xdeInstanciaTrain;
		//float ytrain = ydeInstanciaTrain;
		//vecinoTrain.categoria = categoriadeInstanciaTrain;
		//vecinoTrain.distancia = calcularDistancia(xtest,xtrain,ytest,ytrain);
		KNN::distVecino vecino;
		if ((int)vecinos.size() >= KNN::k) {
			int posMayor;
			posMayor = buscarMayor(vecinos);
			vecino = vecinos[posMayor];
			if (vecino.distancia > vecinoTrain.distancia){
				vecinos[posMayor] = vecinoTrain;
			}
		} else {
			int pos = vecinos.size();
			vecinos[pos] = vecinoTrain;
		}
	//fin recorrido

	return vecinos;
}

int obtenerNumCategoria(string categoria){
	//Hay alguna forma mas corta de hacer esto?????
	if (categoria == "ARSON") return 0;
	if (categoria == "ASSAUTL") return 1;
	if (categoria == "BAD CHECKS") return 2;
	if (categoria == "BRIBERY") return 3;
	if (categoria == "BURGLARY") return 4;
	if (categoria == "DISORDERLY CONDUCT") return 5;
	if (categoria == "DRIVING UNDER THE INFLUENCE") return 6;
	if (categoria == "DRUG/NARCOTIC") return 7;
	if (categoria == "DRUNKENNESS") return 8;
	if (categoria == "EMBEZZLEMENT") return 9;
	if (categoria == "EXTORTION") return 10;
	if (categoria == "FAMILY OFFENSES") return 11;
	if (categoria == "FORGERY/COUNTERFEITING") return 12;
	if (categoria == "FRAUD") return 13;
	if (categoria == "GAMBLING") return 14;
	if (categoria == "KIDNAPPING") return 15;
	if (categoria == "LARCENY/THEFT") return 16;
	if (categoria == "LIQUOR LAWS") return 17;
	if (categoria == "LOITERING") return 18;
	if (categoria == "MISSING PERSON") return 19;
	if (categoria == "NON-CRIMINAL") return 20;
	if (categoria == "OTHER OFFENSES") return 21;
	if (categoria == "PORNOGRAPHY/OBSCENE MAT") return 22;
	if (categoria == "PROSTITUTION") return 23;
	if (categoria == "RECOVERED VEHICLE") return 24;
	if (categoria == "ROBBERY") return 25;
	if (categoria == "RUNAWAY") return 26;
	if (categoria == "SECONDARY CODES") return 27;
	if (categoria == "SEX OFFENSES FORCIBLE") return 28;
	if (categoria == "SEX OFFENSES NON FORCIBLE") return 29;
	if (categoria == "STOLEN PROPERTY") return 30;
	if (categoria == "SUICIDE") return 31;
	if (categoria == "SUSPICIOUS OCC") return 32;
	if (categoria == "TREA") return 33;
	if (categoria == "TRESPASS") return 34;
	if (categoria == "VANDALISM") return 35;
	if (categoria == "VEHICLE THEFT") return 36;
	if (categoria == "WARRANTS") return 37;
	if (categoria == "WEAPON LAWS") return 38;
	return 21;//Si es una categoria desconocida la cuenta como OTHER OFFENSES
}

vector<int> contarCategoria(vector<KNN::distVecino> vecinos) {
	vector<int> frecCat(39, 0);
	int i,pos;
	KNN::distVecino vecino;
	for (i = 0 ; i < (int)vecinos.size(); i++){
		vecino = vecinos[i];
		pos = obtenerNumCategoria(vecino.categoria);
		frecCat[pos] += 1;
	}
	return frecCat;
}
