#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <chrono>

using namespace std;

class DiningPhilosophers {
private:
    enum State { THINKING, HUNGRY, EATING };
    State state[5];
    condition_variable self[5];
    mutex mtx;
    int eatingCount = 0;   // total number of meals eaten
    int mealCount[5] = {0}; // count the meals for each philosopher (max = 1)

    void test(int i) {
        if (state[(i + 4) % 5] != EATING &&
            state[i] == HUNGRY &&
            state[(i + 1) % 5] != EATING) {
            state[i] = EATING;
            self[i].notify_one();
        }
    }

public:
    DiningPhilosophers() {
        for (int i = 0; i < 5; ++i) {
            state[i] = THINKING;
        }
    }

    void pickup(int i) {
        unique_lock<mutex> lock(mtx);
        state[i] = HUNGRY;
        cout << "Philosopher " << i << " is hungry.\n";
        test(i);
        if (state[i] != EATING) {
            self[i].wait(lock);
        }
        cout << "Philosopher " << i << " picked up forks " << (i) << " and " << (i + 1) % 5 << ".\n";
    }

    void putdown(int i) {
        unique_lock<mutex> lock(mtx);
        state[i] = THINKING;
        cout << "Philosopher " << i << " is putting down the forks " << (i) << " and " << (i + 1) % 5 << ".\n";
        test((i + 4) % 5); // Left philosopher
        test((i + 1) % 5); // Right philosopher
    }

    void philosopher(int i) {
        while (eatingCount < 5) { // Stop after all philosophers have eaten once
            think(i);
            pickup(i);
            eat(i);
            putdown(i);
        }
    }

    void think(int i) {
        cout << "Philosopher " << i << " is thinking.\n";
        this_thread::sleep_for(chrono::milliseconds(1000)); // sleep for 1s
    }

    void eat(int i) {
        cout << "Philosopher " << i << " is eating.\n";
        this_thread::sleep_for(chrono::milliseconds(1500)); // sleep for 1.5s
        mealCount[i]++;
        if (mealCount[i] == 1) {
            unique_lock<mutex> lock(mtx);
            eatingCount++; 
        }
    }
};

int main() {
    DiningPhilosophers p;

    vector<thread> philosophers; // philosopher threads
    for (int i = 0; i < 5; ++i) {
        philosophers.emplace_back(&DiningPhilosophers::philosopher, &p, i);
    }

    // Join the threads
    for (auto& philosopher : philosophers) {
        philosopher.join();
    }

    cout << "All philosophers have eaten once. Program ending.\n";
    return 0;
}