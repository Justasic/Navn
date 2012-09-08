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
#pragma once
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <string>
#include <cstring>
#include <errno.h>
#include "flux.h"

class CoreExport DNSInstance : public Base
{
protected:
    // Internal variables
    struct sockaddr sa;
    struct sockaddr_in sa4;
    struct sockaddr_in6 sa6;
    std::map<Flux::string, struct sockaddr*> SocketAddresses;

    void ResolveInternal(const Flux::string&, short);
    void InternalPton(int, const Flux::string&, short = 0);

public:
    DNSInstance(struct sockaddr addr);
//     DNSInstance(const Flux::string &address);
    DNSInstance(const Flux::string &address, short port);

    // Get the ip address
    Flux::string GetIP() const;
    // Get the port used
    unsigned short GetPort() const;

    // Get all resolved addresses
    inline std::map<Flux::string, struct sockaddr*> GetAddresses() const
    {
	return this->SocketAddresses;
    }

    // Get various socket address structs
    inline struct sockaddr *GetSocketAddress()
    {
	return &this->sa;
    }

    inline struct sockaddr_in *GetSocketAddress4()
    {
	return &this->sa4;
    }

    inline struct sockaddr_in6 *GetSocketAddress6()
    {
	return &this->sa6;
    }
};