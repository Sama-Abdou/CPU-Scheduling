#include <bits/stdc++.h> // stl library
using namespace std;

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
};

int main(int argc, char** argv)
{
    Arguments args;

    // line 1
    string trace_stats;
    getline(cin, trace_stats);
    args.trace_stats = trace_stats;
    cout << "mode: "<< args.trace_stats << endl;

    // line 2
    string algorithms;
    getline(cin, algorithms);
    stringstream ss(algorithms); // Converting the algorithms line to string stream to tokenize it
    char del = ',';
    string algo_param; // algorithm and parameter (if there)

    while (!ss.eof()) {
        getline(ss, algo_param, del); // Tokenization using delimiter = ','
        cout << algo_param << endl;
        int pos = algo_param.find('-');

        if (pos != -1){ //it means there is - (dash) in the token (algorithm) e.g. 5-6
            int algorithm = stoi(algo_param.substr(0, pos));
            int parameter = stoi(algo_param.substr(pos + 1));
            args.algorithms.push_back({algorithm,parameter});
        }
        else{ // it means there is no dash in this token
            args.algorithms.push_back({stoi(algo_param),-1});
        }
    }
    for(auto it:args.algorithms){
        cout <<"algorithm: "<< it.first << endl;
        cout <<"quantum: "<< it.second << endl; // it will be negative one if no parameter
        cout<<endl;
    }

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
        getline(ss, t, ',');
        // Priority will be used in aging, service time otherwise
        p.serviceTime_priority = stoi(t);
        cout<<endl;
        cout << "name:" << p.name << endl;
        cout << "arrival: " << p.arrivalTime << endl;
        cout << "service/priority: " << p.serviceTime_priority << endl;
        cout<<endl;

        process.push_back(p);
    }

    // Algorithms
    for(auto algorithm: args.algorithms){
        switch (algorithm.first)
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
    }

     return 0;
}