/*
 * NaiveBayes.h
 *
 *  Created on: Nov 26, 2015
 *      Author: fran
 */

#ifndef NAIVEBAYES_H_
#define NAIVEBAYES_H_

#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

class NaiveBayes {

	ifstream trainFile;
	string trainFileName;
	string predictFieldName;
	vector<string> categoryNames;
	vector<string> fieldNames;
	vector<vector<double> > gaussianDistribution;	// media y varianza de cada campo x categorias
	vector<int> totalCategoryRecords;	// total de registros por categoria para el calculo de la media

public:
	NaiveBayes(string trainFileName, string predictFieldName);
	virtual ~NaiveBayes();
	void train();

private:
	void getFieldNamesFromFirstLine();
	void doGaussianDistribution();
};

#endif /* NAIVEBAYES_H_ */
