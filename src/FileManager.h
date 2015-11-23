/*
 * FileManager.h
 *
 *  Created on: Nov 15, 2015
 *      Author: fran
 */

#ifndef FILEMANAGER_H_
#define FILEMANAGER_H_

#include <iostream>
#include <vector>
using namespace std;

class FileManager {
public:
	FileManager();
	virtual ~FileManager();

	//	Debe eliminar Descript, Resolution, Address
	void deleteIrrelevantFields(string inString, string outString);
	void cleanRegister(string registerLine);

private:
	vector<string> irrelevantFieldNames;
	vector<string> fieldNames;
};

#endif /* FILEMANAGER_H_ */
