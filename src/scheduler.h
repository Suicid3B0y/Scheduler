#ifndef _H_SCHEDULER
#define _H_SCHEDULER

#include "controller.h"
#include <chrono>
#include <thread>

using namespace std;

/**
 * \class Scheduler
 * \brief Class entirely managing the scheduler.
 */
class Scheduler {

    private:
        unsigned timeSlice; /*!< Time at which the scheduler should update its jobs */
        bool alive;  /*!< Check if the scheduler is alive. */
        Controller controller;  /*!< Controller used for managing the jobs. */

    public:
        /**
         * \brief Constructor.
         */
        Scheduler();

        /**
         * \brief Copy constructor.
         */
        Scheduler(const Scheduler &other);

        /**
         * \brief Full constructor.
         */
        Scheduler(unsigned core_number, unsigned timeout, unsigned timeSlice);

        /**
         * \brief Assignment operator
         */
        Scheduler &operator=(const Scheduler &other);

        /**
         * \brief Check if the scheduler is currently working.
         */
        bool isAlive();

        /**
         * \brief Starts the scheduler.
         */
        int run();
};

#endif
