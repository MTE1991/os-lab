#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <atomic>

using namespace std;

int account_balance = 0;
mutex account_mutex;
condition_variable cv;
atomic<bool> alpha_turn(true); // Start with BotAlpha's turn

void BotAlpha(int deposit_amount, int times) {
    for (int i = 0; i < times; i++) {
        unique_lock<mutex> lock(account_mutex);
        cv.wait(lock, []{ return alpha_turn.load(); }); // Wait for Alpha's turn

        account_balance += deposit_amount;
        cout << "BotAlpha deposited " << deposit_amount << ", Balance: " << account_balance << endl;

        alpha_turn.store(false); // Switch to Beta's turn
        cv.notify_one(); // Notify Beta
        lock.unlock(); //optional: manually unlock earlier for performance
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

void BotBeta(int withdraw_amount, int times) {
    for (int i = 0; i < times; i++) {
        unique_lock<mutex> lock(account_mutex);
        cv.wait(lock, []{ return !alpha_turn.load(); }); // Wait for Beta's turn

        if (account_balance >= withdraw_amount) {
            account_balance -= withdraw_amount;
            cout << "BotBeta withdrew " << withdraw_amount << ", Balance: " << account_balance << endl;
        } else {
            cout << "BotBeta tried to withdraw " << withdraw_amount << " but insufficient funds, Balance: " << account_balance << endl;
        }

        alpha_turn.store(true); // Switch to Alpha's turn
        cv.notify_one(); // Notify Alpha
        lock.unlock(); //optional: manually unlock earlier for performance
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

int main() {
    thread bot1(BotAlpha, 50, 7);
    thread bot2(BotBeta, 30, 6);

    bot1.join();
    bot2.join();

    cout << "Final Account Balance: " << account_balance << endl;

    return 0;
}