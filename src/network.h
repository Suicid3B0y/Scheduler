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


class NetworkEntity
{
private:
    Socket socket;
    MessageHandler handler;
    std::string endpoint_addr;
    unsigned port;

    std::string local_buffer;  // Should not harm chickens...
    bool is_started;
    std::thread listening_thread;

    unsigned get_message_length(int timemout);  // NOTE: timeout is not supported right now (blocking IO)
    void wait_new_messages();

protected:
    void start();
    void close();  // NetworkEntity should be destructed after using this function.

public:
    NetworkEntity();
    NetworkEntity(const NetworkEntity &entity);
    NetworkEntity& operator=(const NetworkEntity &entity);
    NetworkEntity(const Socket &socket, const MessageHandler &handler, const std::string endpoint_addr, const unsigned port);
    ~NetworkEntity();
    bool operator==(const NetworkEntity &entity); // TODO

    bool is_alive();
    bool has_data();
    bool is_me();
    BaseMessage get_message(int timeout);  // NOTE: timeout is not supported right now (blocking IO)

    friend NetworkEntity& operator<<(NetworkEntity& output_entity, const BaseMessage &message);
    friend class NetworkServer;
};


// NOTE: to close properly the server, you MUST .stop() and .close() him.
class NetworkServer
{
private:
    Socket server;
    std::vector< std::unique_ptr<NetworkEntity> > clients;
    MessageHandler handler;
    bool is_alive;
    std::thread accept_thread;

    void do_accept();
    void handle_accept(Socket &socket);

public:
    NetworkServer(const unsigned short port, MessageHandler &handler);

    void start();
    void stop();
    void close();
    void close_connection(NetworkEntity &entity);

    NetworkEntity connect_to(const std::string endpoint_addr, const unsigned short port);
    std::vector<NetworkEntity> get_clients();
};


#endif