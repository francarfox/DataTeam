/*
 * KNN.cpp
 *
 *  Created on: Nov 25, 2015
 *      Author: flor
 */

#include "KNN.h"

KNN::KNN(int valorK, SetterData setterDataTrain) {
	k = valorK;
	setterData = setterDataTrain;
}

KNN::~KNN() { }

void KNN::aplicarKNN(string trainFileName, string testFileName){
 	//Devuelve por cada instancia probabilidades por categoria(?)(Agregar a declaracion)
	entrenar(trainFileName);

	return evaluar(testFileName);
}

void KNN::entrenar(string trainFileName) {
	//Transformo Set de Entrenamiento en archivos binarios por distrito de los campos necesarios para realizar knn
	//Campos utilizados: Category, PdDistrict, x, y

	fstream train(trainFileName.c_str(),ios::in);

	//Separa por PdDistric (aunque no se si no habra una forma de separalos en grupos + chicos)
	if (train.is_open()) {
		//Recorre train y guarda campos en archivos binarios con registro ptoDistrito
		vector<string> nombresDeCampos = obtenerNombresDeCamposDePrimerLinea(train);
		int posCategoria, posPdDistrict, posX ,posY;
		obtenerPosicionesDeCamposNecesarios(nombresDeCampos,posCategoria,posPdDistrict,posX,posY);
		while (train.good()) {
			vector<string> datos = obtenerDatosDeLinea(train);
			ptoDistrito instanciaTrainDist;
			instanciaTrainDist.x = strtof(datos[posX].c_str(),NULL);
			instanciaTrainDist.y = strtof(datos[posY].c_str(),NULL);
			if((instanciaTrainDist.x != X_INVALID) && (instanciaTrainDist.y != Y_INVALID)) {
			//Me quedo solo con los campos validos, los demas los descarto
				instanciaTrainDist.categoria = setterData.getInt(datos[posCategoria]);
				int numPdDistrict = setterData.getInt(datos[posPdDistrict]);
				string pdDistrict = setterData.getPdDistrictName(numPdDistrict);
				grabarTrainEnCorrespondiente(pdDistrict,instanciaTrainDist);
			}
		}
	}
	train.close();
}

vector<string> KNN::obtenerNombresDeCamposDePrimerLinea(fstream &train) {
	vector<string> campos;
	string primeraLinea;
	getline(train,primeraLinea);
	size_t pos = -1;
	size_t posComa = 0;
	while (posComa != string::npos) {
		pos = posComa + 1;
		posComa = primeraLinea.find(",",pos);
		campos.push_back(primeraLinea.substr(pos,posComa));
	}
	campos.push_back(primeraLinea.substr(pos));
	return campos;
}

vector<string> KNN::obtenerDatosDeLinea(fstream &train) {
	string linea;
	getline(train,linea);
	vector<string> datos;
	size_t pos = -1;
	size_t posComa = 0;
	while (posComa != string::npos) {
		pos = posComa + 1;
		posComa = linea.find(",",pos);
		datos.push_back(linea.substr(pos,posComa));
	}
	datos.push_back(linea.substr(pos));
	return datos;
}

void KNN::obtenerPosicionesDeCamposNecesarios
 (vector<string> &nombresDeCampos, int &posCategoria, int &posDistrito, int &posX, int &posY) {
	for (int i = 0; i < (int)nombresDeCampos.size(); i++){
		if(nombresDeCampos[i] == "Category") {
			posCategoria = i;
		}
		if(nombresDeCampos[i] == "PdDistrict") {
				posDistrito = i;
		}
		if(nombresDeCampos[i] == "X") {
				posX = i;
		}
		if(nombresDeCampos[i] == "Y") {
				posY = i;
		}
	}
}

void KNN::grabarTrainEnCorrespondiente(string pdDistrict,ptoDistrito instanciaTrain){
	string ubicacion = obtenerDireccionTrain(pdDistrict);
	fstream train(ubicacion.c_str(),ios::app|ios::binary);
	if (train.is_open()){
		train.write((char*)&instanciaTrain,sizeof(ptoDistrito));
	}
	train.close();
}

void KNN::evaluar(string testFileName) {
	//Devuelve por cada instancia probabilidades por categoria(?)(Agregar a declaracion)
	//Busca los k vecinos mas cercanos (segun distancia euclediana)
	//y de ellos obtiene probabilidades de clasificar en cada categoria
	ifstream test(testFileName.c_str(),ios::in);

	//Recorrido TestSet:
	//Obtengo una instacia del set Test
		//float xtest = xdeInstanciaTest;
		//float ytest = ydeInstanciaTest;
		//string pdDistrict = pdDistrictDeInstanciaTest;
	//Vector de K estructuras con Categoria y distancia:
		//vector<distVecino> vecinos = buscarVecinos(xtest,ytest,pdDistrict);
	//Vector con frecuencias por categoria (para obtener probabilidades solo hay que dividirlo por k)
	//Esta ordenado con el orden del setterData:
		//vector<int> frecuenciasCategoria = contarCategoria(vecinos);
	//armar Instancia de salida con numero de instancia de test y vector fecuenciasCategoria
	//almacenar resultado en la estructura/formato que corresponda
	//Buscar siguiente instancia y repetir

	test.close();
	//return estructuraResultado; (poner nombre que convenga)
}

vector<KNN::distVecino> KNN::buscarVecinos(float x, float y, string pdDistrict) {
	distVecino vecinoTrain;
	vector<distVecino> vecinos;
	string direccionTrainActual = obtenerDireccionTrain(pdDistrict);
	fstream trainActual(direccionTrainActual.c_str(),ios::in|ios::binary);
	if (trainActual.is_open()){
		while (trainActual.good()){
			ptoDistrito instanciaTrainActual;
			trainActual.read((char*)&instanciaTrainActual,sizeof(ptoDistrito));
			vecinoTrain.categoria = instanciaTrainActual.categoria;
			vecinoTrain.distancia = calcularDistancia(x,instanciaTrainActual.x,y,instanciaTrainActual.y);
			distVecino vecino;
			if ((int)vecinos.size() >= k) {
				int posMayor;
				posMayor = buscarPosicionDelMayor(vecinos);
				vecino = vecinos[posMayor];
				if (vecino.distancia > vecinoTrain.distancia){
					vecinos[posMayor] = vecinoTrain;
				}
			} else {
				vecinos.push_back(vecinoTrain);
			}
		}
	}
	trainActual.close();
	return vecinos;
}

string KNN::obtenerDireccionTrain(string pdDistrict) {
	string ubicacion = string("../DataTeam/files/");
	ubicacion += pdDistrict;
	ubicacion += string(".bin");
	return ubicacion;
 }

float KNN::calcularDistancia(float xtest, float xtrain, float ytest, float ytrain){
	return sqrt(pow(xtrain-xtest,2)+pow(ytrain-ytest,2));
}

int KNN::buscarPosicionDelMayor(vector<distVecino> vecinos) {
	int i,posMayor;
	distVecino vecino;
	float distanciaMayor = 0;
	for (i = 0; i < k; i++) {
		vecino = vecinos[i];
		if (distanciaMayor < vecino.distancia){
			posMayor = i;
			distanciaMayor = vecino.distancia;
		}
	}
	return posMayor;
}

vector<int> KNN::contarCategoria(vector<distVecino> vecinos) {
	vector<int> frecCat(39, 0);
	int i;
	distVecino vecino;
	for (i = 0 ; i < (int)vecinos.size(); i++){
		vecino = vecinos[i];
		frecCat[vecino.categoria] += 1;
	}
	return frecCat;
}
