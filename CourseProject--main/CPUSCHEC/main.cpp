#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <algorithm>

// Process structure representing a process in the simulation
struct Process
{
    int processID;   // Unique identifier for the process
    int arrivalTime; // Time at which the process arrives
    int priority;    // Priority of the process (lower value = higher priority)
    int CPUBU;       // Total CPU Burst Units required
    int remainingBT; // Remaining Burst Time
    int ET;          // Exit Time (when the process finishes execution)
    int startTime;
    // bool over;           // Indicates if the process has completed execution

    // Constructor to initialize process parameters
    Process(int id, int arrivalT, int priorities, int burst)
        : processID(id),
          arrivalTime(arrivalT),
          priority(priorities),
          CPUBU(burst),
          remainingBT(burst),
          ET(0),
          startTime(-1)
    // over(false)
    {
    }
};

// Function to calculate and print scheduling statistics
void calculateStatistics(std::vector<Process> &processes, const std::string &algorithm, const std::string &outputFile)
{
    int totalBT = 0;      // Sum of all CPU burst times
    int totalWT = 0;      // Sum of all waiting times
    int totalTT = 0;      // Sum of all turnaround times
    int totalRT = 0;      // Sum of all response times
    int totalElapsed = 0; // Total elapsed time during scheduling

    // Iterate through all processes to compute statistics
    for (const auto &process : processes)
    {
        totalBT += process.CPUBU;
        totalElapsed = std::max(totalElapsed, process.ET);             // Update total elapsed time
        totalTT += (process.ET - process.arrivalTime);                 // Turnaround time = Exit time - Arrival time
        totalWT += (process.ET - process.arrivalTime - process.CPUBU); // Waiting time = Turnaround time - Burst time
        totalRT += (process.startTime - process.arrivalTime);          // Response time (same as waiting time for non-preemptive)
    }

    // Calculate metrics based on gathered statistics
    float throughput = static_cast<float>(totalElapsed) / processes.size(); // Throughput = Total burst time / Number of processes
    float cpuUtilization = static_cast<float>(totalBT) / totalElapsed;      // CPU Utilization
    float avgWT = static_cast<float>(totalWT) / processes.size();           // Average waiting time
    float avgTT = static_cast<float>(totalTT) / processes.size();           // Average turnaround time
    float avgRT = static_cast<float>(totalRT) / processes.size();           // Average response time

    // Open the output file to save statistics
    std::ofstream output(outputFile);
    if (output.is_open())
    {
        output << algorithm << " Scheduling Statistics:\n";
        output << "Number of processes: " << processes.size() << "\n";
        output << "Total elapsed time: " << totalElapsed << " CPU burst units\n";
        output << "Throughput: " << throughput << " processes executed in one unit of CPU burst time\n";
        output << "CPU utilization: " << (cpuUtilization * 100) << "%\n";
        output << "Average waiting time: " << avgWT << " CPU burst times\n";
        output << "Average turnaround time: " << avgTT << " CPU burst times\n";
        output << "Average response time: " << avgRT << " CPU burst times\n";
        output << "------------------------------------------\n";
    }
}

// Simulate First-Come, First-Served (FIFO) scheduling
void simulateFIFO(std::vector<Process> &processes)
{
    int currentTime = 0; // Tracks the current time in the simulation

    // Process each task in the order of their arrival
    for (auto &process : processes)
    {
        if (currentTime < process.arrivalTime)
        {
            currentTime = process.arrivalTime;
        }

        process.startTime = currentTime;

        process.ET = currentTime + process.CPUBU; // Calculate exit time for the process
        currentTime = process.ET;                 // Update the current time
    }

    // Calculate and save statistics for FIFO scheduling
    calculateStatistics(processes, "FIFO", "Output-FIFOScheduling.txt");
}

// Simulate Shortest Job First (SJF) scheduling
void simulateSJF(std::vector<Process> &processes)
{
    // Sort processes by their CPU Burst Time (Shortest Job First)
    std::sort(processes.begin(), processes.end(), [](const Process &a, const Process &b)
              { return a.CPUBU < b.CPUBU; });

    int currentTime = 0; // Tracks the current time in the simulation

    // Process each task based on sorted order
    for (auto &process : processes)
    {
        process.ET = currentTime + process.CPUBU; // Calculate exit time
        currentTime = process.ET;                 // Update the current time
    }

    // Calculate and save statistics for SJF scheduling
    calculateStatistics(processes, "SJF", "Output-SJFScheduling.txt");
}

// Simulate Priority-based scheduling
void simulatePriority(std::vector<Process> &processes)
{
    // Sort processes by their priority (lower value = higher priority)
    std::sort(processes.begin(), processes.end(), [](const Process &a, const Process &b)
              {
        if (a.arrivalTime == b.arrivalTime)
            return a.priority < b.priority;
        return a.arrivalTime < b.arrivalTime; });

    int currentTime = 0; // Tracks the current time in the simulation

    // Process each task based on sorted order
    int process_completed = 0;

    while (process_completed < processes.size())
    {
        Process *process = nullptr;

        for (Process &p : processes)
        {
            if (p.arrivalTime <= currentTime && p.remainingBT > 0)
            {
                if (process == nullptr || process->priority < p.priority)
                {
                    process = &p;
                    break;
                }
            }
        }

        if (process)
        {
            if (process->startTime == -1)
            {
                process->startTime = currentTime;
            }

            process->remainingBT -= 1;
            currentTime += 1;

            if(process->remainingBT == 0) {
                process_completed += 1;
                process->ET = currentTime;
            }
        }
        else 
        {
            currentTime++;
        }
    }

    // Calculate and save statistics for Priority scheduling
    calculateStatistics(processes, "Priority", "Output-PriorityScheduling.txt");
}

// Main function to execute the scheduling simulation
int main()
{
    // Open the input file containing process data
    std::ifstream infile("Datafile1-txt.txt");
    if (!infile)
    {
        std::cerr << "Error opening the file." << std::endl;
        return 1;
    }

    std::vector<Process> processes; // Vector to store all processes

    std::string header, header2, header3;
    getline(infile, header);

    // Read process data from the file
    for (int i = 0; i < 500; ++i)
    {
        int arrivalT, priorities, burst;
        infile >> arrivalT >> burst >> priorities;
        processes.emplace_back(i, arrivalT, priorities, burst); // Create and store new process
    }
    infile.close(); // Close the input file

    // Simulate all three scheduling algorithms
    simulateFIFO(processes);     // First-Come, First-Served
    simulateSJF(processes);      // Shortest Job First
    simulatePriority(processes); // Priority-based scheduling

    return 0; // Indicate successful execution
}
