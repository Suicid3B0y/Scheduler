#ifndef _H_JOB_QUEUE
#define _H_JOB_QUEUE


#include <iostream>  // FIXME: for debug purpose only
#include <ctime>
#include <queue>


using namespace std;


class Job {

    private:
    
    public:
        // XXX: Currently public, might be private
        string command_line;
        int burst_time;
        int user_priority;
        int cpu_load;
        time_t timestamp;

        // Constructors
        Job(string command_line, int burst_time);
        Job(string command_line, int burst_time, int user_priority, int cpu_load);

        // Comparators
        // NOTE: a job CANNOT be equal to another. Then j1 > j2 == !(j1 < j2)
        friend bool operator> (const Job& left, const Job& right);
        friend bool operator< (const Job& left, const Job& right);

};


struct JobComparator {
    bool operator()(const Job& left, const Job& right) {
        return left < right;
    }
};


typedef priority_queue<Job, vector<Job>> JobQueue;

#endif
