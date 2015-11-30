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
#include "SetterData.h"
using namespace std;

class FileManager {

	int totalRecords;
	string inFileName;
	string outFileName;
	vector<string> fieldNames;
	vector<string> irrelevantFieldNames;
	vector<string> numericFieldNames;
	SetterData setterData;

public:
	FileManager(int totalRecords, string inFileName, string outFileName);
	virtual ~FileManager();
	void process();
	SetterData getSetterData();

private:
	void getFieldNamesFromFirstLine(ifstream &inFile, ofstream &outFile);
	void generateOutFile(ifstream &inFile, ofstream &outFile);
	void addDataNumeric(vector<string> &dataRecord, string dataString, int currentFieldIndex);
	void writeFieldNames(ofstream &outFile);
	void writeDataRecord(vector<string> dataRecord, ofstream &outFile);
};

#endif /* FILEMANAGER_H_ */
