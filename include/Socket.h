/* Navn IRC bot -- Socket engine header
 * 
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */
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
#include "log.h"
const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 5;
class CoreExport SocketIO
{
private:
  Flux::string server, port, ip;
  int sockn;
  bool ipv6;
  size_t recvlen;
public:
  SocketIO(const Flux::string &server, const Flux::string &port);
  ~SocketIO();
  int GetFD() const;
  bool IsIPv6() const;
  bool SetNonBlocking();
  bool SetBlocking();
  int recv() const;
  int send(const Flux::string buf) const;
  bool Connect();
  bool is_valid() const { return this->GetFD() != -1; }
  int Process();
  bool Read(const Flux::string&) const;
  void ThrowException(const Flux::string&);
};
#endif