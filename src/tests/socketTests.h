#ifndef _H_TESTS_SOCKETTESTS
#define _H_TESTS_SOCKETTESTS


#include <thread>
#include <chrono>

#include "common.h"
#include "../socket/Socket.h"


/**
 * \class SimpleEchoServer
 * \brief Simple server handling an unique connection and sending everything he receive.
 */
class SimpleEchoServer {
    private:
        std::thread listening_thread;  /*!< Thread listening for new connection. */
        Socket socket;  /*!< Server socket listening for new connection. */
        bool has_client;  /*!< Checks wether the server has a client or not */

    public:
        /**
         * \brief Constructor ; starts listening to a specific port.
         */
        SimpleEchoServer(unsigned port);

        /**
         * \brief Check if a connection has been initialized between the client and the server.
         */
        bool is_connected();

        /**
         * \brief Handle a unique connection : send back everything received from the client to the client.
         *
         * Once the '\0' character is received, close the connection.
         */
        void unique_handling();

        /**
         * \brief Join the listening thread.
         */
        void join();
};

void runSocketSuite();

#endif
