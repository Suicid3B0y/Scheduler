#include <unistd.h>
#include <sys/wait.h>
#include "job.h"

static int jobIdValue = 0;  // FIXME: temporary work-around

Job::Job() : Job("", 0) {
}

Job::Job(string command_line, unsigned burst_time)
        : Job(command_line, burst_time, DEFAULT_USER_PRIORITY, DEFAULT_CPU_LOAD) {
    this->command_line = command_line;
}

Job::Job(string command_line, unsigned burst_time, int user_priority, unsigned cpu_load)
        : timestamp{time(nullptr)},
          startTime{0},
          runningTime{0},
          command_line{command_line},
          burst_time{burst_time},
          user_priority{user_priority},
          cpu_load{cpu_load},
          running_core{0} {
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
    running_core = other.running_core;
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

string Job::getCommandLine() const
{
    return command_line;
}

int Job::getBurstTime() const
{
    return burst_time;
}

int Job::getUserPriority() const
{
    return user_priority;
}

int Job::getCPULoad() const
{
    return cpu_load;
}

int Job::getRunningCore() const
{
    return running_core;
}

time_t Job::getStartTime() const
{
    return startTime;
}

time_t Job::getRunningTime() const
{
    if (startTime==0) {
        return runningTime;
    } else {
        return runningTime+(time(nullptr)-startTime);
    }
}

bool Job::start(int& cpu) {
    startTime = time(nullptr);
    struct tm datetime;
    char  format[32];
    datetime = *localtime(&startTime);
    strftime(format, 32, "%Y/%m/%d %H:%M", &datetime);
    if (runningTime == 0) {
        this->running_core = cpu;

        debug("[>] Starting the job " << command_line << " time:" << format << " on CPU : " << cpu << endl);
        pid_t pid = fork();

        if (pid < 0) {
            exit(1);
        } else if (pid == 0) {
            cpu_set_t set;
            CPU_ZERO(&set);
            CPU_SET(cpu, &set);
            sched_setaffinity(getpid(), sizeof(set), &set);
            execlp("/bin/sh", "/bin/sh", "-c", command_line.c_str(), (char *)NULL);
            exit(0);
        } else {
            this->jobPid = pid;
        }
        return true;
    } else { // Job déjà start
        debug("[=] Restarting the job " << command_line << " time:" << format << " already runned for:" << runningTime << endl);
        kill(this->jobPid, SIGCONT);
        return true;
    }
}

bool Job::stop() {
    runningTime += time(nullptr) - startTime;
    debug("[<] Stopping the job " << command_line << " (was running during "
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
