#ifndef _H_CONTROLLER
#define _H_CONTROLLER


#include "scheduler.h"
#include "job.h"

#include <vector>
#include <algorithm>


using namespace std;


class Controller {

    private:
        JobQueue jobQueue;
        vector<Job> currentJobs;
        unsigned int coreNumber;

    public:
        Controller(JobQueue jobQueue, unsigned int coreNumber);
        
        vector<Job> updateRunningJobs();
};


#endif
