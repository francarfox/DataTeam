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

	int totalTrainRecords;
	string inFileName;
	string outFileName;
	vector<string> irrelevantFieldNames;
	vector<string> fieldNames;
	vector<int> irrelevantPositions;

public:
	FileManager(int totalTrainRecords, string inFileName, string outFileName);
	virtual ~FileManager();
	void process();

private:
	void getFieldNamesFromFirstLine(ifstream &inFile);
	void generateOutFile(ifstream &inFile, ofstream &outFile);
};

#endif /* FILEMANAGER_H_ */
