#ifndef _H_TESTS_NETWORKTESTS
#define _H_TESTS_NETWORKTESTS


#include <thread>
#include <chrono>

#include "common.h"
#include "../network.h"
#include "../message_handler.h"
#include "../socket/Socket.h"

/**
 * \brief FakeServer used to generate tests.
 */
class FakeServer {
    private:
        Socket server;  /*!< Listening server socket */
        Socket client_socket;  /*!< Remote socket */
        unsigned port;  /*!< Listening port */
        std::thread t;  /*!< Listening thread */

    public:
        /**
         * \brief Constructor.
         */
        FakeServer(unsigned port);

        /**
         * \brief Handler for accepting remote socket connection.
         */
        void handle_acceptance();

        /**
         * \brief Getter for the remote socket.
         */
        Socket get_remote();
};

void runNetworkSuite();

#endif
