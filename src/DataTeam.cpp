//============================================================================
// Name        : DataTeam.cpp
// Author      : Fran
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include <iostream>
#include <stdio.h>
using namespace std;

//	Debe eliminar descripcion y resolucion
void deleteIrrelevantFields(string inString, string outString){
	FILE *fileIn = fopen(inString.c_str(), "r");
	FILE *fileOut = fopen(outString.c_str(), "w");

	while(!feof(fileIn)){
//		current = fgetc(fileIn);
//		fputc(current, fileOut);
	}

	fclose(fileIn);
	fclose(fileOut);
}

int main() {
	cout << "!!!DataTeam project start!!!" << endl;

//	Eliminando campos irrelevantes de set de entrenamiento
	string trainFile = string("../files/train.csv");
	deleteIrrelevantFields(trainFile, "../files/trainWithoutIrrelevantFields.csv");

	return 0;
}
