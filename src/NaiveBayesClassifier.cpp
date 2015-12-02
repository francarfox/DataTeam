/*
 * NaiveBayesClassifier.cpp
 *
 *  Created on: Nov 30, 2015
 *      Author: fran
 */

#include "NaiveBayesClassifier.h"
#include "math.h"
#include "ConsoleLog.h"

NaiveBayesClassifier::NaiveBayesClassifier(NaiveBayes *bayes) {
	this->bayes = bayes;
	this->setterData = bayes->setterData;

	this->irrelevantFieldNames.push_back("Year");
	this->irrelevantFieldNames.push_back("Month");
	this->irrelevantFieldNames.push_back("Day");
	this->irrelevantFieldNames.push_back("Descript");
	this->irrelevantFieldNames.push_back("Resolution");
	this->irrelevantFieldNames.push_back("Address");
}

NaiveBayesClassifier::~NaiveBayesClassifier() {
	this->bayes = NULL;
}

void NaiveBayesClassifier::test(int totalRecords, string testFileName, string submissionFileName) {
	this->totalTestRecords = totalRecords;
	ifstream testFile(testFileName.c_str(), ios::in);
	ofstream submissionFile(submissionFileName.c_str(), ios::out);

	if (testFile.is_open() && submissionFile.is_open()) {
		cout << "\nPrediciendo con clasificador NaiveBayes..." << endl;

		// Calculos de clasificacion
		doClassification(testFile, submissionFile);

		testFile.close();
		submissionFile.close();
	} else {
		cout << "Error when open testFile or submissionFile!" << endl;
	}
}

void NaiveBayesClassifier::doClassification(ifstream &testFile, ofstream &submissionFile) {
	// Obtengo nombre de los campos con la primera linea leida del trainFile
	getFieldNamesFromFirstLine(testFile, submissionFile);

	// Realizo clasificatcion
	processClassification(testFile, submissionFile);
}

void NaiveBayesClassifier::getFieldNamesFromFirstLine(ifstream &testFile, ofstream &submissionFile) {
	cout << "Obteniendo nombre de los campos..." << endl;

	string fieldName;
	char currentChar;

	while (testFile.good() && currentChar != '\n') {
		currentChar = (char)testFile.get();

		// Contemplo el ultimo campo que no termina en ,
		if (currentChar == '\r') {
			testFieldNames.push_back(fieldName);
		}

		if (currentChar == ',') {
			testFieldNames.push_back(fieldName);
			fieldName = "";
		} else {
			fieldName += currentChar;
		}
	}

	generateFieldNamesWithoutIrrelevants();
	writeFieldNames(submissionFile);
}

void NaiveBayesClassifier::processClassification(ifstream &testFile, ofstream &submissionFile) {
	cout << "Realizando clasificacion de los datos..." << endl;

	int currentProcessRecords = 0;

	while (testFile.good()) {
		vector<double> dataRecord;	// cache de los datos del registro a escribir
		string dataString = "";
		char currentChar = '0';
		int currentFieldIndex = 0;
		bool readingPhrase = false;

		// Proceso por cada registro
		while (testFile.good() && currentChar != '\n') {
			currentChar = (char)testFile.get();

			// Tengo en cuenta frases en Description y Resolution que contienen ,
			if (currentChar == '"') {
				readingPhrase = !readingPhrase;
			}

			// Contemplo el ultimo campo que no termina en ,
			if (currentChar == '\r') {
				addDataNumeric(dataRecord, dataString, currentFieldIndex++);
			}

			if (currentChar == ',' && readingPhrase == false) {
				// Agrego dato numerico
				addDataNumeric(dataRecord, dataString, currentFieldIndex++);
				dataString = "";
			} else {
				// Fin de archivo
				if (currentChar == '\377') {
					addDataNumeric(dataRecord, dataString, currentFieldIndex++);
				} else {
					dataString += currentChar;
				}
			}
		}

		writeSubmissionFile(dataRecord, submissionFile);
		logPercent("Calculando probabilidades", ++currentProcessRecords, totalTestRecords);
	}
}

void NaiveBayesClassifier::addDataNumeric(vector<double> &dataRecord, string dataString, int currentFieldIndex) {
	string currentFieldName = testFieldNames[currentFieldIndex];

	// Me fijo si es un dato perteneciente a un campo irrelevante
	for(size_t i=0; i < irrelevantFieldNames.size(); i++) {
		string irrelevantField = irrelevantFieldNames[i];

		// Lo paso por alto
		if (currentFieldName == irrelevantField) {
			return;
		}
	}

	// Agrego dato numerico
	double dataNumeric = -1;

	if (currentFieldName == "DayOfWeek") {
		dataNumeric = setterData.getDayOfWeek(dataString);
		dataRecord.push_back(dataNumeric);
	} else
	if (currentFieldName == "PdDistrict") {
		dataNumeric = setterData.getPdDistrict(dataString);
		dataRecord.push_back(dataNumeric);
	}
}

void NaiveBayesClassifier::writeFieldNames(ofstream &submissionFile) {
	vector<string> submissionFieldNames = setterData.getCategoryNames();
	submissionFieldNames.insert(submissionFieldNames.begin(), "Id");

	writeDataRecord(submissionFieldNames, submissionFile);
}

void NaiveBayesClassifier::writeSubmissionFile(vector<double> &dataRecord, ofstream &submissionFile) {
	// Antes de escribir el registro en el archivo salida, calculo las probabilidades
	vector<double> numericDataRecord = calculateProbability(dataRecord);

	// Convert to vector<string>
	vector<string> stringDataRecord;

	for(size_t i=0; i < numericDataRecord.size(); i++) {
		double dataNumeric = numericDataRecord[i];
		string dataString = setterData.getString(dataNumeric);
		stringDataRecord.push_back(dataString);
	}

	writeDataRecord(stringDataRecord, submissionFile);
}

void NaiveBayesClassifier::writeDataRecord(vector<string> dataRecord, ofstream &submissionFile) {
	string line;

	for(size_t i=0; i < dataRecord.size(); i++) {
		line += dataRecord[i];

		if (i < dataRecord.size()-1) {
			 line += ",";
		}
	}

	line += "\n";
	cout << line << endl;

	submissionFile << line;
}

void NaiveBayesClassifier::generateFieldNamesWithoutIrrelevants() {
	// Genero una nueva lista sin campo irrelevantes para luego usarlo en los calculos

	for(size_t i=0; i < testFieldNames.size(); i++) {
		string currentFieldName = testFieldNames[i];
		bool isIrrelevant = false;

		// Me fijo si es un dato perteneciente a un campo irrelevante
		for(size_t j=0; j < irrelevantFieldNames.size(); j++) {
			string irrelevantField = irrelevantFieldNames[j];

			// Lo paso por alto
			if (currentFieldName == irrelevantField) {
				isIrrelevant = true;
			}
		}

		if (isIrrelevant == false) {
			fieldNamesWithoutIrrelevant.push_back(currentFieldName);
		}
	}
}


// Calculos

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
