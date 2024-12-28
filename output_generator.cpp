#include "output_generator.h"

void printHeader(pair<int,int> algo, int lastInstance) {
    if(algo.second==-1) {
        cout << schedueling_algorithms[algo.first];
        cout << string(6 - schedueling_algorithms[algo.first].length(), ' ');
    } else {
        if (schedueling_algorithms[algo.first] != "Aging"){
            string header = schedueling_algorithms[algo.first] + "-" + to_string(algo.second);
            cout << header;
            cout << string(6 - header.length(), ' ');
        }
        else{
            cout << schedueling_algorithms[algo.first];
            cout << string(6 - schedueling_algorithms[algo.first].length(), ' ');
        }
    }
    
    for (int i = 0; i <= lastInstance; i++) {
        cout << i % 10 << " ";
    }
    cout << endl;
    cout << string(48, '-') << endl;
}

void printFooter() {
    cout << string(48, '-') << endl << endl;
}

void printStats(pair<int,int> algo, vector<Process> &process, unordered_map<char,int> &finishTime, unordered_map<char,int> &TAT, unordered_map<char,float> &normTurn, int numberOfProcess) {
    float totalTAT = 0, totalNormTurn = 0;
    if(algo.second==-1)
        cout << schedueling_algorithms[algo.first]  << endl ;
    else{
        cout << schedueling_algorithms[algo.first] <<'-'<<algo.second << endl ;
    }
    cout << "Process    |";
    for (const auto &p : process) {
        cout << setw(3) << p.name << setw(3) << "|";
    }
    cout << endl;

    cout << "Arrival    |";
    for (const auto &p : process) {
        cout << setw(3) << p.arrivalTime << setw(3) << "|";
    }
    cout << endl;

    cout << "Service    |";
    for (const auto &p : process) {
        cout << setw(3) << p.serviceTime_priority << setw(3) << "|";
    }
    cout << " Mean|" << endl;

    cout << "Finish     |";
    for (int i = 0; i < numberOfProcess; i++) {
        cout << setw(3) << finishTime[process[i].name] << setw(3) << "|";
    }
    cout << "-----|" << endl;

    cout << "Turnaround |";
    for (int i = 0; i < numberOfProcess; i++) {
        cout << setw(3) << TAT[process[i].name] << setw(3) << "|";
        totalTAT += TAT[process[i].name];
    }
    cout << fixed << setprecision(2) << setw(5) << totalTAT / numberOfProcess << "|" << endl;

    cout << "NormTurn   |";
    for (int i = 0; i < numberOfProcess; i++) {
        cout << fixed << setprecision(2) << setw(5) << normTurn[process[i].name] << "|";
        totalNormTurn += normTurn[process[i].name];
    }
    cout << fixed << setprecision(2) << setw(5) << totalNormTurn / numberOfProcess << "|" << endl << endl;
}

void printTrace(pair<int,int> algo, int lastInstance, vector<vector<char>> &processes_timeline){
    printHeader(algo, lastInstance);
    for(auto process:processes_timeline){
        cout<<process[0]<<"     ";
        for(int i=1;i<process.size();i++){
            cout<<"|";
            cout<<process[i];
        }
        cout<<"| ";
        cout<<endl;
    }
    printFooter();
}