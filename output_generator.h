#ifndef OUTPUT_GENERATOR_H
#define OUTPUT_GENERATOR_H

#include "lab6.h"

void printHeader(string name, int lastInstance);
void printFooter();
void printStats(string name, vector<Process> &process, vector<int> finishTime, vector<int> TAT, vector<float> normTurn, int numberOfProcess);
void printTrace(string name, int lastInstance, vector<vector<char>> &processes_timeline);


#endif // OUTPUT_GENERATOR_H
