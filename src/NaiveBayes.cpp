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
#include "ConsoleLog.h"

using namespace std;

NaiveBayes::NaiveBayes(string predictFieldName) {
	this->predictFieldName = predictFieldName;
}

NaiveBayes::~NaiveBayes() { }

void NaiveBayes::train(int totalRecords, string fileName) {
	this->totalTrainRecords = totalRecords;
	ifstream trainFile(fileName.c_str(), ios::in);

	if (trainFile.is_open()) {
		cout << "Entrenando NaiveBayes..." << endl;

		// Obtengo nombre de los campos con la primera linea leida del trainFile
		getFieldNamesFromFirstLine(trainFile);
		// Extraemos la media y la varianza de los datos
		doGaussianDistribution(trainFile);

		trainFile.close();
	} else {
		cout << "Error when open trainFile!" << endl;
	}
}

void NaiveBayes::test(int totalRecords, string fileName) {
	this->totalTestRecords = totalRecords;
	ifstream testFile(fileName.c_str(), ios::in);

	if (testFile.is_open()) {
		cout << "Prediciendo con clasificador NaiveBayes..." << endl;

		// Calculos de clasificacion

		testFile.close();
	} else {
		cout << "Error when open testFile!" << endl;
	}
}

void NaiveBayes::getFieldNamesFromFirstLine(ifstream &trainFile) {
	cout << "Obteniendo nombre de los campos..." << endl;

	string fieldName;
	char currentChar;

	while (trainFile.good() && currentChar != '\n') {
		currentChar = (char)trainFile.get();

		if (currentChar == ',') {
			cout << fieldName.c_str() << " ";
			fieldNames.push_back(fieldName);
			fieldName = "";
		} else {
			fieldName += currentChar;
		}
	}

	cout << endl;
}

//Year,Month,Day,Hour,Category,Descript,DayOfWeek,PdDistrict,Resolution,Address,X,Y
void NaiveBayes::doGaussianDistribution(ifstream &trainFile) {
	cout << "Haciendo la distribucion Gaussiana..." << endl;

	processCalculateMean(trainFile);
	processCalculateVariance(trainFile);
}

void NaiveBayes::processCalculateMean(ifstream &trainFile) {
	cout << "Calculando medias..." << endl;

	int currentProcessRecords = 0;

	while (trainFile.good()) {
		vector<string> dataRecord;	// cache de los datos del registro a leer
		string currentCategoryName;	// nombre de la categoria del registro a leer
		string dataString = "";
		char currentChar = '0';
		int currentFieldIndex = 0;

		// Proceso por cada registro
		while (trainFile.good() && currentChar != '\n') {
			currentChar = (char)trainFile.get();

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
		logPercent(++currentProcessRecords, totalTrainRecords);
	}

	calculateMean();
}

void NaiveBayes::processCalculateVariance(ifstream &trainFile) {
	cout << "Calculando varianzas..." << endl;

	int currentProcessRecords = 0;

	while (trainFile.good()) {
		vector<string> dataRecord;	// cache de los datos del registro a leer
		string currentCategoryName;	// nombre de la categoria del registro a leer
		string dataString = "";
		char currentChar = '0';
		int currentFieldIndex = 0;

		// Proceso por cada registro
		while (trainFile.good() && currentChar != '\n') {
			currentChar = (char)trainFile.get();

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
		logPercent(++currentProcessRecords, totalTrainRecords);
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

	for(size_t i=0; i < fieldNames.size(); i++) {
		if (fieldNames[i] == predictFieldName) {
			return i;
		}
	}

	return -1;
}

void NaiveBayes::addForCalculateMean(vector<string> dataRecord, string categoryName) {
	int category = (int)categoryName.c_str();	// Previamente trainFile con datos numericos

	for(size_t i=0; i < dataRecord.size(); i++) {
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

	for(size_t i=0; i < dataRecord.size(); i++) {
		double data = atof(dataRecord[i].c_str());

		// sumo datos para el posterior calculo de la varianza
		vector<double> dataCategory = gaussianDistribution[category];
		double mean = dataCategory[i*2];
		double variance = pow(mean - data, 2);

		dataCategory[i*2+1] += variance;
	}
}

void NaiveBayes::calculateMean() {

	for(size_t i=0; i < gaussianDistribution.size(); i++) {
		vector<double> dataCategory = gaussianDistribution[i];
		int totalCategory = totalCategoryRecords[i];

		dataCategory[i*2] /= totalCategory;
	}
}

void NaiveBayes::calculateVariance() {

	for(size_t i=0; i < gaussianDistribution.size(); i++) {
		vector<double> dataCategory = gaussianDistribution[i];
		double mean = (double)dataCategory[i*2];
		int totalCategory = totalCategoryRecords[i];

		// Aplico formula de la varianza
		dataCategory[i*2+1] /= (totalCategory - 1);
	}
}
