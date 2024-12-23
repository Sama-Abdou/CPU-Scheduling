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
    int serviceTime;
    int priority;  // for Aging
};

int main(int argc, char** argv)
{
    Argumnets args;

    // line 1
    string trace_stats;
    getline(cin, trace_stats);
    args.trace_stats = trace_stats;
    cout << "mode: "<< args.trace_stats << endl;

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
    cout <<"algorithm: "<< args.algorithm << endl;
    cout <<"quantum: "<< args.quantum << endl;

    // line 3
    int lastInstance;
    cin >> lastInstance;
    args.lastInstance = lastInstance;
    cout << "last: "<< args.lastInstance << endl;

    // line 4
    int numberOfProcess;
    cin >> numberOfProcess;
    args.numberOfProcess = numberOfProcess;
    cout << "number: "<< args.numberOfProcess << endl;
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
        cout << "name:" << p.name << endl;
        cout << "arrival: " << p.arrivalTime << endl;
        cout << "service: " << p.serviceTime << endl;
        cout << "priority: " << p.priority << endl;
        process.push_back(p);
    }

    // Algorithms
    switch (args.algorithm)
    {
    case 1:
        // FCFS()
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