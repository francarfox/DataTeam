/*
 * SetterData.cpp
 *
 *  Created on: Nov 27, 2015
 *      Author: fran
 */

#include "SetterData.h"
#include <sstream>

SetterData::SetterData() { }

SetterData::~SetterData() { }

string SetterData::getDataNumeric(string dataString, int currentFieldIndex) {

	if (currentFieldIndex > (int)data.size()-1) {
		// not found
		for (int i=data.size(); i<=currentFieldIndex; i++) {
			map<string, string> map;
			data.push_back(map);
		}
	}

	for (size_t i=0; i < data[currentFieldIndex].size(); i++) {
		string keyData = getString(i);
		string valueData = data[currentFieldIndex][keyData];

		if (valueData == dataString) {
			// devuelvo key como string
			return keyData;
		}
	}

	// agrego key value
	int numericKey = data[currentFieldIndex].size();
	string keyData = getString(numericKey);
	data[currentFieldIndex][keyData] = dataString;
	return keyData;
}

string SetterData::getString(int number) {
	stringstream stream;
	stream << number;
	return stream.str();
}
