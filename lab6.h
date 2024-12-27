#ifndef ARGUMENTS_H  // To avoid including the file multiple times 
#define ARGUMENTS_H

#include <bits/stdc++.h> // stl library
using namespace std;



enum state {RUNNING, WAITING, NOT_ARRIVED, FINISHED};

struct Arguments {
    string trace_stats;
    vector<pair<int,int>> algorithms; //algorithm[i].first = algorithm number, algorithm[i].second = algorithm parameter (quantum) (if applicable, -1 otherwise).
    int lastInstance;
    int numberOfProcess;
    
};

struct Process {
    char name;
    int arrivalTime;
    int serviceTime_priority;
    int remainingTime; 
    enum state state;
};

#endif 
