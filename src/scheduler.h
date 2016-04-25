#ifndef _H_SCHEDULER
#define _H_SCHEDULER

#include "controller.h"

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
