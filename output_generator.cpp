#include "output_generator.h"

void printHeader(string name, int lastInstance) {
    cout << name << "\033[7G" ;
    for (int i = 0; i <= lastInstance; i++) cout << i % 10 << " ";
    cout << endl;
    cout << string(48, '-') << endl;
}

void printFooter() {
    cout << string(48, '-') << endl << endl;
}

void printStats(string name, vector<Process> &process, vector<int> &finishTime, vector<int> &TAT, vector<float> &normTurn, int numberOfProcess) {
    float totalTAT = 0, totalNormTurn = 0;
    cout << name << endl;
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
        cout << setw(3) << finishTime[i] << setw(3) << "|";
    }
    cout << "-----|" << endl;

    cout << "Turnaround |";
    for (int i = 0; i < numberOfProcess; i++) {
        cout << setw(3) << TAT[i] << setw(3) << "|";
        totalTAT += TAT[i];
    }
    cout << fixed << setprecision(2) << " " << totalTAT / numberOfProcess << "|" << endl;

    cout << "NormTurn   |";
    for (int i = 0; i < numberOfProcess; i++) {
        cout << fixed << setprecision(2) << setw(5) << normTurn[i] << "|";
        totalNormTurn += normTurn[i];
    }
    cout << fixed << setprecision(2) << " " << totalNormTurn / numberOfProcess << "|" << endl << endl;
}

void printTrace(string name, int lastInstance, vector<vector<char>> &processes_timeline){
    printHeader(name, lastInstance);
    for(auto process:processes_timeline){
        cout<<process[0]<<"     ";
        for(int i=1;i<process.size();i++){
            cout<<"|";
            cout<<process[i];
        }
        cout<<"|";
        cout<<endl;
    }
    printFooter();
}