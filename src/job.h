#ifndef _H_JOB
#define _H_JOB

#include "scheduler.h"

#include <iostream>
#include <ctime>
#include <queue>
#include <memory>

using namespace std;


class Job {

    private:
        int jobId;
    
    public:
        // XXX: Currently public, might be private
        string command_line;
        int burst_time;
        int user_priority;
        int cpu_load;
        time_t timestamp;
        bool isFinished;  // FIXME: This is a temporary variable for testing purpose only.
        time_t startTime;  // NOTE: must be set to 0 if the job is not running
        time_t runningTime;

        // Constructors
        Job(string command_line, int burst_time);
        Job(string command_line, int burst_time, int user_priority, int cpu_load);

        // Comparators
        // NOTE: a job CANNOT be equal to another. Then j1 > j2 == !(j1 < j2)
        friend bool operator> (const Job& left, const Job& right);
        friend bool operator< (const Job& left, const Job& right);
        friend bool operator== (const Job& left, const Job& right);

        // Job operations
        int getId() const;
        bool start();
        bool stop();
};


typedef shared_ptr<Job> job_ptr;


struct JobPtrOrder
{
    bool operator()(job_ptr const& left, job_ptr const& right) const;
};


typedef priority_queue< job_ptr, vector<job_ptr>, JobPtrOrder > JobQueue;

#endif
