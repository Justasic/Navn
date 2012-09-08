/*
 * Simple C++ Socket wrapper
 * The following people authored this code:
 *
 * Justin Crawford (Justasic@gmail.com)
 * Piero Barbagelata
 *
 * This project is not licensed and falls under
 * the Creative Commons Public Domain.
 * You are free to use and/or distribute this
 * source code for any project or organization
 * without authors consent.
 *
 */

#ifndef Networking_TCPClient_H
#define Networking_TCPClient_H

// Basic input/output
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include <strings.h>

// Socket stuff.
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <queue> // std::queue
#include <errno.h> // errno

#include "Networking_Exceptions.h"
#include "Networking_Multiplexer.h"
#include "Networking_DNS.h"

  // TODO: make a buffered socket class that can be inherited
  // this class is really just to (*died*)

class CoreExport TCPSocket : public virtual Socket
{
protected:
    std::queue<Flux::string> Buffer;
    unsigned short port;

public:
    // Constructors
    TCPSocket(int FileDescriptor, int iptype, int socktype);// : Socket(FileDescriptor, iptype, socktype);
    TCPSocket(const Flux::string &addr, unsigned short pn, bool ipv6 = false);

    // Destructor
    virtual ~TCPSocket();

    // DNS Resolver
    DNSInstance *resolver;

    // Required methods
    Flux::string SocketAddressToString();
    inline unsigned short GetPort() const { return this->port; }
    virtual void Connect();

    // SocketHandler required methods
    virtual bool ProcessRead();
    virtual bool ProcessWrite();
    virtual void ProcessError();
    virtual bool Process();

    // Sending/Receiving from the socket
    virtual void Send(const Flux::string&);
    virtual bool Read(const Flux::string&);

    // Statistical info
    inline size_t GetBufferSize() const { return this->Buffer.size(); }

    // Notifications
    virtual void ConnectNotify() { }
};
#endif