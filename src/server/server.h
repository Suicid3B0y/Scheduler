//
// Created by archangel on 30/04/16.
//

#ifndef GSI_SCHEDULER_SERVER_H
#define GSI_SCHEDULER_SERVER_H

#include <thread>
#include <chrono>
#include "../job.h"
#include "../reader.h"
#include "../socket/Socket.h"


class ClientHandler {
    private:
        bool is_alive;
        Socket socket;
        Scheduler &scheduler;
        std::thread listening_thread;

        void handle_client();
        void handle_reception();

        /**
         * \brief Parse the string received through the socket.
         */
        bool parseSocketString(const std::string &line, std::string &result);

    public:
        ClientHandler(const Socket socket, Scheduler &scheduler);

        void start();
        void stop();
};

/**
 * \class Server
 * \brief Server handling the reception of a new job.
 */
class Server {
    private:
        bool is_alive;
        Socket socket;
        Scheduler &scheduler;  /*!< Scheduler on which we send the jobs */
        std::vector< std::unique_ptr<ClientHandler> > clients;
        std::thread listening_thread;  /*!< Listening thread */

        /**
         * \brief handler for acceptance
         */
        void handle_accept();

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
        void stop();
};


#endif //GSI_SCHEDULER_SERVER_H
