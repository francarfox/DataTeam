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
#include "SetterData.h"
using namespace std;

class NaiveBayes {

	string predictFieldName;
	vector<string> fieldNames;
	SetterData setterData;

public:
	int totalTrainRecords;
	vector<int> totalCategoryRecords;	// total de registros por categoria para el calculo de la media
	vector< vector<double> > meanDistribution;	// media de cada campo x categorias
	vector< vector<double> > varianceDistribution;	// varianza de cada campo x categorias

	NaiveBayes(string predictFieldName, SetterData setterData);
	virtual ~NaiveBayes();
	void train(int totalRecords, string fileName);
	void test(int totalRecords, string fileName);
	void getFieldNamesFromFirstLine(ifstream &trainFile, vector<string> &fieldNames);

private:
	void doGaussianDistribution(ifstream &trainFile);
	void ignoreFieldNamesFromFirstLine(ifstream &trainFile);
	void processCalculateMean(ifstream &trainFile);
	void processCalculateVariance(ifstream &trainFile);
	void processDataString(vector<string> &dataRecord, string dataString, int currentFieldIndex, string &currentCategoryName);
	int getPredictFieldIndex();
	void addForCalculateMean(vector<string> dataRecord, string categoryName);
	void addForCalculateVariance(vector<string> dataRecord, string categoryName);
	void calculateMean();
	void calculateVariance();
	void verifyDataCategory(int category);
	vector<double> getNewVector();
};

#endif /* NAIVEBAYES_H_ */
