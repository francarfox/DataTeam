/*
 * ConsoleLog.cpp
 *
 *  Created on: Nov 27, 2015
 *      Author: fran
 */

#include "ConsoleLog.h"

void log(string text) {

}

void logPercent(int current, int total) {
	float percent = 100 * current / total;
	cout << "Procesando: " << percent << "%" << endl;
}
