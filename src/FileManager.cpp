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

FileManager::~FileManager() { }

//	Debe eliminar Descript, Resolution, Address
void FileManager::process() {
//	Year,Month,Day,Hour,Category,Descript,DayOfWeek,PdDistrict,Resolution,Address,X,Y

	ifstream inFile(inFileName.c_str(), ios::in);
	ofstream outFile(outFileName.c_str(), ios::out);

	if (inFile.is_open() && outFile.is_open()) {
		cout << inFileName << " is opened!" << endl;
		cout << outFileName << " is opened!" << endl;

		bool readingFieldNames = true;
		string currentWord;
		int currentIrrelevantPOsition = 0;

//		getFieldNamesFromFirstLine(inFile, outFile);

		while (inFile.good()) {

		}

		while (inFile.good()) {
			char currentChar = (char)inFile.get();

			if (currentChar == '\n') {
				readingFieldNames = false;
				outFile << "Prueba\n";

				// paso a leer el proximo registro
			}

			if (readingFieldNames) {
				if (currentChar == ',') {
					// Guardo los nombres de los campos en la primera linea leida

//					if (currentWord == ) {
//
//					}

					fieldNames.push_back(currentWord);
				} else {
					currentWord += currentChar;
				}
			}

//			cout << (char) inFile.get() << endl;

//			cleanRegister(inFile.get());
		}

		inFile.close();
		outFile.close();
	} else {
		cout << "Error when open files " << endl;
	}
}

void FileManager::cleanRegister(string registerLine) {

	//Leo la primera linea que no me sirve (nombre de campos)
//		getline(infile, line);
//
//		//Guardo la palabra anterior
//		string pal_ant = "";
//		string pal_ant_ant = "";
//		string ngrams;
//		//Itero linea a linea, eliminando el id y puntaje de cada una
//		while(getline(infile, line)) {
//			istringstream iss(line);
//			iss >> word;
//			iss >> word;
//			while(iss >> word) {
//				if (!(mapStopw.count(word))){
//					mymap[word] += 1;
//				}
//				if (bool_ngrams){
//					//hago 2 grams
//					ngrams = pal_ant + " " + word;
//					if (!(mapStopw.count(ngrams))){
//						mymap[ngrams] += 1;
//					}
//				}
//			}
//		}
}
