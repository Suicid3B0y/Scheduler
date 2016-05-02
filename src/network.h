/*
 * =====================================================================================
 *
 *       Filename:  network.h
 *
 *    Description:  Scheduler network layer
 *
 *                  The protocol used in the network layer is designed to work on both
 *                  IPv4 and IPv6 network layer. It uses a TCP connection to send or
 *                  receive message to/from another scheduler, sending or receiving
 *                  informations about the job state, the (remote) scheduler state or
 *                  the (remote) scheduler configuration.
 *
 *                  The protocol is based on the Peer Wire Protocol (PWP) structure
 *                  of the Bit Torrrent Protocol 1.0 (BTP/1.0). It uses messages
 *                  splitted in 3 parts as following :
 *
 *                      +----------------+------------+---------+
 *                      | Message length | Message ID | Payload |
 *                      +----------------+------------+---------+
 *                      |     4 bytes    |   1 byte   | n bytes |
 *                      +----------------+------------+---------+
 *
 *                  Message Length:
 *                      4 bytes big-endian number containing the length of the payload
 *                      plus the length of the message ID (n+1). Message of size 0 MUST
 *                      be sent periodically as keep-alive message, meaning the connection
 *                      is still alive ; a connection should be drop if no keep-alive
 *                      message are received from a certain amount of time.
 *                  Message ID:
 *                      Message ID is the operation code (opcode) related to the message.
 *                      The list of possible opcode are defined below, and if a message with
 *                      an invalid opcode is received, the client MUST close the connection.
 *                  Payload:
 *                      Eventually the operation related to the opcode MIGHT require some
 *                      arguments or data, which are contained in the payload. Some operation
 *                      MIGHT require a specific payload length ; if the expected length is
 *                      not valid, the connection MUST be dropped.
 *
 *        Version:  1.0
 *        Created:  04/05/16 17:07:19
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Axel Martin (axel.martin@eisti.fr)
 *
 * =====================================================================================
 */

#ifndef _H_NETWORK
#define _H_NETWORK

#include <thread>
#include <iostream>
#include <chrono>
#include <memory>
#include <list>
#include <vector>

#include "scheduler.h"
#include "message_handler.h"
#include "socket/Socket.h"


#define MY_HOST "::1"
#define DEFAULT_LISTENING_PORT 4242


class NetworkEntity;
class NetworkServer;


/**
 * \class NetworkEntity
 * \brief Represent an entity (remote or local) connected through a socket.
 */
class NetworkEntity
{
    private:
        Socket socket;  /*!< Socket handling the connection. */
        MessageHandler handler;  /*!< Message handler used if a message is received from the remote endpoint. */
        std::string endpoint_addr;  /*!< Remote (or local) endpoint address. */
        unsigned port;  /*!< Remote (or local) endpoint port. */

        std::string local_buffer;  /*!< Local buffer containing the not treated data */
        bool is_started;  /*!< Boolean checking wether the listening thread is started or not. */
        std::thread listening_thread;  /*!< Thread listening for new messages */

        /**
         * \brief Shortcut for getting the length of the message send by the remote endpoint.
         */
        unsigned get_message_length(int timemout);

        /**
         * \brief Target method for the listening thread, awaiting for new message and handling them.
         */
        void wait_new_messages();

    protected:
        /**
         * \brief Start the listening thread.
         */
        void start();

        /**
         * \brief Stop the listening thread (and join it), and close the socket.
         */
        void close();

    public:
        /**
         * \brief Constructor.
         */
        NetworkEntity();

        /**
         * \brief Copy constructor.
         */
        NetworkEntity(const NetworkEntity &entity);

        /**
         * \brief Assignment operator.
         */
        NetworkEntity& operator=(const NetworkEntity &entity);

        /**
         * \brief Full constructor.
         */
        NetworkEntity(const Socket &socket, const MessageHandler &handler, const std::string endpoint_addr, const unsigned port);

        /**
         * \brief Destructor.
         */
        ~NetworkEntity();

        /**
         * \brief Comparison operator.
         */
        bool operator==(const NetworkEntity &entity); // TODO

        /**
         * \brief Check if the remote endpoint is alive.
         */
        bool is_alive();

        /**
         * \brief Check if the remote endpoint has send data over the socket.
         */
        bool has_data();

        /**
         * \brief Check wether the entity is local or remote.
         */
        bool is_me();

        /**
         * \brief Get a message.
         *
         * NOTE: timeout is not supported right now (blocking IO)
         */
        BaseMessage get_message(int timeout);  // NOTE: timeout is not supported right now (blocking IO)

        /**
         * \brief Operator used to send a message to the remote endpoint.
         */
        friend NetworkEntity& operator<<(NetworkEntity& output_entity, const BaseMessage &message);

        /**
         * \brief Friend class used to manage start / stop listening thread.
         */
        friend class NetworkServer;
};


/**
 * \class NetworkServer
 * \brief Instantiate a server that listen for new connections.
 *
 * NOTE: to close properly the server, you MUST .stop() and .close() him.
 */
class NetworkServer
{

    private:
        Socket server;  /*!< Server socket listening for new connections */
        std::vector< std::unique_ptr<NetworkEntity> > clients;  /*!< Vector containing all currently connected clients */
        MessageHandler handler;  /*!< Message handler used once a new message is received, passed to all sub NetworkEntity */
        bool is_alive;  /*!< Boolean indicating wether the thread is started or not */
        std::thread accept_thread;  /*!< Thread accepting new connections */

        /**
         * \brief Target method of the accepting thread.
         *
         * Accept a connection, create the socket and pass it to the handle_accept method.
         */
        void do_accept();

        /**
         * \brief Method call once a new client is connected.
         *
         * Create the NetworkEntity based on this socket, and starts its listening thread.
         */
        void handle_accept(Socket &socket);

    public:
        /**
         * \brief Constructor.
         */
        NetworkServer(const unsigned short port, MessageHandler &handler);

        /**
         * \brief Start the thread listening for new connections.
         */
        void start();

        /**
         * \brief Stop the thread listening for new connections.
         */
        void stop();

        /**
         * \brief Close the server socket and all clients sockets.
         */
        void close();

        /**
         * \brief Close a specific connection and remove it from the list of the connected clients.
         */
        void close_connection(NetworkEntity &entity);

        /**
         * \brief Connect to a remote client.
         */
        NetworkEntity connect_to(const std::string endpoint_addr, const unsigned short port);

        /**
         * \brief Get the list of all currently connected clients.
         */
        std::vector<NetworkEntity> get_clients();

};


#endif
