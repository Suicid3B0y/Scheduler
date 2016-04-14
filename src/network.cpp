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

Message::Message(const std::string raw_message): Message(raw_message[0], raw_message.substr(1))
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

// ----------------------------------------------------------------------------

NetworkEntity::NetworkEntity() : NetworkEntity(ServerSocket{}, MY_HOST, std::make_shared<MessageHandler>())
{
}

NetworkEntity::NetworkEntity(const NetworkEntity &entity) : NetworkEntity(entity.socket, entity.endpoint_addr, entity.p_handler)
{
}

NetworkEntity::NetworkEntity(const ServerSocket &socket, const std::string endpoint_addr, const std::shared_ptr<MessageHandler> p_handler)
    : socket{socket}, endpoint_addr{endpoint_addr}, p_handler{p_handler}
{
}

NetworkEntity& NetworkEntity::operator=(const NetworkEntity &entity)
{
    socket = entity.socket;
    endpoint_addr = entity.endpoint_addr;
    p_handler = entity.p_handler;

    return (*this);
}

unsigned NetworkEntity::get_message_length(int timeout)
{
    std::string data = local_buffer;
    local_buffer = "";

    while (data.length() < 4)
    {
        std::string tmp;
        socket >> tmp;
        data += tmp;
    }

    if (data.length() > 4)
        local_buffer = data.substr(4);

    unsigned sum = 0;
    for (unsigned i = 0; i < 4; ++i)
        sum += (int)data[i] << (8 * i);

    return sum;
}

Message NetworkEntity::get_message(int timeout)
{
    unsigned length = get_message_length(timeout);
    std::string data = local_buffer;

    while (data.length() < length)
    {
        std::string tmp;
        socket >> tmp;
        data = tmp;
    }

    if (data.length() > length)
        local_buffer = data.substr(length);

    Message m{data};
    return m;
}

void NetworkEntity::start()
{
    debug("Starting network entity" << std::endl);
}

void NetworkEntity::stop()
{
    debug("Stopping network entity" << std::endl);
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

// ----------------------------------------------------------------------------

NetworkServer::NetworkServer()
{
}

NetworkServer::NetworkServer(NetworkServer &network_server)
    : server{network_server.server},
      clients{std::vector< std::shared_ptr<NetworkEntity> >()},
      p_handler{network_server.p_handler},
      is_alive{false}
{
    clients.reserve(network_server.clients.size());

    for (unsigned i = 0; i < network_server.clients.size(); ++i) {
        clients.push_back(network_server.clients[i]);
    }
}

NetworkServer& NetworkServer::operator=(NetworkServer &network_server)
{
    NetworkServer tmp(network_server);

    server = tmp.server;
    clients = tmp.clients;
    p_handler = tmp.p_handler;
    is_alive = tmp.is_alive;

    return (*this);
}

NetworkServer::NetworkServer(const short port, MessageHandler &handler)
    : server{ServerSocket(port)},
      clients{std::vector< std::shared_ptr<NetworkEntity> >()},
      p_handler{std::make_shared<MessageHandler>(handler)},
      is_alive{false}
{
}

void NetworkServer::do_accept()
{
    while (is_alive) {
        ServerSocket client_socket;
        server.accept(client_socket);
        handle_accept(client_socket);
    }
}

void NetworkServer::handle_accept(ServerSocket client_socket)
{
    NetworkEntity entity(client_socket, "todo", p_handler);  // TODO

    clients.push_back(std::make_shared<NetworkEntity>(entity));

    entity.start();
}

void NetworkServer::start()
{
    is_alive = true;

    // http://stackoverflow.com/questions/10673585/start-thread-with-member-function
    std::thread tmp(&NetworkServer::do_accept, this);
    accept_thread = std::move(tmp);
}

void NetworkServer::stop()
{
    is_alive = false;

    // FIXME: might require a fake connection initialization here.
    accept_thread.join();
}
