/* Socket.h */

#ifndef Socket_class
#define Socket_class

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>

const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 5;
const int MAXRECV = 500;

class Socket
{
 public:
  Socket(const std::string server,const std::string port);
  ~Socket();

  bool get_address();
  bool connect();
  bool is_valid() const { return m_sock != -1; }
  const Socket& operator >> (std::string&) const;
  const Socket& operator << (const std::string&) const;
 
 private:
  const std::string server;
  const std::string port;

  int m_sock;
  struct addrinfo hints, *servinfo;

};
#endif