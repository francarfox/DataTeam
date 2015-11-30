/*
 * NaiveBayesClassifier.cpp
 *
 *  Created on: Nov 30, 2015
 *      Author: fran
 */

#include "NaiveBayesClassifier.h"
#include "math.h"

NaiveBayesClassifier::NaiveBayesClassifier(NaiveBayes *bayes) {
	this->bayes = bayes;
}

NaiveBayesClassifier::~NaiveBayesClassifier() {
	this->bayes = NULL;
}

void NaiveBayesClassifier::test(int totalRecords, string fileName) {
	this->totalTestRecords = totalRecords;
	ifstream testFile(fileName.c_str(), ios::in);

	if (testFile.is_open()) {
		cout << "Prediciendo con clasificador NaiveBayes..." << endl;

		// Calculos de clasificacion
		doClassification(testFile);

		testFile.close();
	} else {
		cout << "Error when open testFile!" << endl;
	}
}

void NaiveBayesClassifier::doClassification(ifstream &testFile) {
	// Obtengo nombre de los campos con la primera linea leida del trainFile
	bayes->getFieldNamesFromFirstLine(testFile, testFieldNames);

	// Realizo clasificatcion
	processClassification(testFile);
}

void NaiveBayesClassifier::processClassification(ifstream &testFile) {
	cout << "Realizando clasificacion de los datos..." << endl;

//	int currentProcessRecords = 0;
//
//	while (testFile.good()) {
//		vector<string> dataRecord;	// cache de los datos del registro a escribir
//		string dataString = "";
//		char currentChar = '0';
//		int currentFieldIndex = 0;
//		bool readingPhrase = false;
//
//		// Proceso por cada registro
//		while (testFile.good() && currentChar != '\n') {
//			currentChar = (char)testFile.get();
//
//			// Tengo en cuenta frases en Description y Resolution que contienen ,
//			if (currentChar == '"') {
//				readingPhrase = !readingPhrase;
//			}
//
//			// Contemplo el ultimo campo que no termina en ,
//			if (currentChar == '\r') {
//				addDataNumeric(dataRecord, dataString, currentFieldIndex++);
//			}
//
//			if (currentChar == ',' && readingPhrase == false) {
//				// Agrego dato numerico
//				addDataNumeric(dataRecord, dataString, currentFieldIndex++);
//				dataString = "";
//			} else {
//				// Fin de archivo
//				if (currentChar == '\377') {
//					addDataNumeric(dataRecord, dataString, currentFieldIndex++);
//				} else {
//					dataString += currentChar;
//				}
//			}
//		}
//
//		writeDataRecord(dataRecord, outFile);
//		logPercent("Calculando probabilidades", ++currentProcessRecords, totalTestRecords);
//	}
}

vector<double> NaiveBayesClassifier::calculateProbability(vector<double> dataRecord) {
	/*
	 * 						P(WARRANTS).p(Hour|WARRANTS).p(DayOfWeek|WARRANTS)...
	 * class(WARRANTS) = __________________________________________________________
	 *											evidence
	 *
	 */

	vector<double> probCategoryVector;	// Guardo numeradores para luego dividir por evidence
	double evidence = 0;	// Inicia con el neutro de la multiplicacion

	// Calculo probabilidades por cada categoria
	for(size_t i=0; i < bayes->meanDistribution.size(); i++) {
		double probCategory = calculateProbabilityCategory(i);

		// Por cada campo del registro de datos
		for(size_t j=0; j < dataRecord.size(); j++) {
			probCategory *= calculateProbabilityRespectTo(i, j, dataRecord[j]);
		}

		probCategoryVector.push_back(probCategory);
		evidence += probCategory;
	}

	for(size_t i=0; i < probCategoryVector.size(); i++) {
		probCategoryVector[i] /= evidence;
	}

	return probCategoryVector;
}

double NaiveBayesClassifier::calculateProbabilityCategory(int category) {
	// P(WARRANTS)
	int currentCategoryRecord = bayes->totalCategoryRecords[category];
	return currentCategoryRecord / bayes->totalTrainRecords;
}

double NaiveBayesClassifier::calculateProbabilityRespectTo(int category, int field, double data) {
/*
 * 										  -(data - variance)2
 * p(Hour|WARRANTS) = _______1_______ exp( __________________ )
 *					  sqrt(2PI.mean2)	 		2.mean2
 *
 */
	double mean = bayes->meanDistribution[category][field];
	double variance = bayes->varianceDistribution[category][field];

	double base = (1 / sqrt(2 * M_PI * pow(mean, 2)));
	double exponent = -pow(data - variance, 2) / (2 * pow(mean, 2));
	return pow(base, exponent);
}
