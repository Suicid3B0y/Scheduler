
#ifndef GSI_PROJECT_JOB_H
#define GSI_PROJECT_JOB_H

#include<string>

using namespace std;

class Job {

public:
    Job(string commande_line, int burst_time, int user_priority, int cpu_load) : commande_line(
            commande_line), burst_time(burst_time), user_priority(user_priority), cpu_load(
            cpu_load) { }


    const string &getCommande_line() const {
        return commande_line;
    }

    int getBurst_time() const {
        return burst_time;
    }

    int getUser_priority() const {
        return user_priority;
    }

    int getCpu_load() const {
        return cpu_load;
    }

private:
    string commande_line;
    int burst_time, user_priority, cpu_load;
};

#endif //GSI_PROJECT_JOB_H
