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
// This is the EPoll implementation of the SocketHandler
#include <sys/epoll.h> // Include the multiplexer
#include <ulimit.h>
#include <sys/time.h> // Used for determining the blocking time of the program
#include <unistd.h>
#include <errno.h>
#include <cstdio>
#include <cstring>
#include "Networking_Multiplexer.h"
#include "log.h"

// These are epoll only variables, therefore we cannot include them into the SocketHandler class
static long max;
static int EPollFD;
static epoll_event *events;

std::map<int, Socket*> SocketHandler::Sockets;

void SocketHandler::Initialize()
{
    max = ulimit(4, 0);

    if (max <= 0)
	throw SocketException("Can't determine maximum number of open sockets", ExceptionLocation);

    EPollFD = epoll_create(max / 4);

    if (EPollFD == -1)
	throw SocketException("Could not initialize epoll socket handler: " + Flux::string(strerror(errno)));

    events = new epoll_event[max];
    memset(events, 0, sizeof(epoll_event) * max);
    Log(LOG_RAWIO) << "[SocketHandler] Using EPoll Socket Handler with " << max << " max sockets";
}

// Process the last bit left that's set writable and close all sockets
void SocketHandler::Shutdown()
{
    SocketHandler::ProcessSockets();

    for (std::map<int, Socket*>::const_iterator it = SocketHandler::Sockets.begin(), it_end = SocketHandler::Sockets.end(); it != it_end;)
    {
	Socket *s = it->second;
	++it;
	delete s;
    }
    SocketHandler::Sockets.clear();

    delete[] events;
}

// Set the socket as writable so we can send through the multiplexer
void SocketHandler::SetWritable(Socket *s)
{
    if(s && !(s->flags & SF_WRITABLE))
    {
	epoll_event ev;
	memset(&ev, 0, sizeof(ev));

	ev.events = EPOLLIN | EPOLLOUT;
	ev.data.fd = s->GetFileDescriptor();

	if(epoll_ctl(EPollFD, EPOLL_CTL_ADD, ev.data.fd, &ev) == -1)
	    //throw SocketException("Unable to set socket as writable for socket handler: "+Flux::string(strerror(errno)));
	Log(LOG_WARN) << "Unable to set writable socket: " << strerror(errno);

	s->flags |= SF_WRITABLE;
    }
}

// Clear the writable status
void SocketHandler::ClearWritable(Socket *s)
{
    if(s && (s->flags & SF_WRITABLE))
    {
	epoll_event ev;
	memset(&ev, 0, sizeof(ev));

	ev.events = EPOLLIN;
	ev.data.fd = s->GetFileDescriptor();

	if(epoll_ctl(EPollFD, EPOLL_CTL_ADD, ev.data.fd, &ev) == -1)
	    throw SocketException("Unable to clear socket as writable for socket handler: "+Flux::string(strerror(errno)));

	s->flags &= ~SF_WRITABLE;
    }
}

// Add a socket to handle
void SocketHandler::AddSocket(Socket *s)
{
    epoll_event ev;
    memset(&ev, 0, sizeof(ev));

    ev.events = EPOLLIN;
    ev.data.fd = s->GetFileDescriptor();

    if(epoll_ctl(EPollFD, EPOLL_CTL_ADD, ev.data.fd, &ev) == -1)
    {
	Log(LOG_WARN) << "Unable to add socket: " << strerror(errno);
    }
	//throw SocketException("Unable to add socket to socket handler: "+Flux::string(strerror(errno)));

    SocketHandler::Sockets[ev.data.fd] = s;
}

// Remove a socket to handle
void SocketHandler::RemoveSocket(Socket *s)
{
    epoll_event ev;
    memset(&ev, 0, sizeof(ev));

    ev.data.fd = s->GetFileDescriptor();

    if(epoll_ctl(EPollFD, EPOLL_CTL_ADD, ev.data.fd, &ev) == -1)
	throw SocketException("Unable to remove socket from socket handler: "+Flux::string(strerror(errno)));

    SocketHandler::Sockets.erase(ev.data.fd);
}

void PrintFlags(Socket *s)
{
    Flux::string totalflags;
    if(s->flags & SF_DEAD)
	totalflags += "SF_DEAD ";

    if(s->flags & SF_CONNECTING)
	totalflags += "SF_CONNECTING ";

    if(s->flags & SF_CONNECTED)
	totalflags += "SF_CONNECTED ";

    if(s->flags & SF_ACCEPTING)
	totalflags += "SF_ACCEPTING ";

    if(s->flags & SF_ACCEPTED)
	totalflags += "SF_ACCEPTED ";

    if(s->flags & SF_WRITABLE)
	totalflags += "SF_WRITABLE ";

    totalflags.trim();
    Log(LOG_TERMINAL) << "Socket " << s->GetFileDescriptor() << " has flags: " << totalflags << " (" << s->flags << ")";

}

// Process all the sockets created.
void SocketHandler::ProcessSockets()
{
    int TotalFDs = epoll_wait(EPollFD, events, max - 1, 5 * 1000);

    if(TotalFDs == -1 && errno != EINTR)
    {
	// Don't throw because the error could be temporary!
	Log(LOG_WARN) << "[SocketHandler] Socket Handler error: " << strerror(errno);
	return;
    }

    for(int i = 0; i < TotalFDs; ++i)
    {
	epoll_event *ev = &events[i];

	std::map<int, Socket*>::iterator it = Sockets.find(ev->data.fd);
	if(it == Sockets.end())
	    continue;
	Socket *s = it->second;

	if(ev->events & (EPOLLHUP | EPOLLERR))
	{
	    s->ProcessError();
	    delete s;
	    continue;
	}

	PrintFlags(s);

	if(!s->Process())
	    continue;

	if((ev->events & EPOLLIN) && !s->ProcessRead())
	    s->SetDead(true);

	if((ev->events & EPOLLOUT) && !s->ProcessWrite())
	    s->SetDead(true);

	if (s->IsDead())
	{
	    Log(LOG_RAWIO) << "[SocketHandler] Socket " << s->GetFileDescriptor() << " died!\n";
	    delete s;
	}
    }
}
