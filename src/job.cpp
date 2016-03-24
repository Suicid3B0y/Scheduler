#include "job.h"


int aging_calculation(std::time_t t1, std::time_t t2) {
    return (t2 - t1);  // FIXME: seems really simple...
}


Job::Job(std::string command_line, int burst_time): command_line(command_line), burst_time(burst_time) {
    timestamp = std::time(nullptr);
    cout << "Job initialization (short sequence)" << endl;  // FIXME: for debug purpose only
}


Job::Job(std::string command_line, int burst_time, int user_priority, int cpu_load):
    command_line(command_line), burst_time(burst_time), user_priority(user_priority), cpu_load(cpu_load) {
    timestamp = std::time(nullptr);
    cout << "Job initialization (long sequence)" << endl;  // FIXME: for debug purpose only
}


bool operator>(const Job& left, const Job& right) {
    time_t aging;

    if (left.user_priority != right.user_priority) {
        return left.user_priority > right.user_priority;
    }

    aging = aging_calculation(left.timestamp, right.timestamp);

    return (right.burst_time - left.burst_time) + aging > 0;
}


bool operator<(const Job& left, const Job& right) {
    return right > left;
}

