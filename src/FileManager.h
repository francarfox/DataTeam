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

	ifstream *inFile;
	ofstream *outFile;
	vector<string> irrelevantFieldNames;
	vector<string> fieldNames;
	vector<int> irrelevantPositions;

public:
	FileManager(string inFileName, string outFileName);
	virtual ~FileManager();
	void process();

private:
	void getFieldNamesFromFirstLine();
	void generateOutFile();
};

#endif /* FILEMANAGER_H_ */
