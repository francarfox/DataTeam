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

	// Cumple la funcion de setteador de datos para Category, DayOfWeek, PdDistrict
	map< string, map<int, string> > dataConvert;

public:
	SetterData();
	virtual ~SetterData();
	string getDataNumeric(string dataString, string currentFieldName);
	string getString(int number);
	int getInt(string numberString);
	double getDouble(string numberString);

	string getCategoryName(int categoryIndex);
	string getPdDistrictName(int pdDistrictIndex);
};

#endif /* SETTERDATA_H_ */
