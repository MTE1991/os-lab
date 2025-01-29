#include <iostream>
#include <thread>
#include <vector>

using namespace std;

int BUFF_SIZE = 5;
vector<int> buff(BUFF_SIZE);
int in = 0; // Producer
int out = 0; // Consumer
int item = 0;

void produce() {
    for (int i = 0; i < 5; i++) {
        buff[in] = ++item;
        cout << "Produced item no = " << item << " at index " << in << endl;
        in = (in + 1) % BUFF_SIZE;
        this_thread::sleep_for(chrono::microseconds(500));
    }
}

void consume() {
    for (int i = 0; i < 5; i++) {
        cout << "Consumed item no = " << item << "at index " << out << endl;
        out = (out + 1) % BUFF_SIZE;
        this_thread::sleep_for(chrono::milliseconds(800));
    }
}

int main() {
    thread pthread(produce);
    thread cthread(consume);
    pthread.join();
    cthread.join();

    return 0;
}