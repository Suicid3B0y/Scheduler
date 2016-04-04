#include "Job.h"

Job::Job(std::string command_line, int burst_time) : command_line(command_line), burst_time(burst_time) {
    timestamp = std::time(nullptr);
    debug("Job initialization (short sequence)" << endl);
}

Job::Job(std::string command_line, int burst_time, int user_priority, int cpu_load) :
        command_line(command_line), burst_time(burst_time), user_priority(user_priority), cpu_load(cpu_load) {
    timestamp = std::time(nullptr);
    debug("Job initialization (long sequence)" << endl);
}

bool operator<(const Job &left, const Job &right) {

    if (left.user_priority != right.user_priority) {
        return left.user_priority < right.user_priority;
    }

    std::time_t current_timestamp = std::time(nullptr);

    float l_burst_time = left.burst_time / (current_timestamp - left.timestamp + 1);
    float r_burst_time = right.burst_time / (current_timestamp - right.timestamp + 1);

    return (l_burst_time > r_burst_time);
}

bool operator>(const Job &left, const Job &right) {
    return right < left;
}