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
    Process(const int arrivalTime , const int burstTime)
    {
        this->arrivalTime = arrivalTime;
        this->burstTime = burstTime;
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
        std::cout <<"p"<< x.id << " " << x.arrivalTime << " " << x.burstTime << " " << x.completionTime <<  " " << x.turnaroundTime << " " << x.waitingTime << " \n";
    }
};

int main()
{
    ProcessContainer c;
    c.addProcess( Process(0 , 5) );
    c.addProcess( Process(1 , 3) );
    c.addProcess( Process(2 , 8) );
    run_fcfs(c);
    return 0;
}
