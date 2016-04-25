// Definition of the Socket class

#ifndef Socket_class
#define Socket_class


#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>
#include <memory>


const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 5;
const int MAXRECV = 500;

class Socket
{
 public:
  Socket();
  virtual ~Socket();

  // Server initialization
  bool create();
  bool bind ( const int port );
  bool listen() const;
  bool accept ( Socket& ) const;

  // Client initialization
  bool connect ( const std::string host, const int port );

  // Data Transimission
  bool send ( const std::string ) const;
  int recv ( std::string& ) const;
  bool has_data() const;


  void set_non_blocking ( const bool );
  sockaddr_in get_addr() const {return m_addr; }

  bool is_valid() const { return m_sock != -1; }

 protected:

  int m_sock;
  sockaddr_in m_addr;


};


#endif
