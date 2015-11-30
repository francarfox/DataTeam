/*
 * ConsoleLog.cpp
 *
 *  Created on: Nov 27, 2015
 *      Author: fran
 */

#include "ConsoleLog.h"
#include <stdio.h>

void log(string text) {

}

void logPercent(string processName, int current, int total) {
	float percent = 100 * current / total;
	cout << "\r" << processName << ": " << percent << "%   ";

	if (current >= total) {
		cout << endl;
	}
}
