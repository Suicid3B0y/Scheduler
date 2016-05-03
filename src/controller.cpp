#include <signal.h>
#include "controller.h"

Controller::Controller() : Controller(JobQueue{}, 1, 200) { }

Controller::Controller(const Controller &other) : Controller() {
    (*this) = other;
}

Controller::Controller(JobQueue jobQueue, unsigned coreNumber, unsigned timeout) : jobQueue(jobQueue),
                                                                                   coreNumber(coreNumber),
                                                                                   timeout(timeout) {
    currentJobs = vector<job_ptr>();
    //debug("[ ] Controller instantiation" << endl);
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

    unsigned coreLoad [this->coreNumber];
    for (unsigned int i = 0; i < coreNumber; i++) {
        coreLoad[i] = 0;
    }

    for (unsigned int i = 0; i < currentJobs.size(); ++i) {
        if (!currentJobs[i].get()->isFinished()) {
            job_ptr tmp = currentJobs[i];
            coreLoad[currentJobs[i].get()->getRunningCore()] += currentJobs[i].get()->getCPULoad();
            jobQueue.push(tmp);
        }
    }
    bool overload = false;
    while (jobQueue.size()!=0 && !overload) {
        job_ptr queue_top = jobQueue.top();
        if (timeout!=0 && queue_top->getRunningTime() >= timeout) {
            debug("[!] process " << queue_top.get()->getCommandLine() << " timeout stop" << endl);
            kill(queue_top.get()->getPid(), SIGTERM);
        } else {
            newJobs.push_back(queue_top);
        }
        bool found = false;
        for(unsigned int j = 0; j < currentJobs.size(); ++j) {
            found = found || currentJobs[j]==queue_top;
        }
        if (!found) {
            int cpu = getAffinity(coreLoad, queue_top.get()->getCPULoad());
            if (cpu != -1) {
                coreLoad[cpu] += queue_top.get()->getCPULoad();
                queue_top.get()->start(cpu);
                //debug("[ ] process pid " << queue_top.get()->getPid() << " running on CPU:" << cpu << " for " << queue_top.get()->getCommandLine() << endl);

                jobQueue.pop();
            } else {
                overload = true;
                newJobs.pop_back();
            }
        } else {
            jobQueue.pop();
        }
    }
    for (unsigned int i = 0; i < currentJobs.size(); ++i) {
        bool found = false;
        for(unsigned int j = 0; j < newJobs.size(); ++j) {
            found = found || currentJobs[i]==newJobs[j];
        }
        if (!found) {
            currentJobs[i].get()->stop();
            //debug(endl);
        }
    }
    currentJobs = newJobs;

    return newJobs;
}

int Controller::getAffinity(unsigned coreLoad[], unsigned jobCpuLoad) {
    int iMax = -1;
    unsigned loadMax = 0;
    for (unsigned int i = 0; i < coreNumber; ++i) {
        unsigned curLoad = coreLoad[i] + jobCpuLoad;
        if (curLoad > loadMax && curLoad <= 100) {
            loadMax = curLoad;
            iMax = i;
        }
    }
    return iMax;
}

void Controller::updateJobQueue(vector<Job>& newJobs) {
    // TODO : verify this function

    for (Job &job: newJobs) {
        //debug("[-] " << job.getCommandLine() << endl);
        jobQueue.push(std::make_shared<Job>(Job(job)));
        //debug("[-] 2 " << job.getCommandLine() << endl);
    }

    //debug("[-] JobQueue top " << jobQueue.top()->getCommandLine() << endl);
    //debug("[-] JobQueue size " << jobQueue.size() << endl);
}
