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

unsigned Message::message_length() const
{
    return payload.size() + 1;
}

std::string Message::encoded_message_length() const
{
    unsigned char packed[4];
    unsigned lgt = message_length();

    packed[0] = lgt >> 24;
    packed[1] = lgt >> 16 & 0xff;
    packed[2] = lgt >> 8 & 0xff;
    packed[3] = lgt & 0xff;

    return std::string(packed, packed + 4);
}

Message::operator const std::string() const {
    return std::string(1, opcode) + payload;
}

// ----------------------------------------------------------------------------

NetworkEntity::NetworkEntity() :
    p_socket{std::shared_ptr<ServerSocket>()}, endpoint_addr{MY_HOST}, p_handler{std::shared_ptr<MessageHandler>()}, local_buffer{}
{
}

NetworkEntity::NetworkEntity(const NetworkEntity &entity) :
    NetworkEntity(entity.p_socket, entity.endpoint_addr, entity.p_handler)
{
    local_buffer = entity.local_buffer;
}

NetworkEntity::NetworkEntity(const std::shared_ptr<ServerSocket> &p_socket, const std::string endpoint_addr, const std::shared_ptr<MessageHandler> p_handler)
    : NetworkEntity()
{
    this->p_socket = p_socket;
    this->endpoint_addr = endpoint_addr;
    this->p_handler = p_handler;
}

NetworkEntity& NetworkEntity::operator=(const NetworkEntity &entity)
{
    p_socket = entity.p_socket;
    endpoint_addr = entity.endpoint_addr;
    p_handler = entity.p_handler;
    local_buffer = entity.local_buffer;

    return (*this);
}

unsigned NetworkEntity::get_message_length(int timeout)
{
    std::string data = local_buffer;
    local_buffer = "";

    while (data.length() < 4) {
        std::string tmp;
        *p_socket.get() >> tmp;
        data += tmp;
    }

    if (data.length() > 4)
        local_buffer = data.substr(4);

    unsigned sum = 0;
    for (unsigned i = 0; i < 4; ++i)
        sum += (int)data[i] << (8 * (3 - i));

    return sum;
}

Message NetworkEntity::get_message(int timeout)
{
    unsigned length = get_message_length(timeout);
    std::string data = local_buffer;

    while (data.length() < length)
    {
        std::string tmp;
        *p_socket >> tmp;
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
    (*output_entity.p_socket.get()) << message.encoded_message_length() << message;
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
    server.set_non_blocking(true);
}

void NetworkServer::do_accept()
{
    while (is_alive) {
        std::shared_ptr<ServerSocket> p_client_socket = std::make_shared<ServerSocket>(ServerSocket{});
        if (server.accept(p_client_socket))
            handle_accept(p_client_socket);
        else
            std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void NetworkServer::handle_accept(std::shared_ptr<ServerSocket> &p_client_socket)
{
    NetworkEntity entity(p_client_socket, "todo", p_handler);  // TODO
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

std::vector< std::shared_ptr<NetworkEntity> > NetworkServer::get_clients()
{
    std::vector< std::shared_ptr<NetworkEntity> > res;
    res.reserve(clients.size());

    for (unsigned i = 0; i < clients.size(); ++i)
        res.push_back(std::shared_ptr<NetworkEntity>(clients[i]));

    return res;
}
