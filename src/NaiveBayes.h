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

	int totalTrainRecords;
	int totalTestRecords;
	string predictFieldName;
	vector<string> categoryNames;
	vector<string> fieldNames;
	vector< vector<double> > meanDistribution;	// media de cada campo x categorias
	vector< vector<double> > varianceDistribution;	// varianza de cada campo x categorias
	vector<int> totalCategoryRecords;	// total de registros por categoria para el calculo de la media
	SetterData setterData;

public:
	NaiveBayes(string predictFieldName, SetterData setterData);
	virtual ~NaiveBayes();
	void train(int totalRecords, string fileName);
	void test(int totalRecords, string fileName);

private:
	void getFieldNamesFromFirstLine(ifstream &trainFile);
	void doGaussianDistribution(ifstream &trainFile);
	void processCalculateMean(ifstream &trainFile);
	void processCalculateVariance(ifstream &trainFile);
	void getCategoryName(string dataString, int currentFieldIndex, string &currentCategoryName);
	int getPredictFieldIndex();
	void addForCalculateMean(vector<string> dataRecord, string categoryName);
	void addForCalculateVariance(vector<string> dataRecord, string categoryName);
	void calculateMean();
	void calculateVariance();
	void verifyDataCategory(int category);
	vector<double> getNewVector();
};

#endif /* NAIVEBAYES_H_ */
