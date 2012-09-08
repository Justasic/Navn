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
#ifndef NETWORKING_MULTIPLEXER_H
#define NETWORKING_MULTIPLEXER_H
#include <iostream>
#include <exception>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <cstring>
#include <errno.h>
#include <netdb.h>
#include <map>
#include "flux.h"

//#define BUFSIZE 2048

// Windows thinks it's better to call their api "closesocket" instead of
// just being like everyone else and calling it "close"
#ifdef _WIN32
# define close closesocket
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
# include <winsock2.h>
# include <ws2tcpip.h>
# pragma comment(lib, "ws2_32.lib")
// Windows crap
class CoreExport WSAInstance : public Base
{
    WSADATA data;
    WSABUF databuf;
public:
    WSAInstance()
    {
	int iResult = 0;
	data = { 0 };
	WSAStartup(MAKEWORD(2, 2), &data);
	if(iResult != 0)
	    throw SocketException("Could not initialize Windows Sockets");
    }

    ~WSAInstance()
    {
	WSACleanup();
    }
};
#endif

enum SocketFlag
{
    SF_DEAD       = 0x00000001,
    SF_WRITABLE   = 0x00000002,
    SF_CONNECTING = 0x00000004,
    SF_CONNECTED  = 0x00000008,
    SF_ACCEPTING  = 0x00000010,
    SF_ACCEPTED   = 0x00000020
};

class CoreExport Socket : public Base
{
protected:
    // Hold the file descriptor
    int internalfd;
    // Hold the kind of socket we are (ipv4? ipv6?)
    int protocol;
    // Store how many bytes were read
    size_t bytesread;
    // Store how many bytes were sent
    size_t bytessent;
    // Hold the lifetime of the socket
    time_t lifetime;
    // Socket Address


    //inline void SetSocketAddress(struct sockaddr &sd) { SocketAddress = sd; }
    // private constructor
    Socket();
public:
    // Create a new socket if the fd less than -1
    Socket(int fd, int Domain, int Type, int Protocol = 0);

    // Close our open socket before we destroy ourselves
    virtual ~Socket();

    // Hold our bit flags we have
    int flags;

    // -- Getters and socket status --
    // set the socket as being dead for the socket handler to destroy the socket
    inline void SetDead(bool status) { status ? (this->flags |= SF_DEAD) : (this->flags &= ~SF_DEAD); this->DeathNotify(); }
    // check if the socket is dead
    inline bool IsDead() const { return (this->flags & SF_DEAD); }
    // Check whether it's ipv6 or ipv4
    inline bool IsIPv6() const { return (this->protocol == AF_INET6); }
    // Get the file descriptor
    inline int GetFileDescriptor() const { return this->internalfd; }
    // Get our protocol (AF_INET || AF_INET6)
    inline int GetProtocol() const { return this->protocol; }

    // -- Socket Statistical Data --
    // Get how many bytes were read
    inline size_t GetBytesRead() const { return this->bytesread; }
    // Get how many bytes were sent
    inline size_t GetBytesSent() const { return this->bytessent; }
    // How long the socket has been alive
    inline time_t GetLifetime() const { return this->lifetime; }

    // -- Socket methods --
    // Send stuff to the client
    int InternalSend(const Flux::string &message);
    // Receive stuff from the client
    int InternalReceive(Flux::string &message);

    // Allow the socket to block the program?
    bool SetBlocking();
    bool SetNonBlocking();

    // Our actual read/write functions which are called
    // every time the multiplexer allows the program to
    // iterate.
    virtual bool ProcessRead() = 0;
    virtual bool ProcessWrite() = 0;
    virtual bool Process() { return true; }
    virtual void ProcessError() { }

    // Notifications
    virtual void DeathNotify() { }
    virtual void OnError(const Flux::string&) { }

};

// Our basic class for handing the sockets in the multiplexer
class CoreExport SocketHandler
{
public:
    static std::map<int, Socket*> Sockets;
    // Initialize the socket handler.
    static void Initialize();
    // Process all the sockets created.
    static void ProcessSockets();
    // Set the socket as writable so we can send through the multiplexer
    static void SetWritable(Socket*);
    // Clear the writable status
    static void ClearWritable(Socket*);
    // Add a socket to handle
    static void AddSocket(Socket*);
    // Remove a socket to handle
    static void RemoveSocket(Socket*);
    // Process the last bit left that's set writable and close all sockets
    static void Shutdown();
    // Check if a file descriptor is valid
    static bool IsValidFD(int);
};

#endif