#include "lab6.h"
#include "output_generator.h"



vector<vector<char>> process_timeline;
unordered_map<char,int> finishTime;
unordered_map<char,int> TAT;
unordered_map<char,float> normTurn;


void clearAllTemps(vector<Process> &process){
    process_timeline.clear();
    for(auto& it_process:process){
        process_timeline.push_back({it_process.name});
    }
    finishTime.clear();
    TAT.clear();
    normTurn.clear();
}

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
bool sortByArrivalTime(const Process& p1, const Process& p2){
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
                
                
                it_process.state = FINISHED;
                finishTime[it_process.name] = i+1;
                TAT[it_process.name] = finishTime[it_process.name] - it_process.arrivalTime;
                normTurn[it_process.name] = (float)(TAT[it_process.name]) / it_process.serviceTime_priority;
                break;


            }
    
        }
        i++;
    }
}
void FB(vector<Process> &processes, Arguments args,bool fb_2i) { 
    unordered_map<int, queue<Process*>> FBQ; // Feedback queues
    unordered_set<char> processes_inserted; // Track inserted processes

    int i = 0;
    
    // Simulation loop until the last time instance
    while (i < args.lastInstance) {
        // Add processes to the first queue based on arrival time
        for (auto &proc : processes) {
            if (proc.state != FINISHED && proc.arrivalTime <= i) {
                proc.state = WAITING; // Correctly set to WAITING
                if( processes_inserted.find(proc.name) == processes_inserted.end()) {
                   
                    processes_inserted.insert(proc.name);
                    FBQ[0].push(&proc); 
                }
            }
        }

        // Process feedback queues
        bool processed = false;
        for (int level = 0; level < FBQ.size(); level++) {
            if (!FBQ[level].empty()) {
                Process *current = FBQ[level].front();
                FBQ[level].pop();
                
                int limit = fb_2i? pow(2,level):1;
                for(int j=0;j<limit;j++){
                    for (auto &proc : processes) {
                        if (proc.state != FINISHED && proc.arrivalTime <= i) {
                        
                            proc.state = WAITING; 
                            if(processes_inserted.find(proc.name) == processes_inserted.end()){
                                processes_inserted.insert(proc.name);
                                FBQ[0].push(&proc); 
                            }
                        }
                    }
                    current->state = RUNNING;  
                    current->remainingTime--;
                    i++;
                    renderTimeline(processes);
                    if (current->remainingTime == 0) break;
                    
                    
                }
                // Check for processes because i is incremented.
                for (auto &proc : processes) {
                    if (proc.state != FINISHED && proc.arrivalTime <= i) {
                    
                        proc.state = WAITING; 
                        if(processes_inserted.find(proc.name) == processes_inserted.end()){
                            processes_inserted.insert(proc.name);
                            FBQ[0].push(&proc); 
                        }
                    }
                }

                

                // Check if the process is finished
                if (current->remainingTime == 0) {
                    current->state = FINISHED;
                    //renderTimeline(processes);
                    finishTime[current->name] = i;
                    TAT[current->name] = finishTime[current->name] - current->arrivalTime;
                    normTurn[current->name] = (float)(TAT[current->name]) / current->serviceTime_priority;
                } else {
                    if(processes_inserted.size()==1)
                        FBQ[level].push(current);
                    else
                        FBQ[level + 1].push(current); 
                }

                processed = true;
                break; // Only one process executes at a time
            }
        }
    }
}
void RoundRobin(vector<Process> &processes, Arguments args,int parameter){
    int i=0;
    int processIndex=-1;
    queue<Process*> processes_arrived;
    unordered_set<char> processes_inserted;
    sort(processes.begin(),processes.end(),sortByArrivalTime);
    while(i<args.lastInstance){
        for(auto &it_process:processes){
            if(it_process.state!= FINISHED && it_process.arrivalTime<=i){
                    if(it_process.state == NOT_ARRIVED || it_process.state == RUNNING )
                        it_process.state = WAITING;
                        if( processes_inserted.find(it_process.name) == processes_inserted.end()) {
                   
                            processes_inserted.insert(it_process.name);
                            processes_arrived.push(&it_process);
                        }
            }
        }
        int counter = parameter;
        Process* currentProcess = processes_arrived.front();
        processes_arrived.pop();
        while(counter){
            currentProcess->remainingTime--;
            currentProcess->state=RUNNING;
            counter--;
            renderTimeline(processes);
            i++;
            for(auto &it_process:processes){
                if(it_process.state!= FINISHED && it_process.arrivalTime<=i){
                        if(it_process.state == NOT_ARRIVED || it_process.state == RUNNING )
                            it_process.state = WAITING;
                        if( processes_inserted.find(it_process.name) == processes_inserted.end()) {
                   
                            processes_inserted.insert(it_process.name);
                            processes_arrived.push(&it_process);
                        }
                }
            }
            
            
            
            if (currentProcess->remainingTime == 0) {
                    currentProcess->state = FINISHED;
                    finishTime[currentProcess->name] = i;
                    TAT[currentProcess->name] = finishTime[currentProcess->name] - currentProcess->arrivalTime;
                    normTurn[currentProcess->name] = (float)(TAT[currentProcess->name]) / currentProcess->serviceTime_priority;
                    break;
            }
            

               
        }
        if(currentProcess->remainingTime!=0){
            processes_arrived.push(currentProcess);
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

                break; // make sure that only one process run (prevent overlapping/non-preemptive) 
            }
        }

        // Checking for finishing
        for(auto& it_process: process){
            if(it_process.state!= FINISHED && it_process.remainingTime==0){
                it_process.state = FINISHED;
                finishTime[it_process.name] = i+1;
                TAT[it_process.name] = finishTime[it_process.name] - it_process.arrivalTime;
                normTurn[it_process.name] = (float) (TAT[it_process.name]) / it_process.serviceTime_priority;
            }
        }
        renderTimeline(process);
    }
}

void SPN(vector<Process> &process, Arguments args) {
    Process* running_process = nullptr;
    
    for (int i = 0; i < args.lastInstance; i++) {
        if (running_process && running_process->remainingTime == 0) {
            running_process->state = FINISHED;
            finishTime[running_process->name] = i;
            TAT[running_process->name] = finishTime[running_process->name] - running_process->arrivalTime;
            normTurn[running_process->name] = (float)TAT[running_process->name] / running_process->serviceTime_priority;
            running_process = nullptr;
        }

        for (auto &it_process : process) {
            if (it_process.arrivalTime == i) {
                it_process.state = WAITING;
            }
        }

        if (running_process == nullptr) {
            int shortest_time = INT_MAX;
            Process* shortest_process = nullptr;

            for (auto &it_process : process) {
                if (it_process.state == WAITING && 
                    it_process.serviceTime_priority < shortest_time) {
                    shortest_time = it_process.serviceTime_priority;
                    shortest_process = &it_process;
                }
            }

            if (shortest_process) {
                shortest_process->state = RUNNING;
                running_process = shortest_process;
            }
        }

        if (running_process) {
            running_process->remainingTime--;
        }

        renderTimeline(process);
    }

    if (running_process && running_process->remainingTime == 0) {
        running_process->state = FINISHED;
        finishTime[running_process->name] = args.lastInstance;
        TAT[running_process->name] = finishTime[running_process->name] - running_process->arrivalTime;
        normTurn[running_process->name] = (float)TAT[running_process->name] / running_process->serviceTime_priority;
    }
}

void aging(vector<Process> &process, Arguments args, int quantum) {
    vector<Process*> waiting_processes;
    unordered_map<char,int> priority;

    for (auto& p : process)
        priority[p.name] = p.serviceTime_priority;

    for (int i = 0; i < args.lastInstance; i++) {
        // Add processes that have arrived to the waiting list
        for (auto& p : process) {
            if (p.arrivalTime <= i) {
                if (p.state == NOT_ARRIVED) {
                    p.state = WAITING;
                }
                if (p.state == WAITING) {
                    if (!waiting_processes.empty())
                        p.serviceTime_priority++;

                    int flag = 0;
                    for (auto& waiting : waiting_processes) {
                        if (waiting->name == p.name) {
                            flag = 1;
                        }
                    }
                    if (flag == 0) {
                        waiting_processes.push_back(&p);
                    }
                }
                if (p.state == FINISHED) {
                    waiting_processes.push_back(&p);
                    p.state = WAITING;
                }
            }
        }  
        
        // If there are processes to run, select the one with the highest priority 
        if (!waiting_processes.empty()) {
            // Sort processes by priority
            sort(waiting_processes.begin(), waiting_processes.end(),
                [](Process* a, Process* b) { return a->serviceTime_priority > b->serviceTime_priority; });

            // Select the process with the highest priority
            Process* selected_process = waiting_processes.front();
            
            int count = quantum;

            while (count-- && i < args.lastInstance){
                selected_process->state = RUNNING;
                renderTimeline(process);
                i++;
            }
            i--;

            // After running, the process will be in waiting state again for the next time slice
            selected_process->state = FINISHED;
            selected_process->serviceTime_priority = priority[selected_process->name];

            // Remove the process from the waiting list to prevent it from being scheduled again
            waiting_processes.erase(remove(waiting_processes.begin(), waiting_processes.end(), selected_process), waiting_processes.end());
            
        } else {
            // If no process is waiting, just add a blank space to the timeline
            Process* selected_process;
            selected_process->state = NOT_ARRIVED;  
            renderTimeline(process);
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
                printTrace(algorithm,args.lastInstance,process_timeline);
            if(args.trace_stats=="stats")
                printStats(algorithm,process_copy,finishTime,TAT,normTurn,args.numberOfProcess);
            clearAllTemps(process);
            break;
        case 2:
            RoundRobin(process_copy,args,algorithm.second);
            if(args.trace_stats=="trace")
                printTrace(algorithm,args.lastInstance,process_timeline);
            if(args.trace_stats=="stats")
                printStats(algorithm,process_copy,finishTime,TAT,normTurn,args.numberOfProcess);
            clearAllTemps(process);
            break;
        case 3:
            SPN(process_copy,args);
            if(args.trace_stats=="trace")
                printTrace(algorithm,args.lastInstance,process_timeline);
            if(args.trace_stats=="stats")
                printStats(algorithm,process_copy,finishTime,TAT,normTurn,args.numberOfProcess);
            clearAllTemps(process);
            break;
        case 5:
            HighestResponseRatioNext(process_copy,args);
            if(args.trace_stats=="trace")
                printTrace(algorithm,args.lastInstance,process_timeline);
            if(args.trace_stats=="stats")
                printStats(algorithm,process_copy,finishTime,TAT,normTurn,args.numberOfProcess);
            clearAllTemps(process);
            break;
        case 6:
            FB(process_copy,args,false);
            if(args.trace_stats=="trace")
                printTrace(algorithm,args.lastInstance,process_timeline);
            if(args.trace_stats=="stats")
                printStats(algorithm,process_copy,finishTime,TAT,normTurn,args.numberOfProcess);
            clearAllTemps(process);
            break;
        case 7:
            FB(process_copy,args,true);
            if(args.trace_stats=="trace")
                printTrace(algorithm,args.lastInstance,process_timeline);
            if(args.trace_stats=="stats")
                printStats(algorithm,process_copy,finishTime,TAT,normTurn,args.numberOfProcess);
            clearAllTemps(process);
            break;
        case 8:
            aging(process_copy,args,algorithm.second);
            if(args.trace_stats=="trace")
                printTrace(algorithm,args.lastInstance,process_timeline);
            if(args.trace_stats=="stats")
                printStats(algorithm,process_copy,finishTime,TAT,normTurn,args.numberOfProcess);
            clearAllTemps(process);
            break;
        
        default:
            break;
        }
    }
    
    return 0;
}

