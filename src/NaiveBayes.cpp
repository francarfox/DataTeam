/*
 * NaiveBayes.cpp
 *
 *  Created on: Nov 26, 2015
 *      Author: fran
 */

#include "NaiveBayes.h"
#include "NaiveBayesClassifier.h"
#include "math.h"
#include "ConsoleLog.h"

using namespace std;

NaiveBayes::NaiveBayes(string predictFieldName, SetterData setterData) {
	this->predictFieldName = predictFieldName;

	this->setterData = setterData;
}

NaiveBayes::~NaiveBayes() { }

//Year,Month,Day,Hour,Category,Descript,DayOfWeek,PdDistrict,Resolution,Address,X,Y
void NaiveBayes::train(int totalRecords, string fileName) {
	this->totalTrainRecords = totalRecords;
	ifstream trainFile(fileName.c_str(), ios::in);

	if (trainFile.is_open()) {
		cout << "Entrenando NaiveBayes..." << endl;

		// Extraemos la media y la varianza de los datos
		doGaussianDistribution(trainFile);

		trainFile.close();
	} else {
		cout << "Error when open trainFile!" << endl;
	}
}

void NaiveBayes::debug() {
	cout << "\nmeanDistribution -> Hour DayOfWeek PdDistrict X Y" << endl;
	for(size_t i=0; i < meanDistribution.size(); i++) {
		string category = setterData.getCategoryName(i);
		cout << "i=" << i << " " << category << " ";
		for(size_t j=0; j < meanDistribution[i].size(); j++) {
			cout << meanDistribution[i][j] << " ";
		}

		cout << endl;
	}

	cout << "\nvarianceDistribution -> Hour DayOfWeek PdDistrict X Y" << endl;
	for(size_t i=0; i < varianceDistribution.size(); i++) {
		string category = setterData.getCategoryName(i);
		cout << "i=" << i << " " << category << " ";
		for(size_t j=0; j < varianceDistribution[i].size(); j++) {
			cout << varianceDistribution[i][j] << " ";
		}

		cout << endl;
	}
}

void NaiveBayes::test(int totalRecords, string testFileName, string submissionFileName) {
	NaiveBayesClassifier classifier = NaiveBayesClassifier(this);
	classifier.test(totalRecords, testFileName, submissionFileName);
}

void NaiveBayes::doGaussianDistribution(ifstream &trainFile) {
	cout << "Haciendo la distribucion Gaussiana..." << endl;

	// Obtengo nombre de los campos con la primera linea leida del trainFile
	getFieldNamesFromFirstLine(trainFile);
	processCalculateMean(trainFile);

	// Vuelvo al inicio del archivo
	trainFile.clear();
	trainFile.seekg(0, ios::beg);

	// Ignoro la primera linea de nombre de campos
	ignoreFieldNamesFromFirstLine(trainFile);
	processCalculateVariance(trainFile);

	// Debug
	debug();

	// Elimino el category de los nombres de los campos para que concuerde con las matrices
	fieldNames.erase(fieldNames.begin() + getPredictFieldIndex());
}

void NaiveBayes::getFieldNamesFromFirstLine(ifstream &trainFile) {
	cout << "Obteniendo nombre de los campos..." << endl;

	string fieldName;
	char currentChar;

	while (trainFile.good() && currentChar != '\n') {
		currentChar = (char)trainFile.get();

		if (currentChar == ',') {
			fieldNames.push_back(fieldName);
			fieldName = "";
		} else {
			fieldName += currentChar;
		}
	}
}

void NaiveBayes::ignoreFieldNamesFromFirstLine(ifstream &trainFile) {
	char currentChar;

	while (trainFile.good() && currentChar != '\n') {
		currentChar = (char)trainFile.get();
	}
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

			// Contemplo el ultimo campo que no termina en ,
			if (currentChar == '\n') {
				processDataString(dataRecord, dataString, currentFieldIndex++, currentCategoryName);
			}

			if (currentChar == ',') {
				// Proceso los datos actuales
				processDataString(dataRecord, dataString, currentFieldIndex++, currentCategoryName);
				dataString = "";
			} else {
				dataString += currentChar;
			}
		}

		// sumo datos para el posterior calculo de la media sabiedo la categoria del registro
		addForCalculateMean(dataRecord, currentCategoryName);
		logPercent("Calculando medias", ++currentProcessRecords, totalTrainRecords);
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

			// Contemplo el ultimo campo que no termina en ,
			if (currentChar == '\n') {
				processDataString(dataRecord, dataString, currentFieldIndex++, currentCategoryName);
			}

			if (currentChar == ',') {
				// Proceso los datos actuales
				processDataString(dataRecord, dataString, currentFieldIndex++, currentCategoryName);
				dataString = "";
			} else {
				dataString += currentChar;
			}
		}

		// sumo datos para el posterior calculo de la varianza sabiedo la categoria del registro
		addForCalculateVariance(dataRecord, currentCategoryName);
		logPercent("Calculando varianzas", ++currentProcessRecords, totalTrainRecords);
	}

	calculateVariance();
}

void NaiveBayes::processDataString(vector<string> &dataRecord, string dataString, int currentFieldIndex, string &currentCategoryName) {

	if (currentFieldIndex == getPredictFieldIndex()) {
		currentCategoryName = dataString;
	} else {
		dataRecord.push_back(dataString.c_str());
	}
}

int NaiveBayes::getPredictFieldIndex() {

	for(size_t i=0; i < fieldNames.size(); i++) {
		if (fieldNames[i] == predictFieldName) {
			return i;
		}
	}

	return -1;
}

void NaiveBayes::addForCalculateMean(vector<string> &dataRecord, string categoryName) {
	int category = setterData.getInt(categoryName);	// Previamente trainFile con datos numericos

	for(size_t i=0; i < dataRecord.size(); i++) {
		double data = setterData.getDouble(dataRecord[i]);

		// sumo datos para el posterior calculo de la media
		verifyDataCategory(category);
		meanDistribution[category][i] += data;
	}

	totalCategoryRecords[category]++;
}

void NaiveBayes::addForCalculateVariance(vector<string> &dataRecord, string categoryName) {
	int category = setterData.getInt(categoryName);	// Previamente trainFile con datos numericos

	for(size_t i=0; i < dataRecord.size(); i++) {
		double data = setterData.getDouble(dataRecord[i]);

		// sumo datos para el posterior calculo de la varianza
		verifyDataCategory(category);
		double mean = meanDistribution[category][i];
		double variance = pow(mean - data, 2);

		varianceDistribution[category][i] += variance;
	}
}

void NaiveBayes::calculateMean() {

	for(size_t i=0; i < meanDistribution.size(); i++) {
		int totalCategory = totalCategoryRecords[i];

		for(size_t j=0; j < meanDistribution[i].size(); j++) {
			meanDistribution[i][j] /= totalCategory;
		}
	}
}

void NaiveBayes::calculateVariance() {

	for(size_t i=0; i < varianceDistribution.size(); i++) {
		int totalCategory = totalCategoryRecords[i];

		for(size_t j=0; j < varianceDistribution[i].size(); j++) {
	//		double mean = (double)dataCategory[i*2];

			// Aplico formula de la varianza
			varianceDistribution[i][j] /= (totalCategory - 1);
		}
	}
}

void NaiveBayes::verifyDataCategory(int category) {

	if (category > (int)meanDistribution.size()-1) {
		// Agregar las categorias necesarias para que sea valido
		for (int i = meanDistribution.size(); i <= category; i++) {
			meanDistribution.push_back(getNewVector());
			varianceDistribution.push_back(getNewVector());
			totalCategoryRecords.push_back(0);
		}
	}
}

vector<double> NaiveBayes::getNewVector() {
	vector<double> vector;

	for (size_t i=0; i < fieldNames.size(); i++) {
		vector.push_back(0);
	}

	return vector;
}
