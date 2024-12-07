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
#include <semaphore.h>

// Buffer Configuration
const int BUFFER_SIZE = 5;           // Size of the buffer
const int PRODUCER_COUNT = 2;        // Number of producer threads
const int CONSUMER_COUNT = 2;        // Number of consumer threads

// Semaphores for buffer management
extern sem_t empty;  // Tracks available empty slots in the buffer
extern sem_t full;   // Tracks available full slots in the buffer

// Mutex for thread-safe buffer operations
extern pthread_mutex_t mutex;

// Function to insert an item into the buffer
int insert_item(int item);

// Function to remove an item from the buffer
int remove_item(int *item);

// Producer thread function
void *producer(void *arg);

// Consumer thread function
void *consumer(void *arg);

#endif // BUFFER_H