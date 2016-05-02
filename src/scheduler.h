#ifndef _H_SCHEDULER
#define _H_SCHEDULER

#include "controller.h"
#include "network.h"
#include "message_handler.h"
#include <chrono>
#include <thread>

/**
 * \class Scheduler
 * \brief Class entirely managing the scheduler.
 */
class Scheduler {

    private:
        unsigned timeSlice; /*!< Time at which the scheduler should update its jobs */
        bool alive;  /*!< Check if the scheduler is alive. */
        bool is_started;  /*!< Is the thread started */
        std::thread listening_thread;  /*!< Listening thread */
        Controller controller;  /*!< Controller used for managing the jobs. */
        MessageHandler handler;
        NetworkServer server;  /*!< Server handling client connection / remote scheduler connections */

        /**
         * \brief Starts the scheduler.
         */
        int run();

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
         * \brief Update the job queue with a new set of jobs
         */
        void updateJobQueue(vector<Job>& newJobs);

        /**
         * \brief Starts the scheduler.
         */
        void start();

        /**
         * \brief Join the scheduler.
         */
        void join();

};

#endif
