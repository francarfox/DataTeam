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

	ifstream* trainFile;
	ifstream* testFile;
	string predictFieldName;
	vector<string> categoryNames;
	vector<string> fieldNames;
	vector<vector<double> > gaussianDistribution;	// media y varianza de cada campo x categorias
	vector<int> totalCategoryRecords;	// total de registros por categoria para el calculo de la media

public:
	NaiveBayes(string trainFileName, string testFileName, string predictFieldName);
	virtual ~NaiveBayes();
	void train();
	void classification();

private:
	void getFieldNamesFromFirstLine();
	void doGaussianDistribution();
	void processCalculateMean();
	void processCalculateVariance();
	string getCategoryName(string dataString, int currentFieldIndex);
	int getPredictFieldIndex();
	void addForCalculateMean(vector<string> dataRecord, string categoryName);
	void addForCalculateVariance(vector<string> dataRecord, string categoryName);
	void calculateMean();
	void calculateVariance();
};

#endif /* NAIVEBAYES_H_ */
