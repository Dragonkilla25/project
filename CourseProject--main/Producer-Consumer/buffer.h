#ifndef BUFFER_H
#define BUFFER_H
#include <iostream>
#include <fstream>
#include <chrono>
#include <queue>
#include <atomic>
#include <mutex>
#include <pthread.h>
#include <unistd.h>

const int BUFFER_SIZE = 5;  // Size of the buffer
const int PRODUCER_COUNT = 2; // Num of producer threads
const int CONSUMER_COUNT = 2; // Num of consumer threads

extern sem_t empty, full;  // Semaphores to track empty and full slots
pthread_mutex_t mutex; // Mutex to protec buffer access

//std::queue<int> buffer; // Shared buffer

// A Function that insert the item inside the buffer
int insert_item(int item);

// A Function that removes the item from the buffer
int remove_item(int *item);

// Producer function
void *producer(void *arg);

// Consumer function
void *consumer(void *arg);

#endif // BUFFER_H