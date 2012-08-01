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
#pragma once
#ifndef SOCKET_H
#define SOCKET_H
#include <sys/types.h>

#include <errno.h>
#include <fstream>
#include <iostream>
#include <istream>
#include <cstdarg>
#include <ostream>
#include <queue>

#include "extern.h"
#include "log.h"
class CoreExport SocketIO : public Base
{
private:
  Flux::string server, port, ip, LastBuf;
  int sockn;
  bool ipv6;
  size_t recvlen;
  std::queue<Flux::string> WriteBuffer;
public:
  SocketIO(const Flux::string &server, const Flux::string &port);
  ~SocketIO();
  inline int GetFD() const { return sockn; }
  inline bool IsIPv6() const { return this->ipv6; }
  inline bool is_valid() const { return this->GetFD() != -1; }
  inline size_t GetReceiveLen() const { return this->recvlen; }
  inline Flux::string GetLastBuf() const { return this->LastBuf; };
  bool SetNonBlocking();
  bool SetBlocking();
  void send(const Flux::string &buf);
  void Connect();
  void Process();
  bool Read(const Flux::string&) const;
  void ThrowException(const Flux::string&);
};
#endif
