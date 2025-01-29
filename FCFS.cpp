#include <cstdlib>
#include <iostream>
#include <queue>
using namespace std;

class process {
public:
    pid_t p_no = 0;
    time_t start_AT = 0, AT = 0, BT_left = 0, BT = 0, temp_BT = 0, CT = 0, TAT = 0, WT = 0, RT = 0;
    int priority = 0;

    void set_CT(time_t time) {
        CT = time;
        set_TAT();
        set_WT();
    }

    void set_TAT() {
        TAT = CT - start_AT;
    }

    void set_WT() {
        WT = TAT - BT;
    }

    void P_set() {
        start_AT = AT;
        BT_left = BT;
    }

    void set_RT(time_t time) {
        RT = time - start_AT;
    }

    friend bool operator<(const process& a, const process& b) {
        return a.AT > b.AT;
    }
};

priority_queue<process> FCFS_run(priority_queue<process> ready_queue, queue<process>* gantt) {
    priority_queue<process> completion_queue;
    process p;
    time_t clock = 0;

    while (!ready_queue.empty()) {
        while (clock < ready_queue.top().AT) {
            p.temp_BT++;
            clock++;
        }
        if (p.temp_BT > 0) {
            p.p_no = -1;
            p.CT = clock;
            (*gantt).push(p);
        }
        p = ready_queue.top();
        ready_queue.pop();
        p.set_RT(clock);
        while (p.BT_left > 0) {
            p.temp_BT++;
            p.BT_left--;
            clock++;
        }
        p.set_CT(clock);
        (*gantt).push(p);
        p.temp_BT = 0;
        completion_queue.push(p);
    }
    return completion_queue;
}

priority_queue<process> set_sample_data() {
    priority_queue<process> ready_queue;
    process temp;
    int process_data[7][3] = {
        {0, 4, 2}, {1, 2, 4}, {2, 3, 6}, {3, 5, 10}, {4, 1, 8}, {5, 4, 12}, {6, 6, 9}};

    for (int i = 0; i < 7; i++) {
        temp.AT = process_data[i][0];
        temp.BT = process_data[i][1];
        temp.priority = process_data[i][2];
        temp.p_no = i + 1;
        temp.P_set();
        ready_queue.push(temp);
    }
    return ready_queue;
}

double get_total_WT(priority_queue<process> processes) {
    double total = 0;
    while (!processes.empty()) {
        total += processes.top().WT;
        processes.pop();
    }
    return total;
}

double get_total_TAT(priority_queue<process> processes) {
    double total = 0;
    while (!processes.empty()) {
        total += processes.top().TAT;
        processes.pop();
    }
    return total;
}

double get_total_CT(priority_queue<process> processes) {
    double total = 0;
    while (!processes.empty()) {
        total += processes.top().CT;
        processes.pop();
    }
    return total;
}

double get_total_RT(priority_queue<process> processes) {
    double total = 0;
    while (!processes.empty()) {
        total += processes.top().RT;
        processes.pop();
    }
    return total;
}

void disp(priority_queue<process> main_queue, bool high) {
    int size = main_queue.size();
    priority_queue<process> tempq = main_queue;
    double temp1;
    
    cout << "\nTotal completion time: " << (temp1 = get_total_CT(tempq)) << "\n";
    cout << "Average completion time: " << temp1 / size << "\n";
    cout << "Total turnaround time: " << (temp1 = get_total_TAT(tempq)) << "\n";
    cout << "Average turnaround time: " << temp1 / size << "\n";
    cout << "Total waiting time: " << (temp1 = get_total_WT(tempq)) << "\n";
    cout << "Average waiting time: " << temp1 / size << "\n";
    cout << "Total response time: " << (temp1 = get_total_RT(tempq)) << "\n";
    cout << "Average response time: " << temp1 / size << "\n";
}

void disp_gantt_chart(queue<process> gantt) {
    int temp, prev = 0;
    queue<process> spaces = gantt;
    cout << "\nGantt Chart (IS indicates idle state):\n\n+";
    
    while (!spaces.empty()) {
        cout << string(2 * spaces.front().temp_BT, '-') << "+";
        spaces.pop();
    }
    cout << "\n|";
    spaces = gantt;
    
    while (!spaces.empty()) {
        cout << string(spaces.front().temp_BT, ' ');
        if (spaces.front().p_no == -1)
            cout << "IS" << string(spaces.front().temp_BT, ' ') << '|';
        else
            cout << "P" << spaces.front().p_no << string(spaces.front().temp_BT, ' ') << '|';
        spaces.pop();
    }
    spaces = gantt;
    cout << "\n+";
    
    while (!spaces.empty()) {
        cout << string(2 * spaces.front().temp_BT, '-') << "+";
        spaces.pop();
    }
    spaces = gantt;
    cout << "\n0";
    
    while (!spaces.empty()) {
        temp = to_string(spaces.front().CT).length();
        cout << string(2 * spaces.front().temp_BT - temp / 2 - prev, ' ') << spaces.front().CT;
        prev = temp / 2 - temp % 2 == 0;
        spaces.pop();
    }
    cout << "\n";
}

int main() {
    priority_queue<process> ready_queue = set_sample_data();
    priority_queue<process> completion_queue;
    queue<process> gantt;
    completion_queue = FCFS_run(ready_queue, &gantt);
    disp(completion_queue, false);
    disp_gantt_chart(gantt);
    return 0;
}