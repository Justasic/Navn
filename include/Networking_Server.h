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

#ifndef Networking_Server_H
#define Networking_Server_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <vector>

#include "Networking_ServerClient.h"
#include "Networking_Exceptions.h"
#include "Networking_Multiplexer.h"
#include "log.h"
//#include "Networking_DNS.h"

class CoreExport Server : public virtual Socket
{
    unsigned short port;
    void AcceptClients()
    {
	struct sockaddr TmpClientAddr;
	socklen_t AddrSize = sizeof(TmpClientAddr);
	int afd = accept(this->GetFileDescriptor(), &TmpClientAddr, &AddrSize);
	if(afd == -1)
	    throw SocketException("Cannot accept client: "+Flux::string(strerror(errno)));

	if(afd > 0)
	    ;//new Client(afd, TmpClientAddr);
    }

public:
    DNSInstance *resolver;
    Server(int pn) : Socket(-1, AF_INET, SOCK_STREAM), port(pn)
    {
	struct sockaddr_in ServerAddress;
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = INADDR_ANY;
	ServerAddress.sin_port = htons(pn);
	resolver = new DNSInstance(*reinterpret_cast<struct sockaddr*>(&ServerAddress));
    }

    ~Server()
    {
// 	Log(LOG_RAWIO) << "Destroying Server socket";
    }

    bool ProcessRead()
    {
	// accept clients
	return true;
    }

    bool ProcessWrite()
    {
	// nothing needed
	return true;
    }

    void Bind()
    {
	const sockaddr *csa = const_cast<const sockaddr*>(this->resolver->GetSocketAddress());
	if(bind(this->GetFileDescriptor(), csa, sizeof(csa)) < 0)
	    throw SocketException("Could not bind to port: "+Flux::string(strerror(errno)));
    }

    void Listen(int BackLogMax = 5)
    {
	while(true)
	{
	    if(listen(this->GetFileDescriptor(), BackLogMax) != 0)
		throw SocketException("Could not listen to socket: "+Flux::string(strerror(errno)));

	    AcceptClients();
	}
    }
};


#endif