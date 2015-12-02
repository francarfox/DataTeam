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
#include "SetterData.h"
using namespace std;

class NaiveBayesClassifier {

	int totalTestRecords;
	vector<string> testFieldNames;
	vector<string> irrelevantFieldNames;
	vector<string> fieldNamesWithoutIrrelevant;
	NaiveBayes *bayes;
	SetterData setterData;

public:
	NaiveBayesClassifier(NaiveBayes *bayes);
	virtual ~NaiveBayesClassifier();
	void test(int totalRecords, string testFileName, string submissionFileName);

private:
	void doClassification(ifstream &testFile, ofstream &submissionFile);
	void getFieldNamesFromFirstLine(ifstream &testFile, ofstream &submissionFile);
	void processClassification(ifstream &testFile, ofstream &submissionFile);
	void addDataNumeric(vector<double> &dataRecord, string dataString, int currentFieldIndex);
	void writeFieldNames(ofstream &submissionFile);
	void writeSubmissionFile(vector<double> &dataRecord, ofstream &submissionFile);
	void writeDataRecord(vector<string> dataRecord, ofstream &submissionFile);
	void generateFieldNamesWithoutIrrelevants();
	vector<double> calculateProbability(vector<double> dataRecord);
	double calculateProbabilityCategory(int category);
	double calculateProbabilityRespectTo(int category, int field, double data);
};

#endif /* NAIVEBAYESCLASSIFIER_H_ */
