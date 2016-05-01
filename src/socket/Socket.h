#ifndef _H_SOCKET_SOCKET
#define _H_SOCKET_SOCKET


#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <memory>

#include "../scheduler.h"
#include "SocketException.h"


const int MAXHOSTNAME    = 200;
const int MAXCONNECTIONS = 5;
const int MAXRECV        = 500;


// Warning: do not forget to close sockets !
class Socket
{
private:
    int m_sock;
    sockaddr_in m_addr;
    bool is_server_socket;

protected:
    bool create();
    bool bind(const unsigned port);
    bool listen() const;

public:
    Socket(); // Initialization of a basic socket.
    Socket(const Socket &socket);
    Socket& operator=(const Socket &socket);
    Socket(const std::string host, const unsigned port);  // Initialize a socket and auto-connect to a remote endpoint
    virtual ~Socket();

    bool is_valid() const;

    // Server side utils
    void bind_to(const unsigned port);
    bool accept(Socket& remote_socket) const;

    // Client side utils
    bool connect(const std::string host, const unsigned port);

    // Socket configuration
    void set_non_blocking(const bool blocking);

    // Data Transimission
    bool has_data() const;
    bool send(const std::string data) const;
    bool recv(std::string& data) const;
    const Socket& operator<<(const std::string& data) const;
    const Socket& operator>>(std::string& data) const;

    void close();
};

#endif
