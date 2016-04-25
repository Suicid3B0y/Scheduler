#include "job.h"

static int jobIdValue = 0;  // FIXME: temporary work-around

Job::Job() : Job("", 0) {
}

Job::Job(string command_line, unsigned burst_time) : Job(command_line, burst_time, DEFAULT_USER_PRIORITY,
                                                         DEFAULT_CPU_LOAD) {
    this->command_line = command_line;
}

Job::Job(string command_line, unsigned burst_time, int user_priority, int cpu_load) : timestamp{time(nullptr)},
                                                                                      isFinished{false},
                                                                                      startTime{0},
                                                                                      runningTime{0},
                                                                                      command_line{command_line},
                                                                                      burst_time{burst_time},
                                                                                      user_priority{user_priority},
                                                                                      cpu_load{cpu_load} {
    jobId = jobIdValue;  // FIXME: temporary work-around
    jobIdValue++;
}

Job &Job::operator=(const Job &other) {
    command_line = other.command_line;
    burst_time = other.burst_time;
    user_priority = other.user_priority;
    cpu_load = other.cpu_load;
    timestamp = other.timestamp;
    isFinished = other.isFinished;
    startTime = other.startTime;
    runningTime = other.runningTime;

    return *this;
}

bool operator==(const Job &left, const Job &right) {
    return left.getId() == right.getId();
}

int Job::getId() const {
    return jobId;
}

bool Job::start() {
    startTime = time(nullptr);
    debug("Starting the job " << command_line << " time:" << startTime << endl);

    // TODO: start the process...
    return true;
}

bool Job::stop() {
    runningTime += time(nullptr) - startTime;
    debug("Stopping the job " << command_line << " (was running during "
          << time(nullptr) - startTime << " seconds)" << endl);
    startTime = 0;

    // TODO: stop the process...
    return true;
}

bool JobPtrOrder::operator()(job_ptr const &left, job_ptr const &right) const {
    if (left->user_priority != right->user_priority) {
        return left->user_priority < right->user_priority;
    }

    std::time_t current_timestamp = std::time(nullptr);

    float l_burst_time = left->burst_time / (current_timestamp - left->timestamp + 1);
    float r_burst_time = right->burst_time / (current_timestamp - right->timestamp + 1);

    return (l_burst_time > r_burst_time);
}
