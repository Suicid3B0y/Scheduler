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

#include <iostream>
#include <boost/asio.hpp>
#include "scheduler.h"
#include "message_handler.h"

using boost::asio::ip::tcp;


#define MY_HOST "::1"
#define DEFAULT_LISTENING_PORT 4242


class Message
{
    private:
        int opcode;
        std::string payload;

    public:
        Message();
        Message(Message &message);
        Message(const unsigned char opcode, const std::string payload);
        Message& operator=(Message &message);

        void load_from_string(const std::string message);
        operator const std::string() const;
};

class NetworkEntity
{
    private:
        std::string endpoint_addr;
        std::unique_ptr<tcp::socket> socket_ptr;
        NetworkEntity(std::string endpoint_addr, std::unique_ptr<tcp::socket> socket_ptr);

    public:
        NetworkEntity();
        explicit NetworkEntity(const std::string endpoint_addr);
        NetworkEntity(std::string endpoint_addr, tcp::socket &socket);

        std::string get_message(int timeout);
        bool is_alive();
        bool is_me();

        friend NetworkEntity& operator<<(NetworkEntity& output_entity, const Message &message);
        friend NetworkEntity& operator>>(NetworkEntity& input_entity, Message &message);
};


class NetworkController
{
    private:
        boost::asio::io_service io_service;
        MessageHandler handler;

    public:
        NetworkController();
        explicit NetworkController(const MessageHandler &handler);
        NetworkController(const NetworkController &controller);
        NetworkController& operator=(const NetworkController &controller);

        NetworkEntity& connect(const std::string endpoint_addr);
        NetworkEntity& connect(const std::string endpoint_addr, const int endpoint_port);
};
