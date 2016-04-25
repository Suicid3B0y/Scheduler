#ifndef _H_SCHEDULER
#define _H_SCHEDULER

#include "controller.h"

#include <iostream>


/**
 * Project constants definition
 */
#define DEBUG 1


using namespace std;

class Scheduler {

private:
    unsigned timeSlice;
    Controller controller;

public:
    Scheduler();

    Scheduler(const Scheduler &other);

    Scheduler(Controller controller, unsigned timeSlice);

    Scheduler &operator=(const Scheduler &other);

};

#endif
