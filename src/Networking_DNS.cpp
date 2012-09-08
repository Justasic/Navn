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
#include "Networking_DNS.h"
#include "Networking_Exceptions.h"
#include "log.h"

DNSInstance::DNSInstance(struct sockaddr addr) : sa(addr)
{
    // Clear our structs before use
    memset(&sa4, 0, sizeof(sa4));
    memset(&sa6, 0, sizeof(sa6));

    if(sa.sa_family == AF_INET)
	sa4 = *reinterpret_cast<struct sockaddr_in*>(&sa);
    else
	sa6 = *reinterpret_cast<struct sockaddr_in6*>(&sa);
}

DNSInstance::DNSInstance(const Flux::string &address, short port)
{
    if(!address.empty())
	this->ResolveInternal(address, port);
}
/* Used for specifying an already resolved IP */
// DNSInstance::DNSInstance(const Flux::string &address, int port)
// {
//     if(!address.empty())
//     {
// 	if(address.find(":") != Flux::string::npos && !address.find_first_not_of("0123456789:"))
// 	    this->InternalPton(AF_INET6, address, port);
// 	else if(address.find(".") != Flux::string::npos && !address.find_first_not_of("0123456789."))
// 	    this->InternalPton(AF_INET, address, port);
//     }
// }


void DNSInstance::ResolveInternal(const Flux::string &hostname, short port)
{
    struct addrinfo *result, *res;
    SocketAddresses.clear();
    int err = getaddrinfo(hostname.c_str(), NULL, NULL, &result);

    if(err != 0)
    {
	Log(LOG_WARN) << "Failed to resolve " << hostname << ": " << gai_strerror(err);
	return;
    }

    for(res = result; res != NULL; res = res->ai_next)
    {
	struct sockaddr *haddr;
	haddr = res->ai_addr;
	char address[INET6_ADDRSTRLEN + 1] = "\0";
	// Check to make sure it resolves correctly
	switch(haddr->sa_family)
	{
	    case AF_INET:
		struct sockaddr_in *v4;
		v4 = reinterpret_cast<struct sockaddr_in*>(haddr);

		if (!inet_ntop(AF_INET, &v4->sin_addr, address, sizeof(address)))
		{
		    Log(LOG_RAWIO) << "DNS: " << strerror(errno);
		    return;
		}
		break;
	    case AF_INET6:
		struct sockaddr_in6 *v6;
		v6 = reinterpret_cast<struct sockaddr_in6*>(haddr);

		if (!inet_ntop(AF_INET6, &v6->sin6_addr, address, sizeof(address)))
		{
		    Log(LOG_RAWIO) << "DNS: " << strerror(errno);
		    return;
		}
		break;
	}
	SocketAddresses[address] = haddr;
    }

    sa = *SocketAddresses.begin()->second;
    this->InternalPton(sa.sa_family, SocketAddresses.begin()->first, port);

    freeaddrinfo(result);
}

void DNSInstance::InternalPton(int type, const Flux::string &address, short port)
{
    int i = 0;
    switch (type)
    {
	case AF_INET:
	    i = inet_pton(type, address.c_str(), &sa4.sin_addr);

	    Log(LOG_TERMINAL) << "IP: " << address;
	    if (i == 0)
		throw SocketException("Invalid host (" + address + ")", ExceptionLocation);
	    else if (i <= -1)
		throw SocketException("Invalid host: " + Flux::string(strerror(errno)) + " (" + address + ")");

	    sa4.sin_family = type;
	    sa4.sin_port = htons(port);
	    return;
	case AF_INET6:
	    i = inet_pton(type, address.c_str(), &sa6.sin6_addr);

	    if (i == 0)
		throw SocketException("Invalid host (" + address + ")", ExceptionLocation);
	    else if (i <= -1)
		throw SocketException("Invalid host: " + Flux::string(strerror(errno)) + " (" + address + ")");

	    sa6.sin6_family = type;
	    sa6.sin6_port = htons(port);
	    return;
	default:
	    break;
    }

    throw SocketException("Invalid socket type", ExceptionLocation);
}

// Get the ip address
Flux::string DNSInstance::GetIP() const
{
    char address[INET6_ADDRSTRLEN + 1] = "\0";

    switch (sa.sa_family)
    {
	case AF_INET:
	    if (!inet_ntop(AF_INET, &sa4.sin_addr, address, sizeof(address)))
		throw SocketException(strerror(errno), ExceptionLocation);
	    break;
	case AF_INET6:
	    if (!inet_ntop(AF_INET6, &sa6.sin6_addr, address, sizeof(address)))
		throw SocketException(strerror(errno), ExceptionLocation);
	    break;
	default:
	    break;
    }

    return address;
}

// Get the port used
unsigned short DNSInstance::GetPort() const
{
    switch (sa.sa_family)
    {
	case AF_INET:
	    return ntohs(sa4.sin_port);
	case AF_INET6:
	    return ntohs(sa6.sin6_port);
	default:
	    break;
    }

    return -1;
}