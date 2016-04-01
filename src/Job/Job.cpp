#include "Job.h"

static int jobIdValue = 0;  // FIXME: temporary turnaround


int aging_calculation(time_t t1, time_t t2) {
    return (t2 - t1);  // FIXME: seems really simple...
}


Job::Job(string command_line, int burst_time): command_line(command_line), burst_time(burst_time) {
    timestamp = time(nullptr);
    startTime = 0;
    runningTime = 0;
    isFinished = false;

    jobId = jobIdValue;
    jobIdValue++;
    debug("Job initialization (short sequence)" << endl);
}


Job::Job(string command_line, int burst_time, int user_priority, int cpu_load):
    command_line(command_line), burst_time(burst_time), user_priority(user_priority), cpu_load(cpu_load) {
    timestamp = time(nullptr);
    startTime = 0;
    runningTime = 0;
    isFinished = false;

    jobId = jobIdValue;
    jobIdValue++;
    debug("Job initialization (long sequence)" << endl);
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
<<<<<<< f0680e82eec902eea854dd062f728f114c18cb4b:src/Job/Job.cpp
}
=======
}


bool operator==(const Job& left, const Job& right) {
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
    debug("Stopping the job " << command_line << " (was running during " << time(nullptr) - startTime << " seconds)" << endl);
    startTime = 0;

    // TODO: stop the process...
    return true;
}
>>>>>>> featureSchedulerController: scheduler controller structure:src/job.cpp
