//============================================================================
// Name        : DataTeam.cpp
// Author      : Fran
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include <iostream>
#include "FileManager.h"

using namespace std;

int main() {
	cout << "!!!DataTeam project start!!!" << endl;

	string trainFileName = string("../DataTeam/files/train.csv");
	string testFileName = string("../DataTeam/files/test.csv");
	string trainCleanFileName = string("../DataTeam/files/trainClean.csv");

	FileManager fileManager = FileManager();

	//	Eliminando campos irrelevantes de set de entrenamiento
	fileManager.deleteIrrelevantFields(trainFileName, trainCleanFileName);

	return 0;
}
