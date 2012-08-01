/* Navn IRC bot -- Windows socket shit
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */

#pragma once
#ifndef WINDOWS_SOCKET_H
#define WINDOWS_SOCKET_H
#ifdef _WIN32
#include "flux.h"

extern CoreExport int read(int, char*, size_t);
extern CoreExport int write(int, const char*, size_t);
extern CoreExport int windows_close(int);
extern CoreExport int windows_accept(int, struct sockaddr*, int*);
extern CoreExport int windows_inet_pton(int, const char*, void*);
extern CoreExport const char *windows_inet_ntop(int, const void*, char*, size_t);
extern CoreExport int fcntl(int, int, int);

// Windows socket bullshit
class CoreExport WSAInitialization : public Base
{
public:
	WSADATA wsa;
	WSAInitialization()
	{
		if(WSAStartup(MAKEWORD(2, 0), &wsa))
			throw CoreException("Failed to initialize WinSock library");
	}

	~WSAInitialization()
	{
		WSACleanup();
	}
};

#endif // _WIN32
#endif // WINDOWS_SOCKET_H