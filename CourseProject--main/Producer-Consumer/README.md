# CS471 Project - Producer-Consumer Problem 2

## Authors

William Mbandi, Augustine

## Description

This program uses **Pthreads** in C++ to implement the traditional **Producer-Consumer Problem**. The method uses mutexes and semaphores to synchronize producer and consumer thread access to a shared circular buffer. While the consumers retrieve and process these objects, the producers create random items and store them in the buffer. 

The program simulates the producer-consumer relationship after reading input settings from a file, including the number of producers, consumers, and simulation length. It outputs the turnaround time to a file after execution.

## Input Files

The program reads configuration data from an input file containing:
1. Simulation duration (in seconds)
2. Number of producer threads
3. Number of consumer threads

Example input files provided:
- `input-2sec-wait.txt`
- `input-4sec-wait.txt`
- `input-6sec-wait.txt`

Each file contains the same parameters, except for the duration of the simulation.

## Compiling the Program

To compile the program, use the following command:

```bash
g++ -std=c++11 -pthread main.cpp -o program