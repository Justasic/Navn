/* Socket.h */
/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
/**
 *\file  Socket.h 
 *\brief Socket header for Socket.cpp
 */
#ifndef SOCKET
#define SOCKET

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <fstream>
#include <iostream>
#include <istream>
#include <cstdarg>
#include <ostream>
#include <queue>
#include <arpa/inet.h>
#include "extern.h"
const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 5;
class SocketIO
{
private:
  std::string server, port;
  int sockn;
  size_t recvlen;
  struct addrinfo hints, *servinfo;
public:
  SocketIO(const Flux::string &server, const Flux::string &port);
  ~SocketIO();
  void get_address();
  int GetFD() const;
  bool SetNonBlocking();
  bool SetBlocking();
  const int recv() const;
  const int send(const Flux::string &buf) const;
  bool Connect();
  bool is_valid() const { return this->GetFD() != -1; }
  int Process();
  bool Read(const Flux::string&) const;
};
#endif