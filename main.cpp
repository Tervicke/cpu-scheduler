#include <iostream> #include <queue>
#include<vector>
#include<bits/stdc++.h>
#include<queue>
#include <json.hpp>

class Process
{
public:
    int id{};
    int arrivalTime;
    int burstTime;
    int completionTime{};
    int turnaroundTime{};
    int waitingTime{};
    int startTime{};
    Process(const int arrivalTime , const int burstTime)
    {
        this->arrivalTime = arrivalTime;
        this->burstTime = burstTime;
    }

    Process();

    void print() const
    {
        std::cout <<"p"<< id << " " << arrivalTime << " " << burstTime << " " << completionTime <<  " " << turnaroundTime << " " << waitingTime << " \n";
    }
};
class ProcessContainer
{
    int nextId = 1; //start the pid from 1
public:
    std::vector<Process>  processes;
    void addProcess(Process newProcess)
    {
        newProcess.id = nextId++;
        processes.push_back(newProcess);
    }
};

void run_srjf(const ProcessContainer& container)
{
    auto sorted = container.processes;
    std::ranges::sort(sorted, [](const auto& a , const auto& b)
    {
        //if the arrival times are equal , proccess it according to the pid
        if (a.arrivalTime == b.arrivalTime)
        {
            return a.id < b.id;
        }
        return a.arrivalTime < b.arrivalTime;
    });

    //lambda function to set priority in the ReadyQueue
    auto cmp = [](const Process& p1, const Process& p2) -> bool
    {
        if (p1.burstTime == p2.burstTime)
        {
            return p1.arrivalTime > p2.arrivalTime;
        }
        return p1.burstTime > p2.burstTime;
    };
    int lastProcess = 0; //this points the last proccess in the sorted queue
    std::priority_queue<Process, std::vector<Process>, decltype(cmp)> ReadyQueue(cmp);

    int processesDone = 0;
    int currentTime = 0;
    bool processRunning = false;
    Process dummy(-1,-1);
    Process& currentProcess = dummy;
    std::map<int,int> originalBurstTime;
    while (processesDone < sorted.size())
    {
        while (lastProcess < sorted.size() && sorted[lastProcess].arrivalTime <= currentTime)
        {
            ReadyQueue.push(sorted[lastProcess]);
            originalBurstTime[sorted[lastProcess].id] = sorted[lastProcess].burstTime;
            lastProcess++;
        }
        if (!processRunning) //process not running
        {
            if (!ReadyQueue.empty())
            {
                currentProcess = ReadyQueue.top();
                ReadyQueue.pop();
                processRunning = true;
            }
        }else //process running
        {
            if (ReadyQueue.top().burstTime < currentProcess.burstTime) //context switch only if the burst time is strictly less than the current remaining burst time
            {
               ReadyQueue.push(currentProcess);
                currentProcess = ReadyQueue.top();
                ReadyQueue.pop();
            }
        }
        currentTime++;
        if (processRunning)
        {
            currentProcess.burstTime--;
            if (currentProcess.burstTime == 0) //process is abo
            {
                currentProcess.completionTime = currentTime;
                currentProcess.turnaroundTime = currentTime - currentProcess.arrivalTime;
                currentProcess.waitingTime = currentProcess.turnaroundTime - originalBurstTime[currentProcess.id];
                processesDone++;
                currentProcess.print();
                currentProcess = dummy;
                processRunning = false;
            }
        }
    }
}

void run_rr(const ProcessContainer& container , const int timeQuantum) //Round Robin
{
    auto sorted = container.processes;
    std::ranges::sort(sorted, [](const auto& a , const auto& b)
    {
        //if the arrival times are equal , proccess it according to the pid
        if (a.arrivalTime == b.arrivalTime)
        {
            return a.id < b.id;
        }
        return a.arrivalTime < b.arrivalTime;
    });
    std::deque<Process> ReadyQueue;
    int lastprocess = 0;
    int currentTime = 0;
    int processDone = 0;
    std::map<int,int> originalBurstTime;
    while (processDone < sorted.size())
    {
        while (lastprocess < sorted.size() && sorted[lastprocess].arrivalTime <= currentTime)
        {
            ReadyQueue.push_back(sorted[lastprocess]);
            originalBurstTime[sorted[lastprocess].id] = sorted[lastprocess].burstTime;
            lastprocess++;
        }
        if (!ReadyQueue.empty())
        {
            auto& currentProcess = ReadyQueue.front();
            ReadyQueue.pop_front();
            if (currentProcess.burstTime > timeQuantum)
            {
                currentProcess.burstTime -= timeQuantum;
                ReadyQueue.push_back(currentProcess);
                currentTime += timeQuantum;
            }else
            {
                currentProcess.completionTime = currentTime + currentProcess.burstTime;
                currentProcess.turnaroundTime = currentProcess.completionTime - currentProcess.arrivalTime;
                currentProcess.waitingTime = currentProcess.turnaroundTime - originalBurstTime[currentProcess.id];
                currentTime += currentProcess.burstTime;
                currentProcess.print();
                processDone += 1;
            }
        }else
        {
            currentTime = sorted[lastprocess].arrivalTime;
        }
    }
}
void run_sjf(const ProcessContainer& container , nlohmann::json& j)
{

    j["algorithm"] = "Shortest Job First";
    auto sorted = container.processes;
    //sorted sorts the proccess in the ordero of their arrival time
    std::ranges::sort(sorted, [](const auto& a , const auto& b)
    {
        //if the arrival times are equal , proccess it according to the pid
        if (a.arrivalTime == b.arrivalTime)
        {
            return a.id < b.id;
        }
        return a.arrivalTime < b.arrivalTime;
    });

    //lambda function to set priority in the ReadyQueue
    auto cmp = [](const Process& p1, const Process& p2) -> bool
    {
        if (p1.burstTime == p2.burstTime)
        {
            return p1.arrivalTime > p2.arrivalTime;
        }
        return p1.burstTime > p2.burstTime;
    };
    int last_process = 0; //this points the last proccess in the sorted queue
    int currentTime = 0;
    std::priority_queue<Process, std::vector<Process>, decltype(cmp)> ReadyQueue(cmp);
    while (last_process < sorted.size() || !ReadyQueue.empty())
    {
        while (last_process < sorted.size() && sorted[last_process].arrivalTime <= currentTime)
        {
            ReadyQueue.push(sorted[last_process]);
            last_process++;
        }

        // move to the next arrival time in the list
        if (ReadyQueue.empty())
        {
            currentTime = sorted[last_process].arrivalTime;
            continue;
        }

        auto p = ReadyQueue.top();
        ReadyQueue.pop();

        p.completionTime = currentTime + p.burstTime;
        p.turnaroundTime = p.completionTime - p.arrivalTime;
        p.waitingTime = currentTime - p.arrivalTime;
        p.startTime = currentTime;
        currentTime += p.burstTime;
        p.print();

        j["plot"].push_back({
            {"id" , p.id},
            {"start" , p.startTime},
            {"end" , p.completionTime}
        });
        j["processes"].push_back({
                {"id" , p.id},
                {"AT" , p.arrivalTime},
                {"CT" , p.completionTime},
                {"BT" , p.burstTime},
                {"TAT",p.turnaroundTime},
                   {"WT" , p.waitingTime},
            });
        std::ofstream out("gantt.json");
        out << j.dump(4);
    }
}

void run_fcfs(const ProcessContainer& container , nlohmann::json& j)
{
    j["algorithm"] = "First Come First Serve";
    std::vector<Process> sorted = container.processes;
    int currentTime = 0;
    //custom sorting function sorts according to the arrival time first come first serve
    std::ranges::sort(sorted, [](const auto& a , const auto& b)
    {
        //if the arrival times are equal , proccess it according to the pid
        if (a.arrivalTime == b.arrivalTime)
        {
            return a.id < b.id;
        }
        return a.arrivalTime < b.arrivalTime;
    });
    for (auto& p  : sorted)
    {
        //adjust the IDLE CPU time
        if (currentTime < p.arrivalTime)
            currentTime = p.arrivalTime;

        //completion time = currentTime + p.burstTime
        p.completionTime = currentTime + p.burstTime;
        p.startTime = currentTime;
        currentTime += p.burstTime;
        p.turnaroundTime = p.completionTime - p.arrivalTime;
        p.waitingTime = p.turnaroundTime - p.burstTime;
        //update the current time
        j["plot"].push_back({
            {"id" , p.id},
            {"start" , p.startTime},
            {"end" , p.completionTime}
        });
        j["processes"].push_back({
                {"id" , p.id},
                {"AT" , p.arrivalTime},
                {"CT" , p.completionTime},
                {"BT" , p.burstTime},
                {"TAT",p.turnaroundTime},
                   {"WT" , p.waitingTime},
            });
    }
    std::ofstream out("gantt.json");
    out << j.dump(4);
    for (auto x : sorted)
    {
        x.print();
    }
};

int main()
{
    ProcessContainer c;
    c.addProcess(Process(0,9));;
    c.addProcess(Process(1,4));;
    c.addProcess(Process(2,9));;
    c.addProcess(Process(2,5));;
    nlohmann::json j;
    run_fcfs(c , j);
    system("bash -c 'source ../venv/bin/activate && python3 ../script.py &'");
    return 0;
}
