//
// Created by archangel on 30/04/16.
//

#ifndef GSI_SCHEDULER_SERVER_H
#define GSI_SCHEDULER_SERVER_H

#include <thread>
#include "../job.h"
#include "../reader.h"
#include "../scheduler.h"
#include "../socket/Socket.h"


/**
 * \class Server
 * \brief Server handling the reception of a new job.
 */
class Server {
    private:
        bool is_started;  /*!< Boolean indicating if the server is started. */
        bool alive;  /*!< Boolean indicating if the server is alive. */
        Scheduler &scheduler;  /*!< Scheduler on which we send the jobs */
        std::thread listening_thread;  /*!< Listening thread */

        /**
         * \brief Target method of the running thread.
         */
        void run();

        /**
         * \brief Check if the thread is alive.
         */
        bool isAlive();

        /**
         * \brief Parse the string received through the socket.
         */
        bool parseSocketString(const std::string &line, std::string &result);

    public:

        /**
         * \brief Constructor.
         */
        Server(Scheduler &scheduler);

        /**
         * \brief Starts the server
         */
        void start();

        /**
         * \brief Join the current thread.
         */
        void join();
};


#endif //GSI_SCHEDULER_SERVER_H
