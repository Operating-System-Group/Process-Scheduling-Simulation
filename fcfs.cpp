#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <sstream>

using namespace std;

// Structure to represent a process
struct Process {
    int pid, arrival_time, burst_time, priority;
    int waiting_time, turnaround_time, completion_time;
};

vector<Process> readProcesses(const string& filename) {
    vector<Process> processes;
    ifstream file(filename); 
    if (!file) {
        cerr << "Error opening file!" << endl;
        exit(1);
    }

    string line;
    getline(file, line); // Skip the header

    while (getline(file, line)) {
        istringstream iss(line);
        Process p;
        if (iss >> p.pid >> p.arrival_time >> p.burst_time >> p.priority) {
            processes.push_back(p);
        }
    }

    file.close();
    return processes;
}

// Function to implement First-Come, First-Served Scheduling
void FCFS(vector<Process>& processes) {
    int time = 0;
    for (auto &p : processes) {
        if (time < p.arrival_time)
            time = p.arrival_time; 
        p.completion_time = time + p.burst_time;
        p.turnaround_time = p.completion_time - p.arrival_time;
        p.waiting_time = p.turnaround_time - p.burst_time;
        time = p.completion_time;
    }
}
// Function to display process details
void displayResults(const vector<Process>& processes) {
    cout << "\nFCFS Scheduling Results:\n";
    cout << "----------------------------------------------------\n";
    cout << "PID\tArrival\tBurst\tCompletion\tWaiting\tTurnaround\n";
    cout << "----------------------------------------------------\n";
    
    double total_wt = 0, total_tat = 0;
    for (const auto &p : processes) {
        cout << p.pid << "\t" << p.arrival_time << "\t" << p.burst_time << "\t"
             << p.completion_time << "\t\t" << p.waiting_time << "\t" << p.turnaround_time << "\n";
        total_wt += p.waiting_time;
        total_tat += p.turnaround_time;
    }

    cout << "----------------------------------------------------\n";
    cout << "Average Waiting Time: " << total_wt / processes.size() << endl;
    cout << "Average Turnaround Time: " << total_tat / processes.size() << endl;
}

// Memory Block Structure
struct MemoryBlock {
    int id, size;
    bool allocated;
};

// Function to implement First-Fit memory allocation
void firstFit(vector<MemoryBlock>& memory, vector<int>& process_sizes) {
    cout << "\nMemory Allocation using First-Fit:\n";
    for (int i = 0; i < process_sizes.size(); i++) {
        bool allocated = false;
        for (auto &block : memory) {
            if (!block.allocated && block.size >= process_sizes[i]) {
                cout << "Process " << i + 1 << " allocated to Block " << block.id << endl;
                block.allocated = true;
                allocated = true;
                break;
            }
        }
        if (!allocated)
            cout << "Process " << i + 1 << " cannot be allocated!\n";
    }
}
int main() {
    // Read processes from file
    vector<Process> processes = readProcesses("processes.txt");
    // Sort by arrival time (FCFS requirement)
    sort(processes.begin(), processes.end(), [](const Process &a, const Process &b) {
        return a.arrival_time < b.arrival_time;
    });
    // Run FCFS scheduling
    FCFS(processes);
    displayResults(processes);
    // Memory management using First-Fit
    vector<MemoryBlock> memory = {{1, 10, false}, {2, 5, false}, {3, 8, false}};
    vector<int> process_sizes = {4, 6, 2}; // Example process sizes
    firstFit(memory, process_sizes);

    return 0;
}
