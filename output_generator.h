#ifndef OUTPUT_GENERATOR_H
#define OUTPUT_GENERATOR_H

#include "lab6.h"
const string schedueling_algorithms[9] = {"","FCFS","RR","SPN","SRT","HRRN","FB-1","FB-2i","Aging"};

void printHeader(pair<int,int> algo, int lastInstance);
void printFooter();
void printStats(pair<int,int> algo, vector<Process> &process, unordered_map<char,int> &finishTime, unordered_map<char,int> &TAT, unordered_map<char,float> &normTurn, int numberOfProcess);
void printTrace(pair<int,int> algo, int lastInstance, vector<vector<char>> &processes_timeline);


#endif // OUTPUT_GENERATOR_H
