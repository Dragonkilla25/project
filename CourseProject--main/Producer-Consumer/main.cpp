#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <atomic>
#include <mutex>
#include <queue>
#include <thread>
#include <pthread.h>
#include <semaphore.h>
#include "buffer.h"
#include <unistd.h>

using namespace std;


sem_t empty, full;  // Semaphores to track empty and full slots
pthread_mutex_t buffer_mutex;  // Mutex to protect buffer access

// Atomic flags to control the termination
std::atomic<bool> terminate_producers(false);
std::atomic<bool> terminate_consumers(false);


// Buffer structure
struct Buffer {
    int buffer[BUFFER_SIZE];    // Buffer Size
    int in = 0, out = 0;         // What goes in and out of the buffer
};

Buffer buffer; // Shared buffer

// A Function of inserting an item inside the buffer
int insert_item(int item) {
    
    if ((buffer.in + 1) % BUFFER_SIZE == buffer.out) {

        return -1;
    }

    buffer.buffer[buffer.in] = item;
    buffer.in = (buffer.in + 1) % BUFFER_SIZE;

    return 0;   // return 0 or success code if it is needed
}

// A Function of removing an item from the buffer
int remove_item(int *item) {

    if (buffer.in == buffer.out) {

        return -1;
    }

    *item = buffer.buffer[buffer.out];
    buffer.out = (buffer.out + 1) % BUFFER_SIZE;

    return 0; // return 0 or success code if it is needed
}

// Producer function
void *producer(void *arg) {
    while (!terminate_producers) {
        // Sleep for the random period of time
        usleep((rand() % 1000) * 1000); // usleep uses microseconds

        // Generates a random time
        int item = rand() % 100;

        // Acquires an empty semaphore
        sem_wait(&empty);

        // Acquires a mutex
        pthread_mutex_lock(&buffer_mutex);

        // Insert the item inside the buffer
        insert_item(item);

        // Release the mutex
        pthread_mutex_unlock(&buffer_mutex);

        // Release full semaphore
        sem_post(&full);
    }
    return NULL; // Add a return statement
    }

// Consumer function
void *consumer(void *arg) {
    while (!terminate_consumers) {
        // Sleep for the random period of time
        usleep((rand() % 1000) * 1000); // usleep uses microseconds

        // Acquires the full semaphore
        sem_wait(&full);

        // Acquires the mutex
        pthread_mutex_lock(&buffer_mutex);

        // Remove the item from the buffer
        int item;
        remove_item(&item);

        // Release mutex
        pthread_mutex_unlock(&buffer_mutex);

        // Release empty semaphore
        sem_post(&empty);

        // This process the item (e.g., print or use it)
        std::cout << "Consumed item: " << item << std::endl;
    
    }
    return NULL;
}





// Define a struct to hold thread execution times
    struct ThreadExecutionTime {
    std::chrono::steady_clock::time_point start;
    std::chrono::steady_clock::time_point end;
};

    // Vector to store execution times for each thread
    std::vector<ThreadExecutionTime> producer_execution_times(PRODUCER_COUNT);
    std::vector<ThreadExecutionTime> consumer_execution_times(CONSUMER_COUNT);




int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input-file>" << std::endl;
        return 1;
    }

    std::ifstream input_file(argv[1]);
    if (!input_file) {
        std::cerr << "Error opening input file." << std::endl;
        return 1;
    }

    int wait_time, producers, consumers;
    input_file >> wait_time >> producers >> consumers;
    input_file.close();

    // Initializes the semaphores and mutex
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&buffer_mutex, NULL);

    pthread_mutex_destroy(&buffer_mutex);

    
    


    // Before creating threads, capture the start time
    auto program_start = std::chrono::steady_clock::now();

    // Creates the producer and consumer threads
    pthread_t producer_threads[PRODUCER_COUNT];
    pthread_t consumer_threads[CONSUMER_COUNT];

    for (int i = 0; i < PRODUCER_COUNT; ++i)
        pthread_create(&producer_threads[i], NULL, producer, NULL);

    for (int i = 0; i < CONSUMER_COUNT; ++i)
        pthread_create(&consumer_threads[i], NULL, consumer, NULL);

    // Sleep for a period of time
    std::this_thread::sleep_for(std::chrono::seconds(10));


    // Added a termination code

    // Set termination flags
    terminate_producers = true;
    terminate_consumers = true;


    // Wait for the threads to finish
    for (int i = 0; i < PRODUCER_COUNT; ++i)
        pthread_join(producer_threads[i], NULL);

    for (int i = 0; i < CONSUMER_COUNT; ++i)
        pthread_join(consumer_threads[i], NULL);



    // After all threads finish, capture the end time
    auto program_end = std::chrono::steady_clock::now();

    auto duration = program_end - program_start;

    // Calculate overall turnaround time
    auto duration_seconds = std::chrono::duration_cast<std::chrono::seconds>(program_end - program_start).count();


    



// ... (existing code for thread creations and other operations)


    


    // Write to output file
    std::ofstream output_file("output- " + std::to_string(wait_time) + "sec-wait.txt");
    if (!output_file) {
        std::cerr << "Error creating output file." << std::endl;
        return 1;
    }

    output_file << "Turnaround time: " << duration_seconds << " seconds" << std::endl;

    output_file.close();

    return 0;   

}








