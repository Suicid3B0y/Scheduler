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
    time_t timestamp; /*!< Job instantiation timestamp */
    time_t startTime;  /*!< Job's start time. Must be set to 0 if the job is not running. */
    time_t runningTime; /*!< Job's running time, which is the sum of stop() - start() times */
    string command_line; /*!< Job's command line (i.e. what does the job) */
    unsigned burst_time; /*!< Job's burst time ; user defined parameter */
    int user_priority; /*!< Job's priority defined by the user */
    unsigned cpu_load; /*!< Job's CPU load */
    unsigned running_core; /*!< Job's Running Core. Core that the job is running on */

public:

    /**
     * \brief Constructor ; initialize an empty job.
     */
    Job();

    /**
     * \brief Job short instantiation, using default user priority and default cpu_load.
     */
    Job(string command_line, unsigned burst_time);

    /**
     * \brief Job long instantiation, if the user specify a specific user priority and cpu load.
     */
    Job(string command_line, unsigned burst_time, int user_priority, unsigned cpu_load);

    /**
     * \brief Assignement operator
     */
    Job &operator=(const Job &other);

    /**
     * \brief Job comparison
     */
    friend bool operator==(const Job &left, const Job &right);

    /**
     * \brief Accessor allowing to know if the job is finished.
     */
    bool isFinished() const;

    /**
     * \brief Getter of the Job id.
     */
    int getId() const;

    /**
     * \brief Getter of the Job PID.
     */
    int getPid() const;

    /**
     * \brief Getter of the Job command line.
     */
    string getCommandLine() const;

    /**
     * \brief Getter of the Job burst time.
     */
    int getBurstTime() const;

    /**
     * \brief Getter of the Job priority.
     */
    int getUserPriority() const;

    /**
     * \brief Getter of the Job CPU load.
     */
    int getCPULoad() const;

    /**
     * \brief Getter of the Job Running Core.
     */
    int getRunningCore() const;

    /**
     * \brief Getter of the Job start time.
     */
    time_t getStartTime() const;

    /**
     * \brief Getter of the Job running time.
     */
    time_t getRunningTime() const;

    /**
     * \brief Start the job.
     */
    bool start(int& cpu);

    /**
     * \brief Stop the job.
     */
    bool stop();

    /**
     * \brief Friend class used for sorting a job depending of its priority.
     */
    friend JobPtrOrder;
};


/**
 * \struct job_ptr
 * \brief Shared pointer to a job.
 */
typedef shared_ptr<Job> job_ptr;

/**
 * \struct JobPtrOrder
 * \brief Structure allowing to sort a job priority queue based on a sorting strategy.
 *
 * TODO: document this
 * NOTE: a job CANNOT be equal to another. Then j1 > j2 == !(j1 < j2)
 */
struct JobPtrOrder {

    /**
     * \brief Operator used to sort two jobs.
     */
    bool operator()(job_ptr const &left, job_ptr const &right) const;
};

/**
 * \struct JobQueue
 * \brief Priority queue of jobs, allowing to get the most prioritised job.
 */
typedef priority_queue<job_ptr, vector<job_ptr>, JobPtrOrder> JobQueue;


#endif
