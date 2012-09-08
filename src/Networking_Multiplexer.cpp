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
#include "Networking_Multiplexer.h"
#include "Networking_Exceptions.h"
#include <fcntl.h>
#include <errno.h>
#include <cstdio>
#include <cstring>
#include "flux.h"
#include "log.h"

// Base Socket Class

// This is private. We want an inherited socket, not a blank socket.
Socket::Socket() { }
// Create a new socket if the fd less than -1
Socket::Socket(int sfd, int Domain, int Type, int Protocol) : internalfd(0), protocol(Domain), bytesread(0), bytessent(0), lifetime(time(NULL)), flags(0)
{
//     memset(&SocketAddress, 0, sizeof(SocketAddress));
    top:
    if(sfd == -1)
	//internalfd = socket(Domain, Type, Protocol);
	internalfd = socket(AF_INET, SOCK_STREAM, 0);
    else
	internalfd = sfd;

    printf("File descriptor %d is %svalid!\n", this->GetFileDescriptor(), SocketHandler::IsValidFD(internalfd) ? "" : "NOT ");

    if(!SocketHandler::IsValidFD(internalfd))
	goto top;

    // oh noes! our descriptor wasn't created!
    if(internalfd < 0)
	throw SocketException("Could not create socket: "+Flux::string(strerror(errno)));

    Log(LOG_RAWIO) << "Adding new socket " << this->GetFileDescriptor() << " to socket engine.";

    SocketHandler::AddSocket(this);
}

// Close our open socket before we destroy ourselves
Socket::~Socket()
{
    Log(LOG_RAWIO) << "Destroying socket base. FD: " << this->GetFileDescriptor();
    close(this->GetFileDescriptor());
    try
    {
	SocketHandler::RemoveSocket(this);
    }
    catch(const SocketException &ex)
    {
	ex.Print();
    }
}

// Send stuff to the client
int Socket::InternalSend(const Flux::string &message)
{
    size_t sendbytes = ::send(this->GetFileDescriptor(), message.c_str(), message.size(), 0);
    Log(LOG_RAWIO) << "Sent: " << message;
    this->bytessent += sendbytes;
    return sendbytes;
}
// Receive stuff from the client
int Socket::InternalReceive(Flux::string &message)
{
    char buffer[BUFSIZE] = "\0";
    size_t recvbytes = recv(this->GetFileDescriptor(), buffer, BUFSIZE - 1, 0);
    message = buffer;
    this->bytesread += recvbytes;
    return recvbytes;
}

// Allow the socket to block the program?
bool Socket::SetBlocking()
{
#ifdef _WIN32
    unsigned long iMode = 0;
    ioctlsocket(fd, FIONBIO, &iMode);
#else
    int sflags = fcntl(this->GetFileDescriptor(), F_GETFL, 0);
    return !fcntl(this->GetFileDescriptor(), F_SETFL, sflags & ~O_NONBLOCK);
#endif
}

bool Socket::SetNonBlocking()
{
#ifdef _WIN32
    unsigned long iMode = 1;
    ioctlsocket(fd, FIONBIO, &iMode);
#else
    int sflags = fcntl(this->GetFileDescriptor(), F_GETFL, 0);
    return !fcntl(this->GetFileDescriptor(), F_SETFL, sflags | O_NONBLOCK);
#endif
}

// Check if the file descriptor is valid
bool SocketHandler::IsValidFD(int fd)
{
    return (fcntl(fd, F_GETFL) != -1 || errno != EBADF);
}

// Socket Handler is in another cpp file.