#include <iostream>
#include<vector>
#include<bits/stdc++.h>
class Process
{
public:
    int id{};
    int arrivalTime;
    int burstTime;
    int completionTime{};
    int turnaroundTime{};
    int waitingTime{};
    int starttime{};
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

void run_sjf(const ProcessContainer& container)
{

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
        currentTime += p.burstTime;
        p.print();
    }
}

void run_fcfs(const ProcessContainer& container)
{
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
        currentTime += p.burstTime;
        p.turnaroundTime = p.completionTime - p.arrivalTime;
        p.waitingTime = p.turnaroundTime - p.burstTime;
        //update the current time
    }
    for (auto x : sorted)
    {
        x.print();
    }
};

int main()
{
    ProcessContainer c;
    c.addProcess( Process(0 , 5) );
    c.addProcess( Process(1 , 3) );
    c.addProcess( Process(2 , 8) );
    run_sjf(c);
    return 0;
}
