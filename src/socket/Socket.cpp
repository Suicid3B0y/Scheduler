// Implementation of the Socket class.

#include "Socket.h"


Socket::Socket() : m_sock{-1}, is_server_socket{false}
{
    memset(&m_addr, 0, sizeof(m_addr));
}

Socket::Socket(const std::string host, const unsigned port): Socket()
{
    if (!create()) {
        throw SocketException("Could not create client socket.");
    }

    if (!connect(host, port)) {
        throw SocketException("Could not bind to port.");
    }
}

Socket::Socket(const Socket &socket): Socket()
{
    (*this) = socket;
}

Socket& Socket::operator=(const Socket &socket)
{
    m_sock = socket.m_sock;
    m_addr = socket.m_addr;
    is_server_socket = socket.is_server_socket;

    return (*this);
}

Socket::~Socket()
{
}

bool Socket::is_valid() const
{
    return m_sock != -1;
}

// Internal methods

bool Socket::create()
{
    m_sock = socket(AF_INET, SOCK_STREAM, 0);

    if (!is_valid())
        return false;

    // TIME_WAIT - argh
    int on = 1;
    return (setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&on, sizeof(on)) != -1);
}

bool Socket::bind(const unsigned port)
{
    if (!is_valid())
        return false;

    m_addr.sin_family      = AF_INET;
    m_addr.sin_addr.s_addr = INADDR_ANY;
    m_addr.sin_port        = htons(port);

    return ::bind(m_sock, (struct sockaddr*)&m_addr, sizeof(m_addr)) != -1;
}

bool Socket::listen() const
{
    if (!is_valid())
        return false;

    int listen_return = ::listen(m_sock, MAXCONNECTIONS);

    return (listen_return != -1);
}

// Server side utils

void Socket::bind_to(const unsigned port)
{
    if (!create())
        throw SocketException("Could not create server socket.");
    if (!bind(port))
        throw SocketException("Could not bind to port.");
    if (!listen())
        throw SocketException("Could not listen to socket.");
}

bool Socket::accept(Socket& remote_socket) const
{
    int addr_length   = sizeof(m_addr);
    remote_socket.m_sock = ::accept(m_sock, (sockaddr*)&m_addr, (socklen_t*)&addr_length);

    return (remote_socket.m_sock > 0);
}

// Client side utils

bool Socket::connect(const std::string host, const unsigned port)
{
    if (!is_valid())
        return false;

    m_addr.sin_family = AF_INET;
    m_addr.sin_port   = htons(port);

    inet_pton(AF_INET, host.c_str(), &m_addr.sin_addr);

    if (errno == EAFNOSUPPORT)
        return false;

    return ::connect(m_sock, (sockaddr*)&m_addr, sizeof(m_addr)) == 0;
}

// Data transmission

bool Socket::has_data() const
{
    int count;
    ioctl(m_sock, FIONREAD, &count);

    return count > 0;
}

bool Socket::send(const std::string data) const
{
    if (::send(m_sock, data.c_str(), data.size(), MSG_NOSIGNAL) == -1) {
        debug("Error while sending data over the socket :: Errno: " << errno << std::endl);
        return false;
    } else {
        return true;
    }
}

bool Socket::recv(std::string& data) const
{
    char buf[MAXRECV + 1];

    data = "";

    memset(buf, 0, MAXRECV + 1);

    int nbbytes = ::recv(m_sock, buf, MAXRECV, 0);

    if (nbbytes == -1) {
        throw SocketException("Could not send to server.");
    } else if (nbbytes == 0) {
        return false;
    } else {
        data = std::string(buf, buf + nbbytes);
        return true;
    }
}

const Socket& Socket::operator<<(const std::string& data) const
{
    send(data);
    return (*this);
}

const Socket& Socket::operator>>(std::string& data) const
{
    recv(data);
    return (*this);
}

void Socket::set_non_blocking(const bool blocking)
{
    int opts = fcntl(m_sock, F_GETFL);

    if (opts >= 0) {
        if (blocking)
            opts = (opts | O_NONBLOCK);
        else
            opts = (opts & ~O_NONBLOCK);

        fcntl(m_sock, F_SETFL, opts);
    }
}

void Socket::close()
{
    if (is_valid())
        ::close(m_sock);
}
