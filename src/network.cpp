/*
 * =====================================================================================
 *
 *       Filename:  network.cpp
 *
 *    Description:  Scheduler network layer implementation
 *
 *        Version:  1.0
 *        Created:  04/05/16 17:36:07
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Axel Martin (axel.martin@eisti.fr)
 *
 * =====================================================================================
 */
#include "network.h"

Message::Message(): Message(0, "")
{
}

Message::Message(Message& message): Message(message.opcode, message.payload)
{
}

Message::Message(const unsigned char opcode, const std::string payload): opcode{opcode}, payload{payload}
{
}

Message& Message::operator=(Message &message)
{
    opcode = message.opcode;
    payload = message.payload;

    return (*this);
}

void Message::load_from_string(const std::string message)
{
    opcode = message[0];
    payload = message.substr(1);
}

Message::operator const std::string() const {
    return std::string(1, opcode) + payload;
}

NetworkEntity::NetworkEntity() : NetworkEntity(MY_HOST)
{
}

NetworkEntity::NetworkEntity(const std::string endpoint_addr)
    : NetworkEntity(endpoint_addr, std::unique_ptr<tcp::socket>(nullptr))
{
}

NetworkEntity::NetworkEntity(const std::string endpoint_addr, tcp::socket &socket)
    : NetworkEntity(endpoint_addr, std::unique_ptr<tcp::socket>(&socket))
{
}

NetworkEntity::NetworkEntity(const std::string endpoint_addr, std::unique_ptr<tcp::socket> socket_ptr)
    : endpoint_addr{endpoint_addr}, socket_ptr{std::move(socket_ptr)}
{
}

std::string NetworkEntity::get_message(int timeout)
{
    if (timeout < 0) {
        debug("Receiving message from " << endpoint_addr << " (blocking I/O)" << std::endl);
    } else {
        debug("Receiving message from " << endpoint_addr << " (timeout : " << timeout <<")" << std::endl);
    }
    return "toto"; // TODO
}

bool NetworkEntity::is_alive()
{
    return true; // TODO
}

bool NetworkEntity::is_me()
{
    return endpoint_addr == MY_HOST;
}

NetworkEntity& operator<<(NetworkEntity &output_entity, const Message &message)
{
    debug("Sending message to " << output_entity.endpoint_addr << std::endl);
    return output_entity;
}

NetworkEntity& operator>>(NetworkEntity &input_entity, Message &message) {
    message.load_from_string(input_entity.get_message(-1));
    return input_entity;
}

NetworkController::NetworkController(): NetworkController(DEFAULT_MESSAGE_HANDLER_CLASS{})
{
}

NetworkController::NetworkController(const MessageHandler &handler): io_service{}, handler{handler}
{
}

NetworkController::NetworkController(const NetworkController &controller): NetworkController(controller.handler)
{
}

NetworkController& NetworkController::operator=(const NetworkController &controller)
{
    handler = controller.handler;

    return (*this);
}

NetworkEntity& NetworkController::connect(const std::string endpoint_addr)
{
    return connect(endpoint_addr, DEFAULT_LISTENING_PORT);
}

NetworkEntity& NetworkController::connect(const std::string endpoint_addr, const int endpoint_port)
{
    tcp::resolver resolver(io_service);
    tcp::resolver::query query(endpoint_addr, std::to_string(endpoint_port));
    tcp::resolver::iterator it = resolver.resolve(query);

    tcp::socket socket(io_service);
    boost::asio::connect(socket, it);

    return *(new NetworkEntity(endpoint_addr, socket));
}
