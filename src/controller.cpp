#include "controller.h"

Controller::Controller(JobQueue jobQueue, unsigned int coreNumber)
    : jobQueue(jobQueue), coreNumber(coreNumber)
{
    currentJobs = vector<job_ptr>();
    debug("Controller instantiation" << endl);
}

vector<shared_ptr<Job> > Controller::updateRunningJobs()
{
    vector<job_ptr> newJobs;

    debug("[ ] Updating running jobs" << endl);

    for (unsigned int i = 0; i < currentJobs.size(); ++i) {
        if (!currentJobs[i].get()->isFinished) {
            job_ptr tmp = currentJobs[i];
            jobQueue.push(tmp);
        }
    }

    for (unsigned int i = 0; i < coreNumber; ++i) {
        job_ptr queue_top = jobQueue.top();
        newJobs.push_back(queue_top);
        jobQueue.pop();

        if (find(currentJobs.begin(), currentJobs.end(), newJobs[i]) == currentJobs.end()) {
            newJobs[i].get()->start();
        }
    }

    for (unsigned int i = 0; i < currentJobs.size(); ++i) {
        if (!(find(newJobs.begin(), newJobs.end(), currentJobs[i]) != newJobs.end())) {
            currentJobs[i].get()->stop();
        }
    }

    currentJobs = newJobs;

    return newJobs;
}
