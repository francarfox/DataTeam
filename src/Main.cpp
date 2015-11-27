//============================================================================
// Name        : DataTeam.cpp
// Author      : Fran
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include <iostream>
#include "FileManager.h"
#include "NaiveBayes.h"

using namespace std;

int main() {
	cout << "!!!DataTeam project start!!!" << endl;

	string trainFileName = string("../DataTeam/files/train.csv");
	string testFileName = string("../DataTeam/files/test.csv");
	string trainNewFileName = string("../DataTeam/files/trainNew.csv");

	//	Preparar el archivo para que funcione al pasarlo al NaiveBayes
	FileManager fileManager = FileManager(trainFileName, trainNewFileName);
	fileManager.process();

	//	Pasarle el archivo con los datos de los registros como numericos
	NaiveBayes naiveBayes = NaiveBayes(trainNewFileName, testFileName, "Category");
	naiveBayes.train();
	naiveBayes.classification();

	cout << "Finish run app" << endl;
	return 0;
}
