/*
 * FileManager.cpp
 *
 *  Created on: Nov 15, 2015
 *      Author: fran
 */

#include "FileManager.h"
#include <fstream>
#include "ConsoleLog.h"

FileManager::FileManager(int totalTrainRecords, string inFileName, string outFileName) {
	this->totalTrainRecords = totalTrainRecords;
	this->inFileName = inFileName;
	this->outFileName = outFileName;

	this->irrelevantFieldNames.push_back("Year");
	this->irrelevantFieldNames.push_back("Month");
	this->irrelevantFieldNames.push_back("Day");
	this->irrelevantFieldNames.push_back("Descript");
	this->irrelevantFieldNames.push_back("Resolution");
	this->irrelevantFieldNames.push_back("Address");
}

FileManager::~FileManager() { }

void FileManager::process() {
	ifstream inFile(inFileName.c_str(), ios::in);
	ofstream outFile(outFileName.c_str(), ios::out);

	if (inFile.is_open() && outFile.is_open()) {
		cout << "Procesando archivo de entrenamiento para limpieza..." << endl;

		// Obtengo nombre de los campos con la primera linea leida de inFile
		getFieldNamesFromFirstLine(inFile);
		// Generamos el nuevo archivo con los campos numericos
		generateOutFile(inFile, outFile);

		inFile.close();
		outFile.close();
	} else {
		cout << "Error when open files " << endl;
	}
}

void FileManager::getFieldNamesFromFirstLine(ifstream &inFile) {
	cout << "Obteniendo nombre de los campos..." << endl;

	string fieldName;
	char currentChar;

	while (inFile.good() && currentChar != '\n') {
		currentChar = (char)inFile.get();

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

//	Year,Month,Day,Hour,Category,Descript,DayOfWeek,PdDistrict,Resolution,Address,X,Y
void FileManager::generateOutFile(ifstream &inFile, ofstream &outFile) {
	cout << "Generando nuevo archivo de entrenamiento..." << endl;

	int currentWriteRecords = 0;

	while (inFile.good()) {
		vector<string> dataRecord;	// cache de los datos del registro a leer
		string currentCategoryName;	// nombre de la categoria del registro a leer
		string dataString = "";
		char currentChar = '0';
//		int currentFieldIndex = 0;

		// Proceso por cada registro
		while (inFile.good() && currentChar != '\n') {
			currentChar = (char)inFile.get();

			if (currentChar == ',') {
				// Obtengo el nombre de la categoria del registro
//				currentCategoryName = getCategoryName(dataString, currentFieldIndex++);
				// Guardo en cache los datos del registro
				dataRecord.push_back(dataString.c_str());
				dataString = "";
			} else {
				dataString += currentChar;
			}
		}

		outFile << "Prueba\n";
		logPercent(++currentWriteRecords, totalTrainRecords);
	}
}
