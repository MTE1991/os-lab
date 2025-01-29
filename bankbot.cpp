#include <iostream>
#include <thread>
#include <semaphore.h>
#include <chrono>
using namespace std;
// Shared account balance
int account_balance = 0;

// Semaphore for synchronizing access
sem_t semaphore;

// Function for BotAlpha (Depositing money)
void BotAlpha(int deposit_amount, int times) {
    for (int i = 0; i < times; i++) {
        sem_wait(&semaphore); // Acquire semaphore
        account_balance += deposit_amount;
        cout << "BotAlpha deposited " << deposit_amount << endl;
        sem_post(&semaphore); // Release semaphore
        this_thread::sleep_for(chrono::milliseconds(100)); // Simulate processing time
    }
}

// Function for BotBeta (Withdrawing money)
void BotBeta(int withdraw_amount, int times) {
    for (int i = 0; i < times; i++) {
        sem_wait(&semaphore); // Acquire semaphore
        if (account_balance >= withdraw_amount) {
            account_balance -= withdraw_amount;
            cout << "BotBeta withdrew " << withdraw_amount << endl;
        } else {
            cout << "BotBeta tried to withdraw " << withdraw_amount << " but insufficient funds" << endl;
        }
        sem_post(&semaphore); // Release semaphore
        this_thread::sleep_for(chrono::milliseconds(100)); // Simulate processing time
    }
}

int main() {
    // Initialize the semaphore
    sem_init(&semaphore, 0, 1);

    // Threads for BotAlpha and BotBeta
    thread bot1(BotAlpha, 50, 7); // BotAlpha deposits 50 seven times (50 * 7 = 350)
    thread bot2(BotBeta, 30, 6); // BotBeta withdraws 30 six times (30 * 6 = 180)

    // Wait for threads to finish
    bot1.join();
    bot2.join();

    // Print the final account balance
    cout << "Final Account Balance: " << account_balance << endl;

    // Destroy the semaphore
    sem_destroy(&semaphore);

    return 0;
}

