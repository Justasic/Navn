/* Socket.h */
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
#include "flux.h"
const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 5;
class SocketIO
{
private:
  std::string server;
  std::string port;
  int sockn;
  size_t recvlen;
  struct addrinfo hints, *servinfo;
public:
  SocketIO(const Flux::string &server, const Flux::string &port);
  ~SocketIO();
  bool GetBuffer(Flux::string&);
  bool get_address();
  const int recv() const;
  const int send(const Flux::string &buf) const;
  bool connect();
  bool is_valid()  const { return sockn != -1; }
};
/* just cuz i am hella lazy, i am defining them in the socket engine to make them global */
struct CommandSource;
void send_cmd(const char *fmt, ...); /* we make this global beyond flux_net_irc.hpp so we can send almost anywhere in navn */
extern SocketIO *sock;
void log(const char *fmt, ...);
void ProcessModules(CommandSource&, std::vector<Flux::string>&);
extern std::vector<Flux::string> StringVector(const Flux::string&, char);
#endif