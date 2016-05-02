#ifndef _H_CONTROLLER
#define _H_CONTROLLER

#include "constants.h"
#include "job.h"
#include <chrono>

#include <vector>
#include <algorithm>

using namespace std;

/**
 * \class Controller
 * \brief Scheduler controller, managing which job will be start / stop depending of their priority.
 *
 */
class Controller {
private:
    JobQueue jobQueue; /*!< Priority queue containing all jobs */
    vector<job_ptr> currentJobs; /*!< Vector containing all running jobs */
    unsigned coreNumber; /*!< Number of cores the scheduler should use */
    unsigned timeout;

public:
    /**
     * \brief Constructor
     *
     * Instantiate and empty controller
     */
    Controller();

    /**
     * \brief Copy constructor
     */
    Controller(const Controller &other);

    /**
     * \brief Constructor
     *
     * Create a controller based on a job queue, the numbers of cores allocated to the scheduler and the job timeout.
     */
    Controller(JobQueue jobQueue, unsigned coreNumber,  unsigned timeout);

    /**
     * \brief Assignment operator
     */
    Controller &operator=(const Controller &other);

    /**
     * \brief Update the running jobs
     *
     * Stops the jobs having a lower priority and starts the higher priority jobs
     */
    vector<job_ptr> updateRunningJobs();

    /**
     * \brief Update the jobs queue
     *
     * Update the job queue by adding new jobs to the current jobQueue. An updateRunningJobs should be call after.
     */
    void updateJobQueue(vector<Job>& newJobs);
};

#endif
