/*
 * KNN.h
 *
 *  Created on: Nov 25, 2015
 *      Author: flor
 */

#ifndef KNN_H_
#define KNN_H_

using namespace std;
#include <math.h>
#include <string>
#include <vector>
#include <fstream>
#include "SetterData.h"



class KNN {

int k;
SetterData setterData;
typedef struct ptoDistrito {
	int categoria;
	float x;
	float y;
}pD;
typedef struct distVecino {
	float distancia;
	int categoria;
}dV;

int totalTrainRecords;
int totalTestRecords;

public:
	KNN(int valorK, int totalTrainRecords, int totalTestRecords, SetterData setterData);
	virtual ~KNN();

	void aplicarKNN(string trainFileName, string testFileName, string resultFileName);

private:

	void crearArchivosParaUsoInterno();

	void entrenar(string trainFileName);
	vector<string> obtenerCamposDeLineaTrain(fstream &file);
	int obtenerPosicionDeCampo (vector<string> &nombresDeCampos, string campoABuscar);
	void grabarTrainEnCorrespondiente(string pdDistrict,ptoDistrito instanciaTrain);

	void evaluar(string testFileName, string resultFileName);
	vector<string> obtenerCamposDeLineaTest(fstream &file);
	vector<distVecino> buscarVecinos(float x, float y, string pdDistrict);
	ptoDistrito obtenerInstanciaTrainActual(fstream &train);
	string obtenerDireccionTrainPdDistrict(string pdDistrict);
	float calcularDistancia(float xtest, float xtrain, float ytest, float ytrain);
	int buscarPosicionDelMayor(vector<distVecino> vecinos);
	vector<int> contarCategoria(vector<distVecino> vecinos, vector<string> &categorias);
	int obtenerIndiceOrdenadoCategoria(int numCategoria, vector<string> &categorias);

	void generarNombresDeCamposResultado(string resultKNNFileName, vector<string> &categorias);
	void grabarResultado(string resultFileName, string id, vector<int> &frecuenciasCategoria);

};

#endif /* KNN_H_ */
