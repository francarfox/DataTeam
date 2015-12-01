/*
 * KNN.h
 *
 *  Created on: Nov 25, 2015
 *      Author: flor
 */

#ifndef KNN_H_
#define KNN_H_

using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <vector>
#include "SetterData.h"
#include <iostream>
#include <fstream>

#define X_INVALID -120.5
#define Y_INVALID 90.0

class KNN {

int k;
SetterData setterData;
string trainNorthernFN,trainParkFN,trainInglesideFN,trainBayviewFN,trainRichmondFN,trainCentralFN,trainTaravalFN,trainTenderloinFN,trainMissionFN,trainSouthernFN;
typedef struct ptoDistrito {
	int categoria;
	float x;
	float y;
}pD;
typedef struct distVecino {
	float distancia;
	int categoria;
}dV;

public:
	KNN(int valorK, SetterData setterData);
	virtual ~KNN();

	void aplicarKNN(string trainFileName, string testFileName);

private:

	void entrenar(string trainFileName);
	//Ingresar Set de Entrenamiento
	//Solo se necesita Categoria, Distrito PD, x, y
	//Sino resuelto, elemina datos erroneos
	//Separa por PdDistric

	vector<string> obtenerNombresDeCamposDePrimerLinea(fstream &train);

	vector<string> obtenerDatosDeLinea(fstream &train);

	void obtenerPosicionesDeCamposNecesarios
		(vector<string> &nombresDeCampos, int &posCategoria, int &posDistrito, int &posX, int &posY);

	void grabarTrainEnCorrespondiente(string pdDistrict,ptoDistrito instanciaTrain);

	void evaluar(string testFileName);
	//Ingresar Set de Testeo.
	//Devuelve por cada instancia probabilidades por categoria(?)(Agregar a declaracion)
	//Busca los k vecinos mas cercanos (distancia euclediana)
	//y de ellos obtiene probabilidades de clasificar en cada categoria

	vector<distVecino> buscarVecinos(float x, float y, string pdDistrict);

	string obtenerDireccionTrain(string pdDistrict);

	float calcularDistancia(float xtest, float xtrain, float ytest, float ytrain);

	int buscarPosicionDelMayor(vector<distVecino> vecinos);

	vector<int> contarCategoria(vector<distVecino> vecinos);

};

#endif /* KNN_H_ */
