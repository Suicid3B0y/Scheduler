#include <unistd.h>
#include <sys/wait.h>
#include <stdarg.h>
#include "job.h"

static int jobIdValue = 0;  // FIXME: temporary work-around

Job::Job() : Job("", 0) {
}

Job::Job(string command_line, unsigned burst_time) : Job(command_line, burst_time, DEFAULT_USER_PRIORITY,
                                                         DEFAULT_CPU_LOAD) {
    this->command_line = command_line;
}

Job::Job(string command_line, unsigned burst_time, int user_priority, unsigned cpu_load)
        : timestamp{time(nullptr)},
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

int Job::getPid() const {
    return jobPid;
}

bool Job::start() {
    startTime = time(nullptr);
    if (runningTime == 0) {
        debug("[ ] Starting the job " << command_line << " time:" << startTime << endl);
        pid_t pid = fork();

        if (pid < 0) {
            exit(1);
        } else if (pid == 0) {
            execlp("/bin/sh", "/bin/sh", "-c", command_line.c_str(), (char *)NULL);
            exit(0);
        } else {
            this->jobPid = pid;
        }
        return true;
    } else { // Job déjà start
        debug("[ ] Restarting the job " << command_line << " time:" << startTime << " already runned for:" << runningTime << endl);
        kill(this->jobPid, SIGCONT);
        return true;
    }
}

bool Job::stop() {
    runningTime += time(nullptr) - startTime;
    debug("Stopping the job " << command_line << " (was running during "
          << time(nullptr) - startTime << " seconds)" << endl);
    startTime = 0;

    kill(this->jobPid, SIGSTOP);

    // TODO: stop the process...
    return true;
}

bool Job::isFinished() const {
    int status;
    pid_t result = waitpid(this->getPid(), &status, WNOHANG);
    return (result != 0);
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
