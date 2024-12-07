#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <atomic>
#include <thread>
#include <mutex>
#include <semaphore.h>
#include <pthread.h>
#include "buffer.h"
#include <unistd.h>

using namespace std;

// Define a class to encapsulate shared resources
class SharedResources {
public:
    sem_t slots_available, slots_occupied;   // Semaphores for buffer slot management
    pthread_mutex_t buffer_mutex;           // Mutex for buffer synchronization
    std::atomic<bool> stop_producers;       // Flag to stop producer threads
    std::atomic<bool> stop_consumers;       // Flag to stop consumer threads

    SharedResources() : stop_producers(false), stop_consumers(false) {
        sem_init(&slots_available, 0, BUFFER_SIZE);
        sem_init(&slots_occupied, 0, 0);
        pthread_mutex_init(&buffer_mutex, NULL);
    }

    ~SharedResources() {
        sem_destroy(&slots_available);
        sem_destroy(&slots_occupied);
        pthread_mutex_destroy(&buffer_mutex);
    }
};

// Define a circular buffer class
class CircularBuffer {
public:
    int data[BUFFER_SIZE];
    int write_index = 0, read_index = 0;

    bool add(int item) {
        if ((write_index + 1) % BUFFER_SIZE == read_index) {
            return false;  // Buffer is full
        }
        data[write_index] = item;
        write_index = (write_index + 1) % BUFFER_SIZE;
        return true;
    }

    bool fetch(int &item) {
        if (write_index == read_index) {
            return false;  // Buffer is empty
        }
        item = data[read_index];
        read_index = (read_index + 1) % BUFFER_SIZE;
        return true;
    }
};

// Thread functions
void *producer(void *args) {
    auto *resources = static_cast<SharedResources *>(args);
    CircularBuffer &buffer = *static_cast<CircularBuffer *>(args);

    while (!resources->stop_producers) {
        usleep((rand() % 1000) * 1000);  // Random delay
        int item = rand() % 100;        // Generate a random item

        sem_wait(&resources->slots_available);
        pthread_mutex_lock(&resources->buffer_mutex);

        buffer.add(item);  // Add item to buffer

        pthread_mutex_unlock(&resources->buffer_mutex);
        sem_post(&resources->slots_occupied);
    }
    return nullptr;
}

void *consumer(void *args) {
    auto *resources = static_cast<SharedResources *>(args);
    CircularBuffer &buffer = *static_cast<CircularBuffer *>(args);

    while (!resources->stop_consumers) {
        usleep((rand() % 1000) * 1000);  // Random delay
        int item;

        sem_wait(&resources->slots_occupied);
        pthread_mutex_lock(&resources->buffer_mutex);

        if (buffer.fetch(item)) {  // Fetch item from buffer
            std::cout << "Consumed item: " << item << std::endl;
        }

        pthread_mutex_unlock(&resources->buffer_mutex);
        sem_post(&resources->slots_available);
    }
    return nullptr;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input-file>" << std::endl;
        return 1;
    }

    std::ifstream input_config(argv[1]);
    if (!input_config) {
        std::cerr << "Error opening input file." << std::endl;
        return 1;
    }

    int simulation_duration, producer_count, consumer_count;
    input_config >> simulation_duration >> producer_count >> consumer_count;
    input_config.close();

    SharedResources resources;
    CircularBuffer buffer;

    std::vector<pthread_t> producer_threads(producer_count);
    std::vector<pthread_t> consumer_threads(consumer_count);

    // Start timer
    auto start_time = std::chrono::steady_clock::now();

    // Create producer and consumer threads
    for (int i = 0; i < producer_count; ++i) {
        pthread_create(&producer_threads[i], nullptr, producer, &resources);
    }
    for (int i = 0; i < consumer_count; ++i) {
        pthread_create(&consumer_threads[i], nullptr, consumer, &resources);
    }

    // Run for the specified duration
    std::this_thread::sleep_for(std::chrono::seconds(simulation_duration));

    // Stop producers and consumers
    resources.stop_producers = true;
    resources.stop_consumers = true;

    // Wait for threads to finish
    for (auto &thread : producer_threads) {
        pthread_join(thread, nullptr);
    }
    for (auto &thread : consumer_threads) {
        pthread_join(thread, nullptr);
    }

    // Calculate execution time
    auto end_time = std::chrono::steady_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();

    // Write results to output file
    std::ofstream results_file("output-" + std::to_string(simulation_duration) + "sec-wait.txt");
    if (results_file) {
        results_file << "Turnaround time: " << elapsed_time << " seconds" << std::endl;
    } else {
        std::cerr << "Error creating output file." << std::endl;
        return 1;
    }

    return 0;
}