#include "Controller.h"

Controller::Controller(JobQueue jobQueue, unsigned int coreNumber): jobQueue(jobQueue), coreNumber(coreNumber) {
    currentJobs = vector<Job>();
    debug("Controller instantiation" << endl);
}


vector<Job> Controller::updateRunningJobs() {
    unsigned int i;
    vector<Job> newJobs;

    for (i = 0 ; i < currentJobs.size() ; ++i) {
        if (!currentJobs[i].isFinished) {
            jobQueue.emplace(currentJobs[i]);
        }
    }

    // newJobs.reserve(min(coreNumber, jobQueue.size()));
    for (i = 0 ; i < coreNumber ; ++i) {
        newJobs.emplace_back(jobQueue.top());
        jobQueue.pop();

        if (find(currentJobs.begin(), currentJobs.end(), newJobs[i]) == currentJobs.end()) {
            newJobs[i].start();
        }
    }

    for (i = 0 ; i < currentJobs.size() ; ++i) {
        if (!(find(newJobs.begin(), newJobs.end(), currentJobs[i]) != newJobs.end())) {
            currentJobs[i].stop();
        }
    }

    currentJobs = newJobs;
    
    return newJobs;
}
