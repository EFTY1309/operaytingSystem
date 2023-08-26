#include <iostream>
#include <queue>

using namespace std;

struct Process {
    int pid;
    int arrivalTime;
    int burstTime;
};

void FCFS_Scheduling(queue<Process>& processes) {
    int currentTime = 0;
    cout<<endl;
    while (!processes.empty()) {
        Process currentProcess = processes.front();
        processes.pop();

        // If the process arrives after the current time, update the current time
        if (currentProcess.arrivalTime > currentTime) {
            currentTime = currentProcess.arrivalTime;
        }

        cout << "Executing Process " << currentProcess.pid << " at time " << currentTime << endl;

        currentTime += currentProcess.burstTime;
        int turnaroundTime=currentTime-currentProcess.arrivalTime;
        int waitingTime=turnaroundTime-currentProcess.burstTime;
        
        cout<<"Completion time of process "<<currentProcess.pid<<" at time "<< currentTime << endl;
        cout<<"Turnaround time of process "<<currentProcess.pid<<" is "<< turnaroundTime << endl;
        cout<<"Waiting time of process "<<currentProcess.pid<<" is "<< waitingTime << endl;
        cout<<endl;

    }
}

int main() {
    int n;
    cout << "Enter the number of processes: ";
    cin >> n;

    queue<Process> processes;

    for (int i = 0; i < n; ++i) {
        Process p;
        p.pid = i + 1;
        cout << "Enter arrival time for Process " << p.pid << ": ";
        cin >> p.arrivalTime;
        cout << "Enter burst time for Process " << p.pid << ": ";
        cin >> p.burstTime;
        processes.push(p);
    }

    FCFS_Scheduling(processes);

    return 0;
}
