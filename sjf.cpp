#include <iostream>
#include <vector>
#include <algorithm>
#include<bits/stdc++.h>

using namespace std;

struct Process {
    int pid;
    int arrivalTime;
    int burstTime;
    int completionTime;  // Time when the process completes
    int turnaroundTime;  // Completion time - Arrival time
    int waitingTime;     // Turnaround time - Burst time
};

bool compareArrivalTime(const Process &a, const Process &b) {
    return a.arrivalTime < b.arrivalTime;
}

void SJF_Scheduling(vector<Process> &processes) {
    sort(processes.begin(), processes.end(), compareArrivalTime);

    int n = processes.size();
    int currentTime = 0;
    int completedProcesses = 0;

    while (completedProcesses < n) {
        int shortestIndex = -1;
        int shortestBurstTime = INT_MAX;

        for (int i = 0; i < n; ++i) {
            if (processes[i].arrivalTime <= currentTime && processes[i].burstTime > 0) {
                if (processes[i].burstTime < shortestBurstTime) {
                    shortestIndex = i;
                    shortestBurstTime = processes[i].burstTime;
                }
            }
        }

        if (shortestIndex == -1) {
            currentTime++;
        } else {
            Process &currentProcess = processes[shortestIndex];
            currentProcess.completionTime = currentTime + currentProcess.burstTime;
            currentProcess.turnaroundTime = currentProcess.completionTime - currentProcess.arrivalTime;
            currentProcess.waitingTime = currentProcess.turnaroundTime - currentProcess.burstTime;

            cout << "Executing Process " << currentProcess.pid << " at time " << currentTime << endl;
            currentTime += currentProcess.burstTime;
            currentProcess.burstTime = 0;
            completedProcesses++;
        }
    }
}

int main() {
    int n;
    cout << "Enter the number of processes: ";
    cin >> n;

    vector<Process> processes(n);

    for (int i = 0; i < n; ++i) {
        processes[i].pid = i + 1;
        cout << "Enter arrival time for Process " << processes[i].pid << ": ";
        cin >> processes[i].arrivalTime;
        cout << "Enter burst time for Process " << processes[i].pid << ": ";
        cin >> processes[i].burstTime;
    }

    SJF_Scheduling(processes);

    // Print turnaround time and waiting time for each process
    cout << "\nProcess\tTurnaround Time\tWaiting Time\n";
    for (const Process &p : processes) {
        cout << p.pid << "\t" << p.turnaroundTime << "\t\t" << p.waitingTime << endl;
    }

    return 0;
}
