/*
 * SetterData.cpp
 *
 *  Created on: Nov 27, 2015
 *      Author: fran
 */

#include "SetterData.h"
#include <sstream>
#include <stdlib.h>

SetterData::SetterData() {
	map<int, string> map;
	dataConvert["Category"] = map;
	dataConvert["DayOfWeek"] = map;
	dataConvert["PdDistrict"] = map;
}

SetterData::~SetterData() { }

string SetterData::getDataNumeric(string dataString, string currentFieldName) {
	// Busco el dato numerico en el setteador
	for (size_t i=0; i < dataConvert[currentFieldName].size(); i++) {
		string valueData = dataConvert[currentFieldName][i];

		if (valueData == dataString) {
			// devuelvo key como string
			return getString(i);;
		}
	}

	// Agrego key value
	int keyData = dataConvert[currentFieldName].size();
	dataConvert[currentFieldName][keyData] = dataString;

	cout << "fieldName:" << currentFieldName
	<< " dataString:" << dataString
	<< " size:" << dataConvert[currentFieldName].size() << endl;
	return getString(keyData);
}

string SetterData::getString(int number) {
	stringstream stream;
	stream << number;
	return stream.str();
}

int SetterData::getInt(string numberString) {
	return atoi( numberString.c_str() );
}

double SetterData::getDouble(string numberString) {
	return atof(numberString.c_str());
}

string SetterData::getCategoryName(int categoryIndex) {
	cout << "dataConvert[Category][" << categoryIndex << "]=" << dataConvert["Category"][categoryIndex] << endl;
	return dataConvert["Category"][categoryIndex];
}
