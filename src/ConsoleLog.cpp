/*
 * ConsoleLog.cpp
 *
 *  Created on: Nov 27, 2015
 *      Author: fran
 */

#include "ConsoleLog.h"
#include <iomanip>
using namespace std;

void log(string text) {

}

void logPercent(string processName, int current, int total) {
	float percent = 100 * (float)current / (float)total;
	cout << "\r" << processName << ": ";
	cout << fixed << percent << setprecision(2) << "%   ";
}
