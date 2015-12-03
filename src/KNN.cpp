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

KNN::KNN(int valorK, SetterData setterDataTrain) {
	k = valorK;
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
		while (train.good()) {
			vector<string> datos = obtenerCamposDeLinea(train);
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
			train.peek();//Archivo train tiene una ultima linea en blanco
		}
	}
	train.close();
}

vector<string> KNN::obtenerCamposDeLinea(fstream &file) {
	vector<string> campos;
	string primeraLinea;
	if(file.good()) getline(file,primeraLinea);
	size_t posComa = 0;
	while (posComa != string::npos) {
		posComa = primeraLinea.find(",");
		string campo = primeraLinea.substr(0,posComa);
		campos.push_back(campo);
		primeraLinea.erase(0,posComa+1);
	}
	campos.push_back(primeraLinea.substr(0));
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
		train << instanciaTrain.categoria << instanciaTrain.x << instanciaTrain.y;
	}
	train.close();
}

void KNN::evaluar(string testFileName, string resultKNNFileName) {
	fstream test(testFileName.c_str(),ios::in);
	generarNombresDeCamposResultado(resultKNNFileName);
	if (test.is_open()){
		vector<string> nombresDeCampos = obtenerCamposDeLinea(test);
		int posId = obtenerPosicionDeCampo(nombresDeCampos,"Id");
		int posPdDistrict = obtenerPosicionDeCampo(nombresDeCampos,"PdDistrict");
		int posX = obtenerPosicionDeCampo(nombresDeCampos,"X");
		int posY = obtenerPosicionDeCampo(nombresDeCampos,"Y");
		while (test.good()) {
			vector<string> datos = obtenerCamposDeLinea(test);
			string id = datos[posId];
			string pdDistrict = datos[posPdDistrict];
			float xtest = strtof(datos[posX].c_str(),NULL);
			float ytest = strtof(datos[posY].c_str(),NULL);
			//Creo vector de frecuencias en 0 para el caso de coordenadas invalidas
			vector<int> frecuenciasCategoria(39,0);
			if ((xtest != X_INVALID) && (ytest != Y_INVALID)){
				vector<distVecino> vecinos = buscarVecinos(xtest,ytest,pdDistrict);
				frecuenciasCategoria = contarCategoria(vecinos);
			}
			grabarResultado(resultKNNFileName,id,frecuenciasCategoria);
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
			ptoDistrito instanciaTrainActual;
			trainActual >> instanciaTrainActual.categoria >> instanciaTrainActual.x >> instanciaTrainActual.y;
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

string KNN::obtenerDireccionTrainPdDistrict(string pdDistrict) {
	return "../DataTeam/files/train"+pdDistrict+".txt";
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
		frecCat[obtenerIndiceOrdenadoCategoria(vecino.categoria)] += 1;
	}
	return frecCat;
}

int KNN::obtenerIndiceOrdenadoCategoria(int numCategoria) {
	int pos = 0;
	vector<string> categorias = setterData.getCategoryNames();
	string categoria = setterData.getCategoryName(numCategoria);
	for (int i = 0; i < (int)categorias.size(); i++) {
		if(categorias[i] == categoria){
			pos = i;
		}
	}
	return pos;
}

void KNN::generarNombresDeCamposResultado(string resultKNNFileName) {
	fstream resultado(resultKNNFileName.c_str(),ios::out|ios::trunc);
	string linea = "Id,";
	vector<string> categorias = setterData.getCategoryNames();
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
		int tamanio = (int) frecuenciasCategoria.size();
		for (int i = 0; i < tamanio; i++){
			float probabilidad = (float) frecuenciasCategoria[i] / (float) k;
			if (resultado.good()) {
				resultado << probabilidad;
				if ((resultado.good()) && (i != tamanio - 1 )) resultado << ",";
			}
		}
	}
	resultado.close();
}
