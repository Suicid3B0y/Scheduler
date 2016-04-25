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
// ----------------------------------------------------------------------------

NetworkEntity::NetworkEntity() :
    p_socket{std::shared_ptr<ServerSocket>()}, endpoint_addr{MY_HOST},
    p_handler{std::shared_ptr<MessageHandler>()}, local_buffer{},
    is_started{false}
{
}

NetworkEntity::~NetworkEntity()
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

BaseMessage NetworkEntity::get_message(int timeout)
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

    BaseMessage m{data};
    return m;
}

void NetworkEntity::wait_new_messages()
{
    try {
        while (is_started) {
            if (has_data()) {
                BaseMessage message{get_message(0)};
                p_handler->handle_message(message);
            } else {
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }
    } catch (const std::exception &e) {
        debug("Exception caught on network entity !");
    }
}

void NetworkEntity::start()
{

    // http://stackoverflow.com/questions/10673585/start-thread-with-member-function
    std::thread tmp(&NetworkEntity::wait_new_messages, this);
    listening_thread = std::move(tmp);
    is_started = true;
}

void NetworkEntity::stop()
{
    if (is_started) {
        is_started = false;
        listening_thread.join();
    }
}

bool NetworkEntity::is_alive()
{
    return true; // TODO
}

bool NetworkEntity::is_me()
{
    return endpoint_addr == MY_HOST;
}

bool NetworkEntity::has_data()
{
    return p_socket->has_data();
}

NetworkEntity& operator<<(NetworkEntity &output_entity, const BaseMessage &message)
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
    std::shared_ptr<NetworkEntity> entity = std::make_shared<NetworkEntity>(NetworkEntity(p_client_socket, "todo", p_handler));  // TODO
    clients.push_back(entity);
    entity->start();
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

    for (unsigned i = 0; i < clients.size(); ++i) {
        clients[i]->stop();
    }

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
