#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <algorithm>

// Definition of the Process structure representing a process in the simulation
struct Process {
    int processID;  
    int arrivalTime;
    int priority;
    int CPUBU; // CPU Burst Units
    int remainingBT; // remaining Burst Time
    int ET; //Exit Time
    bool over;

    // Constructor to initialize a process with given parameters
    Process(int id, int arrivalT, int priorities, int BT)
        : processID(id), arrivalTime(arrivalT), priority(priorities), CPUBU(BT),
          remainingBT(BT), ET(0), over(false) {}
};

// Function to calculate and print scheduling statistics
void calculateStatistics(std::vector<Process>& processes, const std::string& algorithm, const std::string& outputFile) {
    int totalBurstTime;
    int totalWaitingTime;
    int totalTurnaroundTime;
    int totalResponseTime;
    int totalElapsed;

    // Loop through each process to calculate various statistics
    for (const auto& process : processes) {
        totalBurstTime += process.CPUBU;
        totalElapsed = std::max(totalElapsed, process.ET);
        totalTurnaroundTime += process.ET - process.arrivalTime;
        totalWaitingTime += process.ET - process.arrivalTime - process.CPUBU;
        totalResponseTime += process.ET - process.arrivalTime - process.CPUBU;
    }

    // Calculate various performance metrics
    float throughput = static_cast<float>(totalBurstTime) / processes.size();
    float cpuUtilization = static_cast<float>(totalBurstTime) / totalElapsed;
    float avgWaitingTime = static_cast<float>(totalWaitingTime) / processes.size();
    float avgTurnaroundTime = static_cast<float>(totalTurnaroundTime) / processes.size();
    float avgResponseTime = static_cast<float>(totalResponseTime) / processes.size();

    // Check if totalElapsed is not zero to avoid division by zero
    if (totalElapsed != 0) {
        cpuUtilization = static_cast<float>(totalBurstTime) / totalElapsed;
    } else {
        std::cerr << "Error: Total elapsed time is zero. Cannot calculate CPU utilization.\n";
    }

    // Write statistics to an output file
    std::ofstream output(outputFile);
    output << algorithm << " Scheduling Statistics:\n";
    output << "Number of processes: " << processes.size() << "\n";
    output << "Total elapsed time: " << totalElapsed << " CPU burst units\n";
    output << "Throughput: " << throughput << " processes executed in one unit of CPU burst time\n";
    output << "CPU utilization: " << cpuUtilization * 100 << "%\n";
    if (totalElapsed != 0) {
        output << cpuUtilization * 100 << "%\n";
    } else {
        output << "N/A (Total elapsed time is zero)\n";
    }
    output << "Average waiting time: " << avgWaitingTime << " CPU burst times\n";
    output << "Average turnaround time: " << avgTurnaroundTime << " CPU burst times\n";
    output << "Average response time: " << avgResponseTime << " CPU burst times\n";
    output << "------------------------------------------\n";
    output.close();
}

// Function to simulate First-Come, First-Served (FIFO) scheduling
void simulateFIFO(std::vector<Process>& processes) {
    int currentTime;

    // Iterate through each process, update exit time, and accumulate total elapsed time
    for (auto& process : processes) {
        process.ET = currentTime + process.CPUBU;
        currentTime = process.ET;
    }

    // Call calculateStatistics to print statistics for FIFO scheduling
    calculateStatistics(processes, "FIFO" , "Output-FIFOScheduling.txt" );
}

// Function to simulate Shortest Job First (SJF) scheduling
void simulateSJF(std::vector<Process>& processes) {
    // Sort processes based on CPU burst units (Shortest Job First)
    std::sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.CPUBU < b.CPUBU;
    });

    int currentTime;

    // Iterate through each process, update exit time, and accumulate total elapsed time
    for (auto& process : processes) {
        process.ET = currentTime + process.CPUBU;
        currentTime = process.ET;
    }

    // Call calculateStatistics to print statistics for SJF scheduling
    calculateStatistics(processes, "SJF" , "Output-SJFScheduling.txt");
}

// Function to simulate Priority-based scheduling
void simulatePriority(std::vector<Process>& processes) {
    // Sort processes based on priority
    std::sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.priority < b.priority;
    });

    int currentTime;

    // Iterate through each process, update exit time, and accumulate total elapsed time
    for (auto& process : processes) {
        process.ET = currentTime + process.CPUBU;
        currentTime = process.ET;
    }

    // Call calculateStatistics to print statistics for Priority scheduling
    calculateStatistics(processes, "Priority" , "Output-PriorityScheduling.txt");
}

// Main function
int main() {
    // Open the input file
    std::ifstream infile("Datafile1-txt.txt");

    // Check if the file is successfully opened
    if (!infile) {
        std::cerr << "Error opening the file." << std::endl;
        return 1;
    }

    // Vector to store the processes read from the file
    std::vector<Process> processes;

    // Read process information from the file and populate the processes vector
    for (int i = 0; i < 500; ++i) {
        int id, arrival, prio, burst;
        infile >> id >> arrival >> prio >> burst;
        Process newProcess(id, arrival, prio, burst);
        processes.push_back(newProcess);
    }

    // Close the input file
    infile.close();

    // Simulate FIFO, SJF, and Priority scheduling and print statistics
    simulateFIFO(processes);
    simulateSJF(processes);
    simulatePriority(processes);

    // Return 0 to indicate successful execution
    return 0;
}
