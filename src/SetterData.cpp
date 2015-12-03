/*
 * SetterData.cpp
 *
 *  Created on: Nov 27, 2015
 *      Author: fran
 */

#include "SetterData.h"
#include <sstream>
#include <stdlib.h>
#include <algorithm>

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

vector<string> SetterData::getCategoryNames() {
	orderCategories();
	return categoryNames;
}

string SetterData::getCategoryName(int categoryIndex) {
	return dataConvert["Category"][categoryIndex];
}

int SetterData::getDayOfWeek(string dataString) {

	for(size_t i=0; i < dataConvert["DayOfWeek"].size(); i++) {
		string currentData = dataConvert["DayOfWeek"][i];

		if (currentData == dataString) {
			return i;
		}
	}

	return -1;
}

int SetterData::getPdDistrict(string dataString) {

	for(size_t i=0; i < dataConvert["PdDistrict"].size(); i++) {
		string currentData = dataConvert["PdDistrict"][i];

		if (currentData == dataString) {
			return i;
		}
	}

	return -1;
}

void SetterData::orderCategories() {

	for(size_t i=0; i < dataConvert["Category"].size(); i++) {
		string valueData = dataConvert["Category"][i];
		categoryNames.push_back(valueData);
	}

	sort(categoryNames.begin(), categoryNames.begin()+categoryNames.size());
}

string SetterData::getPdDistrictName(int pdDistrictIndex) {
	//cout << "dataConvert[PdDistrict][" << pdDistrictIndex << "]=" << dataConvert["PdDistrict"][pdDistrictIndex] << endl;
	return dataConvert["PdDistrict"][pdDistrictIndex];
}
