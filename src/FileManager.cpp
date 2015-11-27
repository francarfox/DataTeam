/*
 * FileManager.cpp
 *
 *  Created on: Nov 15, 2015
 *      Author: fran
 */

#include "FileManager.h"
#include <fstream>

FileManager::FileManager(string inFileName, string outFileName) {
	this->inFile->open(inFileName.c_str(), ios::in);
	this->outFile->open(outFileName.c_str(), ios::out);

	this->irrelevantFieldNames.push_back("Year");
	this->irrelevantFieldNames.push_back("Month");
	this->irrelevantFieldNames.push_back("Day");
	this->irrelevantFieldNames.push_back("Descript");
	this->irrelevantFieldNames.push_back("Resolution");
	this->irrelevantFieldNames.push_back("Address");
}

FileManager::~FileManager() {
	this->inFile = NULL;
	this->outFile = NULL;
}

void FileManager::process() {
	if (inFile->is_open() && outFile->is_open()) {
		cout << "Procesando archivo de entrenamiento para limpieza..." << endl;

		// Obtengo nombre de los campos con la primera linea leida de inFile
		getFieldNamesFromFirstLine();
		// Generamos el nuevo archivo con los campos numericos
		generateOutFile();

		inFile->close();
		outFile->close();
	} else {
		cout << "Error when open files " << endl;
	}
}

void FileManager::getFieldNamesFromFirstLine() {
	cout << "Obteniendo nombre de los campos..." << endl;

	string fieldName;
	char currentChar;

	while (inFile->good() && currentChar != '\n') {
		currentChar = (char)inFile->get();

		if (currentChar == ',') {
			fieldNames.push_back(fieldName);
			fieldName = "";
		} else {
			fieldName += currentChar;
		}
	}
}

//	Year,Month,Day,Hour,Category,Descript,DayOfWeek,PdDistrict,Resolution,Address,X,Y
void FileManager::generateOutFile() {
	cout << "Generando nuevo archivo de entrenamiento..." << endl;

	while (inFile->good()) {
		vector<string> dataRecord;	// cache de los datos del registro a leer
		string currentCategoryName;	// nombre de la categoria del registro a leer
		string dataString = "";
		char currentChar;
//		int currentFieldIndex = 0;

		// Proceso por cada registro
		while (currentChar != '\n') {
			currentChar = (char)inFile->get();

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

		outFile->write("Prueba\n", 6);
		// paso a leer el proximo registro
		// sumo datos para el posterior calculo de la media sabiedo la categoria del registro
//		addForCalculateMean(dataRecord, currentCategoryName);
	}
}
