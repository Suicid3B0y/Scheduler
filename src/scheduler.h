#ifndef _H_SCHEDULER
#define _H_SCHEDULER

#include "controller.h"
#include <chrono>
#include <thread>

using namespace std;

class Scheduler {

private:
    unsigned timeSlice;
    Controller controller;
    bool alive;

public:
    Scheduler();

    Scheduler(const Scheduler &other);

    Scheduler(Controller controller, unsigned timeSlice);

    Scheduler &operator=(const Scheduler &other);

    bool isAlive();

    int run();
};

#endif
