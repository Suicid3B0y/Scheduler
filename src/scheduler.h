#ifndef _H_SCHEDULER
#define _H_SCHEDULER

#include "controller.h"
#include <chrono>
#include <thread>

class Scheduler {

private:
    unsigned timeSlice;

    bool alive;
    bool is_started;

    int run();

public:
    std::thread listening_thread;
    Controller controller;
    Scheduler();

    Scheduler(const Scheduler &other);

    Scheduler(unsigned core_number, unsigned timeout, unsigned timeSlice);

    Scheduler &operator=(const Scheduler &other);
    bool isAlive();

    void start();
};

#endif
