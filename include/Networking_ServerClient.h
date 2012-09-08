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

#ifndef Networking_ServerClient_H
#define Networking_ServerClient_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <strings.h>
#include <netdb.h>
#include <netinet/in.h>

#include "Networking_Multiplexer.h"
#include "Networking_TCPClient.h"
#include "log.h"
/*
// TODO: Finish this class and the server class
class CoreExport Client : public TCPSocket
{
public:
    Client(int FD, struct sockaddr address) : TCPSocket(FD, (int)address.sa_family, (int)SOCK_STREAM)
    {
	this->resolver = new DNSInstance(address);
    }

    ~Client()
    {
	Log(LOG_RAWIO) << "Client with file descriptor " << this->GetFileDescriptor() << " was destroyed\n";
    }

    bool ProcessRead()
    {
	return false; // fail because this socket isnt finished
    }

    bool ProcessWrite()
    {
	return false; // fail because this socket isnt finished
    }

    void ProcessError()
    {

    }

    bool Read(const Flux::string &buffer)
    {
	printf("Message from %s: %s\n", "SocketAddressASCII().c_str()", buffer.c_str());
	Send("I got your message!");
	return true;
    }
};*/
#endif