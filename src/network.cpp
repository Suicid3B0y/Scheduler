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

NetworkEntity::NetworkEntity() :
        socket{}, handler{},
        endpoint_addr{MY_HOST}, port{},
        local_buffer{}, is_started{false}
{
}

NetworkEntity::NetworkEntity(const NetworkEntity &entity) :
        NetworkEntity(entity.socket, entity.handler, entity.endpoint_addr, entity.port)
{
}

NetworkEntity::~NetworkEntity()
{
    if (is_started) {
        is_started = false;
        listening_thread.join();
    }
}

NetworkEntity& NetworkEntity::operator=(const NetworkEntity &entity)
{
    socket = entity.socket;
    endpoint_addr = entity.endpoint_addr;
    handler = entity.handler;
    local_buffer = entity.local_buffer;

    return (*this);
}

NetworkEntity::NetworkEntity(const Socket &socket, const MessageHandler &handler, const std::string endpoint_addr, const unsigned port)
        : NetworkEntity()
{
    this->socket = socket;
    this->handler = handler;
    this->endpoint_addr = endpoint_addr;
    this->port = port;
}

unsigned NetworkEntity::get_message_length(int timeout)
{
    std::string data = local_buffer;
    local_buffer = "";

    while (data.length() < 4) {
        std::string tmp;
        socket >> tmp;
        data += tmp;
    }

    if (data.length() > 4)
        local_buffer = data.substr(4);

    unsigned sum = 0;
    for (unsigned i = 0; i < 4; ++i)
        sum += (int)data[i] << (8 * (3 - i));

    return sum;
}

void NetworkEntity::wait_new_messages()
{
    try {
        while (is_started) {
            if (has_data()) {
                BaseMessage message{get_message(0)};
                handler.handle_message(message);
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
    std::thread tmp(&NetworkEntity::wait_new_messages, this);
    listening_thread = std::move(tmp);
    is_started = true;
}

void NetworkEntity::close()
{
    if (is_started) {
        is_started = false;
        listening_thread.join();
    }

    socket.close();
}

BaseMessage NetworkEntity::get_message(int timeout)
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

    BaseMessage m{data};
    return m;
}

bool NetworkEntity::is_alive()
{
    return is_started; // FIXME: devrait vérifier que la socket est toujours valide.
}

bool NetworkEntity::is_me()
{
    return endpoint_addr == MY_HOST;
}

bool NetworkEntity::has_data()
{
    return socket.has_data();
}

NetworkEntity& operator<<(NetworkEntity &output_entity, const BaseMessage &message)
{
    output_entity.socket << message.encoded_message_length() << message;
    return output_entity;
}

// ----------------------------------------------------------------------------


NetworkServer::NetworkServer(const unsigned short port, MessageHandler &handler)
        : server{}, clients{}, handler{handler}, is_alive{false}
{
    server.bind_to(port);
}

void NetworkServer::do_accept()
{
    server.set_non_blocking(true);

    while (is_alive) {
        Socket client_socket;
        if (server.accept(client_socket)) {
            handle_accept(client_socket);
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}

void NetworkServer::handle_accept(Socket &client_socket)
{
    std::unique_ptr<NetworkEntity> entity = std::make_unique<NetworkEntity>(NetworkEntity{client_socket, handler, "todo", 42});  // TODO
    clients.push_back(std::move(entity));
    clients.back()->start();
}

void NetworkServer::start()
{
    is_alive = true;
    std::thread tmp(&NetworkServer::do_accept, this);
    accept_thread = std::move(tmp);
}

void NetworkServer::stop()
{
    is_alive = false;

    accept_thread.join();
}

void NetworkServer::close()
{
    for (unsigned i = 0; i < clients.size(); ++i) {
        clients[i]->close();
    }

    clients.empty();
}

NetworkEntity NetworkServer::connect_to(const std::string endpoint_addr, const unsigned short port)
{
    Socket socket{endpoint_addr, port};

    std::unique_ptr<NetworkEntity> entity{new NetworkEntity{socket, handler, endpoint_addr, port}};
    clients.push_back(std::move(entity));
    entity->start();

    return NetworkEntity(*entity); // Returns a copy ; we do not want to modify the original in any cases
}

std::vector<NetworkEntity> NetworkServer::get_clients()
{
    std::vector<NetworkEntity> res;
    res.reserve(clients.size());

    for (unsigned i = 0; i < clients.size(); ++i)
        res.push_back(NetworkEntity(*clients[i]));

    return res;
}