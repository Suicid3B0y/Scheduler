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

#include "../constants.h"
#include "SocketException.h"


const int MAXHOSTNAME    = 200;
const int MAXCONNECTIONS = 5;
const int MAXRECV        = 500;


/**
 * \class Socket
 * \brief C socket encapsulation for C++
 *
 * Manage a socket entirely. Important note : do not forget to close a socket once the transmission is finished !
 */
class Socket
{
    private:
        int m_sock;  /*!< Socket file descriptor */
        sockaddr_in m_addr;  /*!< Structure containing the remote endpoint address */
        bool is_server_socket;  /*!< Boolean telling if the socket is a server socket or not */

    protected:
        /**
         * \brief Allocate necessary memory space for the socket structure
         */
        bool create();

        /**
         * \brief Bind the socket to a specific local port
         */
        bool bind(const unsigned port);

        /**
         * \brief Tell the system the socket is listening for a connection
         */
        bool listen() const;

    public:
        /**
         * \brief Socket constructor ; does not allocate memory for the socket
         */
        Socket();

        /**
         * \brief Copy constructor
         */
        Socket(const Socket &socket);

        /**
         * \brief Assignment operator
         */
        Socket& operator=(const Socket &socket);

        /**
         * \brief Constructor ; create the socket and try to connect to a remote socket
         */
        Socket(const std::string host, const unsigned port);  // Initialize a socket and auto-connect to a remote endpoint

        /**
         * \brief Destructor
         */
        virtual ~Socket();

        /**
         * \brief Check if the socket is valid, i.e. the socket is created.
         */
        bool is_valid() const;

        /**
         * \brief Server side util : bind to a local port the socket
         */
        void bind_to(const unsigned port);

        /**
         * \brief Server side util : accept for new connection.
         */
        bool accept(Socket& remote_socket) const;

        /**
         * \brief Client side util : connect to a remote endpoint.
         */
        bool connect(const std::string host, const unsigned port);

        /**
         * \brief Socket configuration : tell if the socket is in blocking state when accepting / receiving data.
         */
        void set_non_blocking(const bool blocking);

        /**
         * \brief Transmission util : check if the socket has data.
         */
        bool has_data() const;

        /**
         * \brief Transmission util : send data to the remote endpoint.
         */
        bool send(const std::string data) const;

        /**
         * \brief Transmission util : receive data from the remote endpoint.
         */
        bool recv(std::string& data) const;

        /**
         * \brief Transmission util : send data to the remote endpoint, using the << operator.
         */
        const Socket& operator<<(const std::string& data) const;

        /**
         * \brief Transmission util : receive ddata from the remote endpoint, using the >> operator.
         */
        const Socket& operator>>(std::string& data) const;

        /**
         * \brief Close the socket, unallocate the memory.
         */
        void close();
};

#endif
