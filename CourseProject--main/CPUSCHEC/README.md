# CS471 Project - Problem 1

## Authors

William Mbandi, Augustine

## Description

This prohgram manages scheduling for 500 processes by simulating a CPU scheduler. To process the jobs, the user can select from the following scheduling algorithms:

1. **FIFO (First-In-First-Out)**  
2. **SJF (Shortest Job First) without Preemption**  
3. **Priority-based Scheduling without Preemption**

Key performance indicators, such as average waiting time, average turnaround time, CPU utilization, throughput, and average response time, are computed by each scheduling method and written to the appropriate files.

## Input File

The program reads an input file named `Datafile1-txt.txt`, which contains 513 processes. Each line in the file includes three columns:
- **Arrival Time**
- **CPU Burst Length**
- **Priority**

Only the first 500 processes are parsed and used in the simulation. The file must be formatted correctly for the program to function as intended.

## Compiling the Program

To compile the program, use the following command:

```bash
g++ -o main main.cpp









