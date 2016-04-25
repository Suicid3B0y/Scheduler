#include "scheduler.h"

Scheduler::Scheduler() : timeSlice(100), controller(Controller{}) { }

Scheduler::Scheduler(const Scheduler &other) : Scheduler() {
    (*this) = other;
}

Scheduler::Scheduler(Controller controller, unsigned timeSlice) : controller(controller), timeSlice(timeSlice) {
    debug("Scheduler instantiation" << endl);
}

Scheduler &Scheduler::operator=(const Scheduler &other) {
    this->timeSlice = other.timeSlice;
    this->controller = other.controller;
    return (*this);
}