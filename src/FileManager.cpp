/*
 * FileManager.cpp
 *
 *  Created on: Nov 15, 2015
 *      Author: fran
 */

#include "FileManager.h"
#include <fstream>
#include "ConsoleLog.h"

FileManager::FileManager(int totalRecords, string inFileName, string outFileName) {
	this->totalRecords = totalRecords;
	this->inFileName = inFileName;
	this->outFileName = outFileName;

	this->irrelevantFieldNames.push_back("Year");
	this->irrelevantFieldNames.push_back("Month");
	this->irrelevantFieldNames.push_back("Day");
	this->irrelevantFieldNames.push_back("Descript");
	this->irrelevantFieldNames.push_back("Resolution");
	this->irrelevantFieldNames.push_back("Address");

	this->numericFieldNames.push_back("Hour");
	this->numericFieldNames.push_back("X");
	this->numericFieldNames.push_back("Y");

	this->setterData = SetterData();
}

FileManager::~FileManager() { }

void FileManager::process() {
	ifstream inFile(inFileName.c_str(), ios::in);
	ofstream outFile(outFileName.c_str(), ios::out);

	if (inFile.is_open() && outFile.is_open()) {
		cout << "Procesando archivo de entrenamiento para limpieza..." << endl;

		// Obtengo nombre de los campos con la primera linea leida de inFile
		getFieldNamesFromFirstLine(inFile, outFile);
		// Generamos el nuevo archivo con los campos numericos
		generateOutFile(inFile, outFile);

		inFile.close();
		outFile.close();
	} else {
		cout << "Error when open files " << endl;
	}
}

void FileManager::getFieldNamesFromFirstLine(ifstream &inFile, ofstream &outFile) {
	cout << "Obteniendo nombre de los campos..." << endl;

	string fieldName;
	char currentChar;

	while (inFile.good() && currentChar != '\n') {
		currentChar = (char)inFile.get();

		// Contemplo el ultimo campo que no termina en ,
		if (currentChar == '\r') {
			cout << fieldName.c_str() << " ";
			fieldNames.push_back(fieldName);
		}

		if (currentChar == ',') {
			cout << fieldName.c_str() << " ";
			fieldNames.push_back(fieldName);
			fieldName = "";
		} else {
			fieldName += currentChar;
		}
	}

	cout << endl;
	writeFieldNames(outFile);
}

//	Year,Month,Day,Hour,Category,Descript,DayOfWeek,PdDistrict,Resolution,Address,X,Y
void FileManager::generateOutFile(ifstream &inFile, ofstream &outFile) {
	cout << "Generando nuevo archivo de entrenamiento..." << endl;

	int currentWriteRecords = 0;

	while (inFile.good()) {
		vector<string> dataRecord;	// cache de los datos del registro a escribir
		string dataString = "";
		char currentChar = '0';
		int currentFieldIndex = 0;
		bool readingPhrase = false;

		// Proceso por cada registro
		while (inFile.good() && currentChar != '\n') {
			currentChar = (char)inFile.get();

			// Tengo en cuenta frases en Description y Resolution que contienen ,
			if (currentChar == '"') {
				readingPhrase = !readingPhrase;
			}

			// Contemplo el ultimo campo que no termina en ,
			if (currentChar == '\r') {
				toDataNumeric(dataRecord, dataString, currentFieldIndex++);
			}

			if (currentChar == ',' && readingPhrase == false) {
				// Paso a dato numerico
				toDataNumeric(dataRecord, dataString, currentFieldIndex++);
				dataString = "";
			} else {
				// Fin de archivo
				if (currentChar == '\377') {
					toDataNumeric(dataRecord, dataString, currentFieldIndex++);
				} else {
					dataString += currentChar;
				}
			}
		}

		writeDataRecord(dataRecord, outFile);
		logPercent(++currentWriteRecords, totalRecords);
	}
}

void FileManager::toDataNumeric(vector<string> &dataRecord, string dataString, int currentFieldIndex) {
	string currentField = fieldNames[currentFieldIndex];

	for(size_t i=0; i < irrelevantFieldNames.size(); i++) {
		string irrelevantField = irrelevantFieldNames[i];

		if (currentField == irrelevantField) {
			return;
		}
	}

	for(size_t i=0; i < numericFieldNames.size(); i++) {
		string numericField = numericFieldNames[i];

		if (currentField == numericField) {
			// Ya es un dato numerico
			dataRecord.push_back(dataString.c_str());
			return;
		}
	}

	// Cambio a numerico
	string dataNumeric = setterData.getDataNumeric(dataString, currentFieldIndex);
	dataRecord.push_back(dataNumeric.c_str());
}

void FileManager::writeFieldNames(ofstream &outFile) {
	vector<string> outFieldNames;

	for(size_t j=0; j < fieldNames.size(); j++) {
		bool isIrrelevant = false;

		for(size_t i=0; i < irrelevantFieldNames.size(); i++) {
			string irrelevantField = irrelevantFieldNames[i];
			string currentField = fieldNames[j];

			if (currentField == irrelevantField) {
				isIrrelevant = true;
			}
		}

		if (isIrrelevant == false) {
			outFieldNames.push_back(fieldNames[j]);
		}
	}

	writeDataRecord(outFieldNames, outFile);
}

void FileManager::writeDataRecord(vector<string> dataRecord, ofstream &outFile) {
	string line;

	for(size_t i=0; i < dataRecord.size(); i++) {
		line += dataRecord[i] + ",";
	}

	line += "\n";

	outFile << line;
}
