/*
 * SetterData.h
 *
 *  Created on: Nov 27, 2015
 *      Author: fran
 */

#ifndef SETTERDATA_H_
#define SETTERDATA_H_

#include <iostream>
#include <vector>
#include <map>
using namespace std;

class SetterData {

	vector< map<string, string> > data;

public:
	SetterData();
	virtual ~SetterData();
	string getDataNumeric(string dataString, int currentFieldIndex);
	string getString(int number);
};

#endif /* SETTERDATA_H_ */
