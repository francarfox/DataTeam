/*
 * NaiveBayesClassifier.h
 *
 *  Created on: Nov 30, 2015
 *      Author: fran
 */

#ifndef NAIVEBAYESCLASSIFIER_H_
#define NAIVEBAYESCLASSIFIER_H_

#include <iostream>
#include "NaiveBayes.h"
using namespace std;

class NaiveBayesClassifier {

	int totalTestRecords;
	vector<string> testFieldNames;
	NaiveBayes *bayes;

public:
	NaiveBayesClassifier(NaiveBayes *bayes);
	virtual ~NaiveBayesClassifier();
	void test(int totalRecords, string fileName);

private:
	void doClassification(ifstream &testFile);
	void processClassification(ifstream &testFile);
	vector<double> calculateProbability(vector<double> dataRecord);
	double calculateProbabilityCategory(int category);
	double calculateProbabilityRespectTo(int category, int field, double data);
};

#endif /* NAIVEBAYESCLASSIFIER_H_ */
