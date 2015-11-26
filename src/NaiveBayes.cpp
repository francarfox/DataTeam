/*
 * NaiveBayes.cpp
 *
 *  Created on: Nov 26, 2015
 *      Author: fran
 */

#include "NaiveBayes.h"
#include "math.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

NaiveBayes::NaiveBayes(string trainFileName, string predictFieldName) {
	this->trainFileName = trainFileName;
	this->predictFieldName = predictFieldName;
//	this->trainFile(trainFileName.c_str(), ios::in);
	this->trainFile->open(trainFileName.c_str(), ios::in);
}

NaiveBayes::~NaiveBayes() { }

void NaiveBayes::train() {
	if (trainFile->is_open()) {
		cout << "Entrenando NaiveBayes..." << endl;
		// Obtengo nombre de los campos con la primera linea leida del trainFile
		getFieldNamesFromFirstLine();
		// Extraemos la media y la varianza de los datos
		doGaussianDistribution();

		trainFile->close();
	} else {
		cout << "Error when open file " + trainFileName << endl;
	}
}

void NaiveBayes::getFieldNamesFromFirstLine() {
	cout << "Obteniendo nombre de los campos..." << endl;

	string fieldName;
	char currentChar;

	while (trainFile->good() && currentChar != '\n') {
		currentChar = (char)trainFile->get();

		if (currentChar == ',') {
			fieldNames.push_back(fieldName);
			fieldName = "";
		} else {
			fieldName += currentChar;
		}
	}
}

//Year,Month,Day,Hour,Category,Descript,DayOfWeek,PdDistrict,Resolution,Address,X,Y
void NaiveBayes::doGaussianDistribution() {
	cout << "Haciendo la distribucion Gaussiana..." << endl;

	processCalculateMean();
	processCalculateVariance();
}

void NaiveBayes::processCalculateMean() {
	cout << "Calculando medias..." << endl;

	while (trainFile->good()) {
		vector<string> dataRecord;	// cache de los datos del registro a leer
		string currentCategoryName;	// nombre de la categoria del registro a leer
		string dataString = "";
		char currentChar;
		int currentFieldIndex = 0;

		// Proceso por cada registro
		while (currentChar != '\n') {
			currentChar = (char)trainFile->get();

			if (currentChar == ',') {
				// Obtengo el nombre de la categoria del registro
				currentCategoryName = getCategoryName(dataString, currentFieldIndex++);
				// Guardo en cache los datos del registro
				dataRecord.push_back(dataString.c_str());
				dataString = "";
			} else {
				dataString += currentChar;
			}
		}

		// sumo datos para el posterior calculo de la media sabiedo la categoria del registro
		addForCalculateMean(dataRecord, currentCategoryName);
	}

	calculateMean();
}

void NaiveBayes::processCalculateVariance() {
	cout << "Calculando varianzas..." << endl;

	while (trainFile->good()) {
		vector<string> dataRecord;	// cache de los datos del registro a leer
		string currentCategoryName;	// nombre de la categoria del registro a leer
		string dataString = "";
		char currentChar;
		int currentFieldIndex = 0;

		// Proceso por cada registro
		while (currentChar != '\n') {
			currentChar = (char)trainFile->get();

			if (currentChar == ',') {
				// Obtengo el nombre de la categoria del registro
				currentCategoryName = getCategoryName(dataString, currentFieldIndex++);
				// Guardo en cache los datos del registro
				dataRecord.push_back(dataString.c_str());
				dataString = "";
			} else {
				dataString += currentChar;
			}
		}

		// sumo datos para el posterior calculo de la varianza sabiedo la categoria del registro
		addForCalculateVariance(dataRecord, currentCategoryName);
	}

	calculateVariance();
}

string NaiveBayes::getCategoryName(string dataString, int currentFieldIndex) {

	if (currentFieldIndex == getPredictFieldIndex()) {
		return dataString;
	}

	return "";
}

int NaiveBayes::getPredictFieldIndex() {

	for(int i=0; i < fieldNames.size(); i++) {
		if (fieldNames[i] == predictFieldName) {
			return i;
		}
	}

	return -1;
}

void NaiveBayes::addForCalculateMean(vector<string> dataRecord, string categoryName) {
	int category = (int)categoryName.c_str();	// Previamente trainFile con datos numericos

	for(int i=0; i < dataRecord.size(); i++) {
		double data = atof(dataRecord[i].c_str());

		// sumo datos para el posterior calculo de la media
		vector<double> dataCategory = gaussianDistribution[category];
		int totalCategory = totalCategoryRecords[category];

		dataCategory[i*2] += data;
		totalCategory++;
	}
}

void NaiveBayes::addForCalculateVariance(vector<string> dataRecord, string categoryName) {
	int category = (int)categoryName.c_str();	// Previamente trainFile con datos numericos

	for(int i=0; i < dataRecord.size(); i++) {
		double data = atof(dataRecord[i].c_str());

		// sumo datos para el posterior calculo de la varianza
		vector<double> dataCategory = gaussianDistribution[category];
		double mean = dataCategory[i*2];
		double variance = pow(mean - data, 2);

		dataCategory[i*2+1] += variance;
	}
}

void NaiveBayes::calculateMean() {

	for(int i=0; i < gaussianDistribution.size(); i++) {
		vector<double> dataCategory = gaussianDistribution[i];
		int totalCategory = totalCategoryRecords[i];

		dataCategory[i*2] /= totalCategory;
	}
}

void NaiveBayes::calculateVariance() {

	for(int i=0; i < gaussianDistribution.size(); i++) {
		vector<double> dataCategory = gaussianDistribution[i];
		double mean = dataCategory[i*2];
		int totalCategory = totalCategoryRecords[i];

		// Aplico formula de la varianza
		dataCategory[i*2+1] /= (totalCategory - 1);
	}
}
