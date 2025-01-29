#include <cstdlib>
#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>

using namespace std;

// Global variables shared between producer and consumer
std::mutex g_mutex;    // Mutex for synchronizing access to shared data
bool g_ready = false;  // Flag indicating whether data is ready to be consumed
int g_data = 0;        // Shared data between producer and consumer

// Function to simulate data production
int produceData() {
  // Generate a random number and print it
  int randomNumber = rand() % 1000;
  std::cout << "Produced data: " << randomNumber << "\n";
  return randomNumber;
}

// Function to simulate data consumption
void consumeData(int data) {
  // Print the received data
  std::cout << "Consumed data: " << data << "\n";
}

// Consumer function to continuously consume data
void consumer() {
  while (true) {
    // Wait until the producer has data ready
    while (!g_ready) {
      std::this_thread::sleep_for(std::chrono::milliseconds(80));  // Simulate waiting
    }

    // Lock the mutex before consuming data
    std::unique_lock<std::mutex> lock(g_mutex);
    consumeData(g_data);   // Consume the data
    g_ready = false;       // Reset the flag to indicate that data has been consumed
  }
}

// Producer function to continuously produce data
void producer() {
  while (true) {
    // Lock the mutex before producing data
    std::unique_lock<std::mutex> lock(g_mutex);

    g_data = produceData();  // Produce new data
    g_ready = true;          // Set the flag to indicate data is ready
    lock.unlock();           // Unlock the mutex to allow the consumer to access the data

    // Wait until the consumer consumes the data before producing again
    while (g_ready) {
      std::this_thread::sleep_for(std::chrono::milliseconds(80));  // Simulate waiting
    }
  }
}

int main() {
  // Create and start consumer and producer threads
  std::thread t1(consumer);
  std::thread t2(producer);

  // Wait for threads to finish execution
  t1.join();
  t2.join();

  return 0;
}
