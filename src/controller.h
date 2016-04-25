#ifndef _H_CONTROLLER
#define _H_CONTROLLER

#include "constants.h"
#include "job.h"

#include <vector>
#include <algorithm>

using namespace std;

class Controller {
private:
    JobQueue jobQueue;
    vector<job_ptr> currentJobs;
    unsigned coreNumber;
    unsigned timeout;

public:
    Controller();

    Controller(const Controller &other);

    Controller(JobQueue jobQueue, unsigned coreNumber, unsigned timeout);

    Controller &operator=(const Controller &other);

    vector<job_ptr> updateRunningJobs();

    void updateJobQueue(vector<Job> newJobs);
};

#endif
