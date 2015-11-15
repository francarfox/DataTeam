//============================================================================
// Name        : DataTeam.cpp
// Author      : Fran
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <list>
using namespace std;

//	Debe eliminar descripcion y resolucion
void deleteIrrelevantFields(string inString, string outString){
	FILE *fileIn = fopen(inString.c_str(), "r");
	FILE *fileOut = fopen(outString.c_str(), "w");

	int i = 0;
	while(!feof(fileIn)){
		cout << i++ << endl;
//		current = fgetc(fileIn);
//		fputc(current, fileOut);
	}

	fclose(fileIn);
	fclose(fileOut);
}

int main() {
	cout << "!!!DataTeam project start!!!" << endl;

//	Eliminando campos irrelevantes de set de entrenamiento
	string trainFileName = string("../DataTeam/files/train.csv");
//	deleteIrrelevantFields(trainFile, "../Data/trainWithoutIrrelevantFields.csv");

	ifstream trainFile(trainFileName.c_str(), ios::in);
//	infile.open (trainFileName.c_str());
	if (trainFile.is_open())
	{
		cout << "train.csv opened!" << endl;

		while (trainFile.good())
			cout << (char) trainFile.get();

		trainFile.close();
	} else
	{
		cout << "train.csv error when open file" << endl;
	}

	return 0;
}
