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

    void set_TAT() { TAT = CT - start_AT; }

    void set_WT() { WT = TAT - BT; }

    void P_set() {
        start_AT = AT;
        BT_left = BT;
    }

    void set_RT(time_t time) { RT = time - start_AT; }

    friend bool operator<(const process& a, const process& b) { return a.AT > b.AT; }
};

process pop_index(priority_queue<process>* main_queue, int index) {
    priority_queue<process> rm_index;
    process p;
    if (index == 0) {
        p = (*main_queue).top();
        (*main_queue).pop();
    } else {
        for (int i = 0; i < index; i++) {
            rm_index.push((*main_queue).top());
            (*main_queue).pop();
        }
        p = (*main_queue).top();
        (*main_queue).pop();
        while (!(*main_queue).empty()) {
            rm_index.push((*main_queue).top());
            (*main_queue).pop();
        }
        (*main_queue) = rm_index;
    }
    return p;
}

int max_priority(priority_queue<process> queue, int limit, bool high) {
    int max = -1;
    while (!queue.empty() && queue.top().AT <= limit) {
        if (high ? queue.top().priority > max : (max == -1 || queue.top().priority < max))
            max = queue.top().priority;
        queue.pop();
    }
    return max;
}

int max_priority_index(priority_queue<process> queue, int limit, bool high) {
    int max = -1, index = 0, i = 0;
    while (!queue.empty() && queue.top().AT <= limit) {
        if (high ? queue.top().priority > max : (max == -1 || queue.top().priority < max)) {
            max = queue.top().priority;
            index = i;
        }
        queue.pop();
        i++;
    }
    return index;
}

priority_queue<process> Priority_P_run(priority_queue<process> ready_queue, queue<process>* gantt, bool high) {
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
        p = pop_index(&ready_queue, max_priority_index(ready_queue, clock, high));
        if (p.AT == p.start_AT)
            p.set_RT(clock);
        while (p.BT_left > 0 && (ready_queue.empty() || clock < ready_queue.top().AT || (high ? p.priority >= max_priority(ready_queue, clock, high) : p.priority <= max_priority(ready_queue, clock, high)))) {
            p.temp_BT++;
            p.BT_left--;
            clock++;
        }
        if (p.BT_left == 0) {
            p.AT = p.start_AT;
            p.set_CT(clock);
            (*gantt).push(p);
            completion_queue.push(p);
        } else {
            p.AT = clock;
            p.CT = clock;
            (*gantt).push(p);
            ready_queue.push(p);
        }
    }
    return completion_queue;
}

priority_queue<process> set_sample_data() {
    priority_queue<process> ready_queue;
    process temp;
    for (int i = 1; i <= 7; i++) {
        temp.AT = i - 1;
        temp.BT = (i % 2 == 0) ? i : i + 1;
        temp.priority = i * 2;
        temp.p_no = i;
        temp.P_set();
        ready_queue.push(temp);
    }
    return ready_queue;
}

double get_total(priority_queue<process> processes, char type) {
    double total = 0;
    while (!processes.empty()) {
        switch (type) {
            case 'W': total += processes.top().WT; break;
            case 'T': total += processes.top().TAT; break;
            case 'C': total += processes.top().CT; break;
            case 'R': total += processes.top().RT; break;
        }
        processes.pop();
    }
    return total;
}

void disp(priority_queue<process> queue, bool high) {
    cout << "\nProcess No.  AT  BT  CT  TAT  WT  RT" << (high ? " Priority" : "") << "\n";
    while (!queue.empty()) {
        auto p = queue.top();
        cout << "P" << p.p_no << "\t" << p.AT << "\t" << p.BT << "\t" << p.CT << "\t" << p.TAT << "\t" << p.WT << "\t" << p.RT;
        if (high) cout << "\t" << p.priority;
        cout << endl;
        queue.pop();
    }
}

void disp_gantt_chart(queue<process> gantt) {
    cout << "\nGantt Chart:\n";
    while (!gantt.empty()) {
        cout << "| P" << gantt.front().p_no << " " << gantt.front().CT << " ";
        gantt.pop();
    }
    cout << "|\n";
}

int main() {
    priority_queue<process> ready_queue = set_sample_data(), completion_queue;
    queue<process> gantt;
    completion_queue = Priority_P_run(ready_queue, &gantt, true);
    disp(completion_queue, true);
    disp_gantt_chart(gantt);
    return 0;
}
