#include "scheduler.h"

Scheduler::Scheduler() : timeSlice(100), controller(Controller{}), alive(true) { }

Scheduler::Scheduler(const Scheduler &other) : Scheduler() {
    (*this) = other;
}

Scheduler::Scheduler(Controller controller, unsigned timeSlice) : controller(controller), timeSlice(timeSlice), alive(true) {
    debug("Scheduler instantiation" << endl);
}

Scheduler &Scheduler::operator=(const Scheduler &other) {
    this->timeSlice = other.timeSlice;
    this->controller = other.controller;
    return (*this);
}

bool Scheduler::isAlive() {
    return alive;
}

int Scheduler::run() {
    while (isAlive()) {
        controller.updateRunningJobs();
        std::cout << "Jobs updated." << std::endl;

        this_thread::sleep_for(std::chrono::duration<unsigned, std::milli>(timeSlice));
    }
    return 0;
}