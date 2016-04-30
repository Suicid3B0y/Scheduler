#ifndef _H_JOB
#define _H_JOB

#include "constants.h"

#include <iostream>
#include <ctime>
#include <queue>
#include <memory>

using namespace std;

class Job {
private:
    int jobId;
    int jobPid;

public:
    // XXX: Currently public, might be private
    time_t timestamp;
    time_t startTime;  // NOTE: must be set to 0 if the job is not running
    time_t runningTime;
    string command_line;
    unsigned burst_time;
    int user_priority;
    unsigned cpu_load;

    // Constructors
    Job();

    Job(string command_line, unsigned burst_time);

    Job(string command_line, unsigned burst_time, int user_priority, unsigned cpu_load);

    ~Job() { debug("[-] Destructor of " << command_line << endl); };

    Job &operator=(const Job &other);

    // Comparators
    // NOTE: a job CANNOT be equal to another. Then j1 > j2 == !(j1 < j2)
    friend bool operator==(const Job &left, const Job &right);

    bool isFinished() const;

    // Job operations
    int getId() const;

    int getPid() const;

    bool start();

    bool stop();
};

typedef shared_ptr<Job> job_ptr;

struct JobPtrOrder {
    bool operator()(job_ptr const &left, job_ptr const &right) const;
};
typedef priority_queue<job_ptr, vector<job_ptr>, JobPtrOrder> JobQueue;


#endif
