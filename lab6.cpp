#include <bits/stdc++.h> // stl library
using namespace std;

struct Argumnets {
    string trace_stats;
    int algorithm;
    int quantum;
    int lastInstance;
    int numberOfProcess;
};

struct Process {
    char name;
    int arrivalTime;
    int serviceTime; // burst time
    int priority;  // for Aging
};

void FCFS(Argumnets args, vector<Process> process){
    int finishTime[args.numberOfProcess];
    int TAT[args.numberOfProcess];
    float normTurn[args.numberOfProcess];
    
    if (args.trace_stats == "trace"){
        cout << "FCFS  ";
        for (int i = 0; i <= args.lastInstance; i++) cout << i % 10 << " ";
        cout << endl;
        cout << "------------------------------------------------" << endl;

        for (int i = 0; i < args.numberOfProcess; i++){
            finishTime[i] = (i == 0 ? process[i].arrivalTime : finishTime[i - 1]) + process[i].serviceTime;

            cout << process[i].name << "     ";

            for (int j = 0; j <= args.lastInstance; j++) {
                if (j >= process[i].arrivalTime && j < finishTime[i]) {
                    if (j < finishTime[i] - process[i].serviceTime) {
                        cout << "|.";
                    } else {
                        cout << "|*";
                    }
                } else {
                    cout << "| ";
                }
            }
            cout << endl;
        }
        cout << "------------------------------------------------" << endl << endl ;
    }
    else if (args.trace_stats == "stats"){
        cout << "FCFS" << endl;
        for (int i = 0; i < args.numberOfProcess; i++){
            finishTime[i] = (i == 0 ? process[i].arrivalTime : finishTime[i - 1]) + process[i].serviceTime;
            TAT[i] = finishTime[i] - process[i].arrivalTime;
            normTurn[i] = (float)TAT[i] / process[i].serviceTime;  
        }
        cout << "Process    |";
        for (const auto& p : process) {
            cout << setw(3) << p.name << setw(3) << "|";
        }
        cout << endl;

        cout << "Arrival    |";
        for (const auto& p : process) {
            cout << setw(3) << p.arrivalTime << setw(3) << "|";
        }
        cout << endl;

        cout << "Service    |";
        for (const auto& p : process) {
            cout << setw(3) << p.serviceTime << setw(3) << "|";
        }
        cout << " Mean|" << endl;

        cout << "Finish     |";
        for (int i = 0; i < args.numberOfProcess; i++) {
            cout << setw(3) << finishTime[i] << setw(3) << "|";
        }
        cout << "-----|" << endl;

        cout << "Turnaround |";
        float totalTAT = 0;
        for (int i = 0; i < args.numberOfProcess; i++) { 
            cout << setw(3) << TAT[i] << setw(3) << "|";
            totalTAT += TAT[i];
        }
        cout << fixed << setprecision(2) << " " << totalTAT / args.numberOfProcess << "|" << endl;

        cout << "NormTurn   |";
        float totalNormTurn = 0;
        for (int i = 0; i < args.numberOfProcess; i++) { 
            cout << fixed << setprecision(2) << setw(5) << normTurn[i] << "|";
            totalNormTurn += normTurn[i];
        }
        cout << fixed << setprecision(2) << " " << totalNormTurn / args.numberOfProcess << "|" << endl << endl;
    }
    else{
        cout << "Invalid Mode" << endl;
    }   
}

int main(int argc, char** argv)
{
    Argumnets args;

    // line 1
    string trace_stats;
    getline(cin, trace_stats);
    args.trace_stats = trace_stats;

    // line 2
    string policy;
    getline(cin, policy);
    int pos = policy.find('-');
    if (pos != -1){
        args.algorithm = stoi(policy.substr(0, pos)); 
        args.quantum = stoi(policy.substr(pos + 1));
    }
    else{
        args.algorithm = stoi(policy);
        args.quantum = -1;
    }

    // line 3
    int lastInstance;
    cin >> lastInstance;
    args.lastInstance = lastInstance;

    // line 4
    int numberOfProcess;
    cin >> numberOfProcess;
    args.numberOfProcess = numberOfProcess;
    cin.ignore();

    // line 5
    vector<Process> process;
    for (int i = 0; i < numberOfProcess; i++){
        Process p;

        string processLine;
        getline(cin, processLine);

        stringstream ss(processLine);
        string t;

        // Parse process details
        getline(ss, t, ','); // Name
        p.name = t[0];

        getline(ss, t, ','); // Arrival Time
        p.arrivalTime = stoi(t);

        if (args.algorithm == 8) {
            getline(ss, t, ',');
            p.priority = stoi(t);
        } 
        else{
            getline(ss, t, ',');
            p.serviceTime = stoi(t);
        }
        process.push_back(p);
    }

    sort( process.begin(), process.end(),
              []( const Process &p1, const Process &p2 )
                 { return ( p1.arrivalTime < p2.arrivalTime ); });

    // Algorithms
    switch (args.algorithm)
    {
    case 1:
        FCFS(args, process);
        break;
    case 2:
        // RR()
        break;
    case 3:
        // SPN()
        break;
    case 4:
        // SRT()
        break;
    case 5:
        // HRRN()
        break;
    case 6:
        // FB-1()
        break;
    case 7:
        // FB-2i()
        break;
    case 8:
        // aging()
        break;
     
    default:
        break;
    }

    return 0;
}