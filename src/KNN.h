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

class KNN {
public:
	KNN();
	virtual ~KNN();

	void KNN::aplicarKNN(int valorK/*, setTrain, setTest*/);

private:
	int k;
	typedef struct distVecino {
		float distancia;
		string categoria;
	}dV;

	void entrenar(/*setTrain*/);
	//Ingresar Set de Entrenamiento (Agregar a declaracion)
	//Solo se necesita Categoria, Distrito PD, x, y
	//Sino resuelto, elemina datos erroneos
	//Separa por PdDistric

	void evaluar(/*setTest */);
	//Ingresar Set de Testeo (Agregar a declaracion) y valor de k a usar.
	//Devuelve por cada instancia prbabilidades por categoria(?)(Agregar a declaracion)
	//Busca los k vecinos mas cercanos (distancia euclediana)
	//y de ellos obtiene probabilidades de clasificar en cada categoria

	vector<distVecino> buscarVecinos(/*instanciaTest*/);

	//formatoTrain obtenerTrainNecesario(string pdDistrict);

	float calcularDistancia(float xtest, float xtrain, float ytest, float ytrain);

	int buscarMayor(vector<distVecino> vecinos);

	vector<int> contarCategoria(vector<distVecino> vecinos);

	int obtenerNumCategoria(string categoria);

};

#endif /* KNN_H_ */
