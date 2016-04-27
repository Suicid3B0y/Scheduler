#ifndef _H_SCHEDULER
#define _H_SCHEDULER

#include "controller.h"
#include <chrono>
#include <thread>

#include <iostream>


/**
 * Project constants definition
 */
#define DEBUG 1


using namespace std;

class Scheduler {

private:
    unsigned timeSlice;

    bool alive;

public:
    Controller controller;
    Scheduler();

    Scheduler(const Scheduler &other);

    Scheduler(unsigned core_number, unsigned timeout, unsigned timeSlice);

    Scheduler &operator=(const Scheduler &other);

    bool isAlive();

    int run();
};

#endif
