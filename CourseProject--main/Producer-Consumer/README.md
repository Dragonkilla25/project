# CS471 Project - Producer-Consumer Problem 2

## Authors 

Aamr Ibrahim, Justin Cantoria

## Description

The program is a development of the producer-consumer problem using the Pthreads and is written in C++. It takes the input file that
lists a number of arguments and outputs the arguments along with the turnaround time which will be needed to complete these execution.

## Input files

Three input files are included, which will include the same parameters except the wait time. These are listed below:

input-2sec-wait.txt
input-4sec-wait.txt
input-6sec-wait.txt 

## Compiling the program

Starting from the source code, the program can be compiled using the following command below:

g++ -std=c++11 -pthread main.cpp -o program

## Running the program 

The program can run in Terminal using the following command below:

./program [input file]

For example, using the provided input files, the program can run as follows:

./program input-2sec-wait.txt
./program input-4sec-wait.txt
./program input-6sec-wait.txt

## The output files have the following structure: "output-[ wait time ]sec-wait.txt", such as:

output-2sec-wait.txt
output-4sec-wait.txt
output-6sec-wait.txt

## Result's summary and explanation 

As seen in the generated output files, the turnaround time can vary due to factors such as the number of producers, consumers and
wait times. 

First, the overall workload and turnaround time can be impacted by adding more producers or consumers. In effect, when the number of
producers increased, more items are being produced concurrently, which can potentially increase the overall workload and affect the 
turnaround time. Similarly, as the number of consumers increases, more items are being consumed concurrently, which may also impact
the turnaround times. Furthermore, we see that the set wait times directly and significantly affect the turnaround times. Choosing
an appropriate wait time for all processes to complete cleanly while optimizing the turnaround time is then crucial to making an
effective program.  
