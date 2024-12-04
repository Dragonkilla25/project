# CS471 Project - Problem 1 

## Authors 

William Mbandi, Augustine 

## Description 

This program will act as a CPU schedueler and handle scheduleing 500 simulated processes. the user will choose which 
scheduleing algorithm to handle the process: FIFO, SJF, Without Preemption, Priority with Preemption

## Input File 
The program will takes an input file titled "Datafile1-txt.txt" that contains 513 processes with three colums:
Arrival time, CPU burst length, and priority. only the first 500 process are parsed and read from the file.

## compiling the program 

g++ -o main main.cpp

## Running the program

./main

## Output

The output file have the following structure "output[scheduleing-Type].txt" such as:
Output-FIFOScheduling.txt
Output-PriorityScheduling.txt
Output-SJFScheduling.txt









