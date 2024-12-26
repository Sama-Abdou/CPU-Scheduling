#include "lab6.h"
#include "output_generator.h"

vector<vector<char>> process_timeline;
unordered_map<char,int> finishTime;
unordered_map<char,int> TAT;
unordered_map<char,float> normTurn;

const string schedueling_algorithms[9] = {"","FCFS","RR","SPN","SRT","HRRN","FB-1","FB-2i","Aging"};

// Generic function to render timeline based on states of process
void renderTimeline(vector<Process> &process){
    for(auto& it_process_timeline: process_timeline){
        for(auto& it_process: process){
        
            if(it_process_timeline[0] == it_process.name){
                if(it_process.state == RUNNING){
                    it_process_timeline.push_back('*');
                    
                }
                else if(it_process.state == WAITING)
                    it_process_timeline.push_back('.');
                else{
                    it_process_timeline.push_back(' ');
                }
            }
        
        }
    }
}

void clearAllTemps(vector<Process> &process){
    process_timeline.clear();
    for(auto& it_process:process){
        process_timeline.push_back({it_process.name});
    }
    finishTime.clear();
    TAT.clear();
    normTurn.clear();
}

bool sortByRemainingTime(const Process& p1, const Process& p2){
    return p1.remainingTime > p2.remainingTime;
}

void ShortestRemainingTime(vector<Process> &process, Arguments args){
    
    for(int i=0;i<args.lastInstance;i++){
        

        // Min-Heap for remaining time
        priority_queue <Process, vector<Process>, decltype(&sortByRemainingTime) > shortest_rem_time(sortByRemainingTime); 
        for(auto& it_process:process){
            if(it_process.state!= FINISHED && it_process.arrivalTime<=i){
                shortest_rem_time.push(it_process); // Heap contains only arrived processes
                if(it_process.state == NOT_ARRIVED || it_process.state == RUNNING )
                    it_process.state = WAITING;
            }
        }
        
        for(auto& it_process: process){
            
            if(shortest_rem_time.top().name == it_process.name){
                it_process.state = RUNNING;
                it_process.remainingTime--;
            }
            
        }
        renderTimeline(process);

        // Checking for finishing
        for(auto& it_process: process){
            if(it_process.state!= FINISHED && it_process.remainingTime==0){
                it_process.state = FINISHED;
                finishTime[it_process.name] = i+1;
                TAT[it_process.name] = finishTime[it_process.name] - it_process.arrivalTime;
                normTurn[it_process.name] = (float) (TAT[it_process.name])/ it_process.serviceTime_priority;
            }
        }

        


    }
}

void FCFS(vector<Process> &process, Arguments args) {
    for (int i = 0; i < args.lastInstance; i++) {

        // Update process states based on arrival time
        for (auto& it_process : process) {
            if (it_process.state != FINISHED && it_process.arrivalTime <= i) {
                if (it_process.state == NOT_ARRIVED) {
                    it_process.state = WAITING;
                }
            }
        }

        for (auto& it_process : process) {
            if (it_process.state == WAITING || it_process.state == RUNNING) {
                it_process.state = RUNNING;

                it_process.remainingTime--;

                break; // make sure that only one process run (prevent overlapping) 
            }
        }
        renderTimeline(process);

        // Checking for finishing
        for(auto& it_process: process){
            if(it_process.state!= FINISHED && it_process.remainingTime==0){
                it_process.state = FINISHED;
                finishTime[it_process.name] = i+1;
                TAT[it_process.name] = finishTime[it_process.name] - it_process.arrivalTime;
                normTurn[it_process.name] = (float) (TAT[it_process.name]) / it_process.serviceTime_priority;
            }
        }
    }
}


int main(int argc, char** argv)
{
    Arguments args;

    // line 1
    string trace_stats;
    getline(cin, trace_stats);
    args.trace_stats = trace_stats; 

    // line 2
    string algorithms;
    getline(cin, algorithms);
    stringstream ss(algorithms); // Converting the algorithms line to string stream to tokenize it
    char del = ',';
    string algo_param; // algorithm and parameter (if there)

    while (!ss.eof()) {
        getline(ss, algo_param, del); // Tokenization using delimiter = ','
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
        getline(ss, t, ',');
        // Priority will be used in aging, service time otherwise
        p.serviceTime_priority = stoi(t);
        p.remainingTime = stoi(t);
        process_timeline.push_back({p.name});
        p.state = NOT_ARRIVED;

        process.push_back(p);
    }

    sort( process.begin(), process.end(),
              []( const Process &p1, const Process &p2 )
                 { return ( p1.arrivalTime < p2.arrivalTime ); });

    // Algorithms
    for(auto algorithm: args.algorithms){
        vector<Process> process_copy = process;
        switch (algorithm.first) 
        {
        case 1:
            FCFS(process_copy,args);
            if(args.trace_stats=="trace")
                printTrace(schedueling_algorithms[algorithm.first],args.lastInstance,process_timeline);
            if(args.trace_stats=="stats")
                printStats(schedueling_algorithms[algorithm.first],process_copy,finishTime,TAT,normTurn,args.numberOfProcess);
            clearAllTemps(process);
            break;
        case 2:
            // RR()
            break;
        case 3:
            // SPN()
            break;
        case 4:
            ShortestRemainingTime(process_copy,args);
            if(args.trace_stats=="trace")
                printTrace(schedueling_algorithms[algorithm.first],args.lastInstance,process_timeline);
            if(args.trace_stats=="stats")
                printStats(schedueling_algorithms[algorithm.first],process_copy,finishTime,TAT,normTurn,args.numberOfProcess);
            clearAllTemps(process);
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

