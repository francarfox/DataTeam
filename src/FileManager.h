/*
 * FileManager.h
 *
 *  Created on: Nov 15, 2015
 *      Author: fran
 */

#ifndef FILEMANAGER_H_
#define FILEMANAGER_H_

#include <iostream>
using namespace std;

class FileManager {
public:
	FileManager();
	virtual ~FileManager();

	//	Debe eliminar Descript, Resolution, Address
	void deleteIrrelevantFields(string inString, string outString);
};

#endif /* FILEMANAGER_H_ */
