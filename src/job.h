#ifndef _H_JOB
#define _H_JOB

#include "constants.h"

#include <iostream>
#include <ctime>
#include <queue>
#include <memory>

using namespace std;


struct JobPtrOrder;

/**
 * \class Job
 * \brief Job controller, managing a job.
 *
 * A job is caracterized by several state variable (running / not running), the command line of the job,
 * and the job properties (burst time / user defined priority).
 *
 * This class manage entirely the job's state.
 */
class Job {
private:
    int jobId; /*!< Unique job id */
    int jobPid; /*!< Job PID, in case it is running */
    time_t timestamp;
    time_t startTime;  // NOTE: must be set to 0 if the job is not running
    time_t runningTime;
    string command_line;
    unsigned burst_time;
    int user_priority;
    int cpu_load;

public:

    // Constructors
    Job();

    Job(string command_line, unsigned burst_time);

    Job(string command_line, unsigned burst_time, int user_priority, int cpu_load);

    ~Job() { debug("[-] Destructor of " << command_line << endl); };

    Job &operator=(const Job &other);

    // Comparators
    // NOTE: a job CANNOT be equal to another. Then j1 > j2 == !(j1 < j2)
    friend bool operator==(const Job &left, const Job &right);

    bool isFinished() const;

    // Job operations
    int getId() const;

    int getPid() const;

    string getCommandLine() const;

    int getBurstTime() const;

    int getUserPriority() const;

    int getCPULoad() const;

    time_t getStartTime() const;

    time_t getRunningTime() const;

    bool start();

    bool stop();

    friend JobPtrOrder;
};

typedef shared_ptr<Job> job_ptr;

struct JobPtrOrder {
    bool operator()(job_ptr const &left, job_ptr const &right) const;
};
typedef priority_queue<job_ptr, vector<job_ptr>, JobPtrOrder> JobQueue;


#endif
