/*
 * KNN.cpp
 *
 *  Created on: Nov 25, 2015
 *      Author: flor
 */

#include "KNN.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "ConsoleLog.h"

#define X_INVALID -120.5
#define Y_INVALID 90.0

KNN::KNN(int valorK, int totalTrainRecords, int totalTestRecords, SetterData setterDataTrain) {
	k = valorK;
	this->totalTrainRecords = totalTrainRecords;
	this->totalTestRecords = totalTestRecords;
	setterData = setterDataTrain;
	crearArchivosParaUsoInterno();
}

KNN::~KNN() {}

void KNN::crearArchivosParaUsoInterno(){
	//Me aseguro que los archivos se crean en blanco
	fstream train("../DataTeam/files/trainNORTHERN.txt",ios::out|ios::trunc);
	train.close();
	train.open("../DataTeam/files/trainPARK.txt",ios::out|ios::trunc);
	train.close();
	train.open("../DataTeam/files/trainINGLESIDE.txt",ios::out|ios::trunc);
	train.close();
	train.open("../DataTeam/files/trainBAYVIEW.txt",ios::out|ios::trunc);
	train.close();
	train.open("../DataTeam/files/trainRICHMOND.txt",ios::out|ios::trunc);
	train.close();
	train.open("../DataTeam/files/trainCENTRAL.txt",ios::out|ios::trunc);
	train.close();
	train.open("../DataTeam/files/trainTARAVAL.txt",ios::out|ios::trunc);
	train.close();
	train.open("../DataTeam/files/trainTENDERLOIN.txt",ios::out|ios::trunc);
	train.close();
	train.open("../DataTeam/files/trainMISSION.txt",ios::out|ios::trunc);
	train.close();
	train.open("../DataTeam/files/trainSOUTHERN.txt",ios::out|ios::trunc);
	train.close();
}

void KNN::aplicarKNN(string trainFileName, string testFileName, string resultKNNFileName){
	cout << endl << "Inicio KNN" << endl;


	entrenar(trainFileName);

	cout << "Entrenamiento completo" << endl;

	return evaluar(testFileName, resultKNNFileName);
}

void KNN::entrenar(string trainFileName) {
	//Transformo Set de Entrenamiento en archivos binarios por distrito de los campos necesarios para realizar knn
	//Campos utilizados: Category, PdDistrict, x, y

	fstream train(trainFileName.c_str(),ios::in);

	if (train.is_open()) {
		//Recorre train y guarda campos en archivos binarios con registro ptoDistrito
		vector<string> nombresDeCampos = obtenerCamposDeLinea(train);
		int posCategoria = obtenerPosicionDeCampo(nombresDeCampos,"Category");
		int posPdDistrict = obtenerPosicionDeCampo(nombresDeCampos,"PdDistrict");
		int posX = obtenerPosicionDeCampo(nombresDeCampos,"X");
		int posY = obtenerPosicionDeCampo(nombresDeCampos,"Y");
		int currentProcessRecords = 0;

		while (train.good()) {
			vector<string> datos = obtenerCamposDeLinea(train);
			if (!datos.empty()) {
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
				// Muestro porcetaje de proceso
				logPercent("Entrenando KNN", ++currentProcessRecords, totalTrainRecords);
			}
		}
	}
	train.close();
}

vector<string> KNN::obtenerCamposDeLinea(fstream &file) {
	vector<string> campos;
	string linea;
	if(file.good()){
		getline(file,linea);
		if(!linea.empty()) {
			size_t posComa = 0;
			while (posComa != string::npos) {
				posComa = linea.find(",");
				campos.push_back(linea.substr(0,posComa));
				linea.erase(0,posComa+1);
			}
			campos.push_back(linea.substr(0,linea.find("\r")));
		}
	}
	return campos;
}

int KNN::obtenerPosicionDeCampo (vector<string> &nombresDeCampos, string campoABuscar) {
	int pos;
	for (int i = 0; i < (int)nombresDeCampos.size(); i++){
		if(nombresDeCampos[i] == campoABuscar) {
			pos = i;
		}
	}
	return pos;
}

void KNN::grabarTrainEnCorrespondiente(string pdDistrict,ptoDistrito instanciaTrain){
	string ubicacion = obtenerDireccionTrainPdDistrict(pdDistrict);
	fstream train(ubicacion.c_str(),ios::app|ios::out);
	if (train.is_open()){
		train << instanciaTrain.categoria << "," << instanciaTrain.x << "," << instanciaTrain.y << endl;
	}
	train.close();
}

void KNN::evaluar(string testFileName, string resultKNNFileName) {
	fstream test(testFileName.c_str(),ios::in);
	vector<string> categoriasOrdenadas = setterData.getCategoryNames();
	generarNombresDeCamposResultado(resultKNNFileName,categoriasOrdenadas);
	if (test.is_open()){
		vector<string> nombresDeCampos = obtenerCamposDeLinea(test);
		int posId = obtenerPosicionDeCampo(nombresDeCampos,"Id");
		int posPdDistrict = obtenerPosicionDeCampo(nombresDeCampos,"PdDistrict");
		int posX = obtenerPosicionDeCampo(nombresDeCampos,"X");
		int posY = obtenerPosicionDeCampo(nombresDeCampos,"Y");
		int currentProcessRecords = 0;

		while (test.good()) {
			vector<string> datos = obtenerCamposDeLinea(test);
			if (!datos.empty()) {
				string id = datos[posId];
				string pdDistrict = datos[posPdDistrict];
				float xtest = strtof(datos[posX].c_str(),NULL);
				float ytest = strtof(datos[posY].c_str(),NULL);
				//Creo vector de frecuencias en 0 para el caso de coordenadas invalidas
				vector<int> frecuenciasCategoria(categoriasOrdenadas.size(),0);
				if ((xtest != X_INVALID) && (ytest != Y_INVALID)){
					vector<distVecino> vecinos = buscarVecinos(xtest,ytest,pdDistrict);
					if(!vecinos.empty()) {
						frecuenciasCategoria = contarCategoria(vecinos, categoriasOrdenadas);
					}
				}
				grabarResultado(resultKNNFileName,id,frecuenciasCategoria);

				// Muestro porcetaje de proceso
				cout << "currentProcessRecords: " << currentProcessRecords << endl;
				logPercent("Entrenando KNN", ++currentProcessRecords, totalTestRecords);
			}
		}
	}
	test.close();

	cout << "KNN Terminado" << endl;
}

vector<KNN::distVecino> KNN::buscarVecinos(float x, float y, string pdDistrict) {
	distVecino vecinoTrain;
	vector<distVecino> vecinos;
	string direccionTrainActual = obtenerDireccionTrainPdDistrict(pdDistrict);
	fstream trainActual(direccionTrainActual.c_str(),ios::in);
	if (trainActual.is_open()){
		while (trainActual.good()){
			ptoDistrito instanciaTrainActual = obtenerInstanciaTrainActual(trainActual);
			if(instanciaTrainActual.categoria != -1) {
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
	}
	trainActual.close();
	return vecinos;
}

KNN::ptoDistrito KNN::obtenerInstanciaTrainActual(fstream &trainActual) {
	ptoDistrito instancia;
	if(trainActual.good()){
		string linea;
		getline(trainActual,linea);
		if(!linea.empty()) {
			size_t pos = 0;
			pos = linea.find(",");
			instancia.categoria = atoi(linea.substr(0,pos).c_str());
			linea.erase(0,pos+1);
			pos = linea.find(",");
			instancia.x = strtof(linea.substr(0,pos).c_str(),NULL);
			linea.erase(0,pos+1);
			pos = linea.find("\r");
			instancia.y = strtof(linea.substr(0,pos).c_str(),NULL);
		} else {
			instancia.categoria = -1;
		}
	}
	return instancia;
}

string KNN::obtenerDireccionTrainPdDistrict(string pdDistrict) {
	return string("../DataTeam/files/train")+pdDistrict+string(".txt");
 }

float KNN::calcularDistancia(float xtest, float xtrain, float ytest, float ytrain){
	return sqrt(pow(xtrain-xtest,2)+pow(ytrain-ytest,2));
}

int KNN::buscarPosicionDelMayor(vector<distVecino> &vecinos) {
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

vector<int> KNN::contarCategoria(vector<distVecino> &vecinos, vector<string> &categorias) {
	vector<int> frecCat(categorias.size(), 0);
	int i;
	distVecino vecino;
	for (i = 0 ; i < (int)vecinos.size(); i++){
		vecino = vecinos[i];
		int indice = obtenerIndiceOrdenadoCategoria(vecino.categoria,categorias);
		frecCat[indice] += 1;
	}
	return frecCat;
}

int KNN::obtenerIndiceOrdenadoCategoria(int numCategoria, vector<string> &categorias) {
	int pos = 0;
	string categoria = setterData.getCategoryName(numCategoria);
	for (int i = 0; i < (int)categorias.size(); i++) {
		if(categorias[i] == categoria){
			pos = i;
		}
	}
	return pos;
}

void KNN::generarNombresDeCamposResultado(string resultKNNFileName, vector<string> &categorias) {
	fstream resultado(resultKNNFileName.c_str(),ios::out|ios::trunc);
	string linea = "Id,";
	int tamanio = (int) categorias.size();
	for (int i = 0; i < tamanio; i++){
		linea += categorias[i];
		if (i != tamanio - 1 ) {
			linea += ",";
		}
	}
	if(resultado.good()){
		resultado << linea;
	}
	resultado.close();
}

void KNN::grabarResultado(string resultKNNFileName, string id, vector<int> &frecuenciasCategoria){
	fstream resultado(resultKNNFileName.c_str(),ios::app|ios::out);
	if (resultado.is_open()) {
		resultado << endl;
		resultado << id << ",";
		int tamanio = (int) frecuenciasCategoria.size();
		for (int i = 0; i < tamanio; i++){
			float probabilidad = (float) frecuenciasCategoria[i] / (float) k;
			if (resultado.good()) {
				resultado << probabilidad;
				if ((resultado.good()) && (i < tamanio - 1 )) resultado << ",";
			}
		}
	}
	resultado.close();
}
