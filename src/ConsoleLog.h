/*
 * ConsoleLog.h
 *
 *  Created on: Nov 27, 2015
 *      Author: fran
 */

#ifndef CONSOLELOG_H_
#define CONSOLELOG_H_

#include <iostream>
using namespace std;

struct ConsoleLog
{
};

void log(string text);
void logPercent(int current, int total);

#endif /* CONSOLELOG_H_ */
