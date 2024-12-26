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
void HighestResponseRatioNext(vector<Process> &process, Arguments args){
    int i=0;
    while(i<args.lastInstance){
        pair<int, char> max_response_ratio = {INT_MIN,'-'};
        for(auto& it_process:process){
            if(it_process.state!= FINISHED && it_process.arrivalTime<=i){
                if(it_process.state == NOT_ARRIVED || it_process.state == RUNNING )
                    it_process.state = WAITING;
                int waiting = i-it_process.arrivalTime;
                int response_ratio = (waiting + it_process.serviceTime_priority)/it_process.serviceTime_priority;
                if(response_ratio>max_response_ratio.first){
                    max_response_ratio.second = it_process.name;
                    max_response_ratio.first = response_ratio;
                }
            }
        }
    
        for(auto& it_process:process){
            if(max_response_ratio.second==it_process.name){
                it_process.state = RUNNING;
                it_process.remainingTime--;
                renderTimeline(process);
                while(it_process.remainingTime){ // Non pre-emptive algorithm
                    i++;
                    for(auto& it_process_check:process){
                        if(it_process_check.state!= FINISHED && it_process_check.arrivalTime<=i){
                             if(it_process_check.state == NOT_ARRIVED || it_process_check.state == RUNNING )
                                   it_process_check.state = WAITING;
                        }
                    }
                    it_process.state = RUNNING;
                    it_process.remainingTime--;
                    renderTimeline(process);
                }
                it_process.state=FINISHED;
                finishTime[it_process.name] = i+1;
                TAT[it_process.name] = finishTime[it_process.name] - it_process.arrivalTime;
                normTurn[it_process.name] = (float) (TAT[it_process.name])/ it_process.serviceTime_priority;
                break;


            }
    
        }
        i++;
    }
}

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
        p.remainingTime = stoi(t);
        process_timeline.push_back({p.name});
        p.state = NOT_ARRIVED;

        cout<<endl;
        cout << "name:" << p.name << endl;
        cout << "arrival: " << p.arrivalTime << endl;
        cout << "service/priority: " << p.serviceTime_priority << endl;
        cout<<endl;

        process.push_back(p);
    }

    // Algorithms
    for(auto algorithm: args.algorithms){
        vector<Process> process_copy = process;
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
            ShortestRemainingTime(process_copy,args);
            if(args.trace_stats=="trace")
                printTrace(schedueling_algorithms[algorithm.first],args.lastInstance,process_timeline);
            if(args.trace_stats=="stats")
                printStats(schedueling_algorithms[algorithm.first],process_copy,finishTime,TAT,normTurn,args.numberOfProcess);
            clearAllTemps(process);
            break;
        case 5:
            HighestResponseRatioNext(process_copy,args);
            if(args.trace_stats=="trace")
                printTrace(schedueling_algorithms[algorithm.first],args.lastInstance,process_timeline);
            if(args.trace_stats=="stats")
                printStats(schedueling_algorithms[algorithm.first],process_copy,finishTime,TAT,normTurn,args.numberOfProcess);
            clearAllTemps(process);
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

