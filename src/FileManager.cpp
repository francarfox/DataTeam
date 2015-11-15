/*
 * FileManager.cpp
 *
 *  Created on: Nov 15, 2015
 *      Author: fran
 */

#include "FileManager.h"
#include <fstream>

FileManager::FileManager() {
	// TODO Auto-generated constructor stub

}

FileManager::~FileManager() {
	// TODO Auto-generated destructor stub
}

//	Debe eliminar Descript, Resolution, Address
void FileManager::deleteIrrelevantFields(string inFileName, string outFileName) {

	ifstream inFile(inFileName.c_str(), ios::in);

	if (inFile.is_open())
	{
		cout << inFileName << " is opened!" << endl;

		while (inFile.good())
			cout << (char) inFile.get();

		inFile.close();
	} else
	{
		cout << "Error when open file: " << inFileName << endl;
	}
}
