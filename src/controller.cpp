#include "controller.h"

Controller::Controller() : Controller(JobQueue{}, 1, 200) { }

Controller::Controller(const Controller &other) : Controller() {
    (*this) = other;
}

Controller::Controller(JobQueue jobQueue, unsigned coreNumber, unsigned timeout) : jobQueue(jobQueue),
                                                                                   coreNumber(coreNumber),
                                                                                   timeout(timeout) {
    currentJobs = vector<job_ptr>();
    debug("Controller instantiation" << endl);
}

Controller &Controller::operator=(const Controller &other) {
    this->jobQueue = other.jobQueue;
    this->coreNumber = other.coreNumber;
    this->timeout = other.timeout;
    this->currentJobs = other.currentJobs;
    return (*this);
}

vector<shared_ptr<Job> > Controller::updateRunningJobs() {
    vector<job_ptr> newJobs;

    for (unsigned int i = 0; i < currentJobs.size(); ++i) {
        if (!currentJobs[i].get()->isFinished()) {
            job_ptr tmp = currentJobs[i];
            jobQueue.push(tmp);
        }
    }

    if (jobQueue.size() != 0) {
        for (unsigned int i = 0; i < coreNumber; ++i) {
            job_ptr queue_top = jobQueue.top();
            newJobs.push_back(queue_top);
            jobQueue.pop();

            if (find(currentJobs.begin(), currentJobs.end(), newJobs[i]) == currentJobs.end()) {
                newJobs[i].get()->start();
                debug("[ ] process pid " << newJobs[i].get()->getPid() << endl);
            }
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

void Controller::updateJobQueue(vector<Job>& newJobs) {
    // TODO : verify this function

    for (Job &job: newJobs) {
        debug("[-] " << job.getCommandLine() << endl);
        jobQueue.push(std::make_shared<Job>(Job(job)));
        debug("[-] 2 " << job.getCommandLine() << endl);
    }

    debug("[-] JobQueue top " << jobQueue.top()->getCommandLine() << endl);
    debug("[-] JobQueue size " << jobQueue.size() << endl);
}
