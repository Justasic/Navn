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
// This is the Poll implementation of the SocketHandler

#include <sys/time.h> // Used for determining the blocking time of the program
#include <unistd.h>
#include <errno.h>
#include <cstdio>
#include <cstring>
#include "Networking_Multiplexer.h"
#include "log.h"

#ifndef _WIN32
# include <sys/poll.h>
# include <poll.h>
# include <sys/types.h>
# include <sys/time.h>
# include <sys/resource.h>
# ifndef POLLRDHUP
#  define POLLRDHUP 0
# endif
#else
# define poll WSAPoll
# define POLLRDHUP POLLHUP
WSAInstance *winsocketinstance;
#endif

static long max;
static pollfd *events;
static std::map<int, int> socket_positions;

void SocketHandler::Initialize()
{
    #ifdef _WIN32
    WSAInstance *winsockinstance;
    #endif
    rlimit fd_limit;
    if(getrlimit(RLIMIT_NOFILE, &fd_limit) == -1)
	throw SocketException(strerror(errno), ExceptionLocation);

    max = fd_limit.rlim_cur;
    events = new pollfd[max];
    Log(LOG_RAWIO) << "[SocketHandler] Using Poll Socket Handler with " << max << " max sockets";
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
    #ifdef _WIN32
    delete winsockinstance;
    #endif
}

// Set the socket as writable so we can send through the multiplexer
void SocketHandler::SetWritable(Socket *s)
{
    if(s && !(s->flags & SF_WRITABLE))
    {
	std::map<int, int>::iterator pos = socket_positions.find(s->GetFileDescriptor());
	if(pos == socket_positions.end())
	    throw SocketException("Unable to set socket as writable for socket handler: "+strerror(errno));

	pollfd *ev = &events[pos->second];
	ev->events |= POLLOUT;

	s->flags |= SF_WRITABLE;
    }
}

// Clear the writable status
void SocketHandler::ClearWritable(Socket *s)
{
    if(s && (s->flags & SF_WRITABLE))
    {
	std::map<int, int>::iterator pos = socket_positions.find(s->GetFileDescriptor());
	if(pos == socket_positions.end())
	    throw SocketException("Unable to clear socket as writable for socket handler: "+strerror(errno));

	pollfd *ev = &events[pos->second];
	ev->events & ~POLLOUT;

	s->flags &= ~SF_WRITABLE;
    }
}

// Add a socket to handle
void SocketHandler::AddSocket(Socket *s)
{
    if(static_cast<int>(socket_positions.size()) == max)
	throw SocketException("Cannot add another socket to Socket Handler: "+strerror(errno));

    pollfd *ev = &events[socket_positions.size()];
    ev->fd = s->GetFileDescriptor();
    ev->events = POLLIN;
    ev->revents = 0;

    socket_positions[ev->fd] = socket_positions.size();
    SocketHandler::Sockets[ev->fd] = s;
}

// Remove a socket to handle
void SocketHandler::RemoveSocket(Socket *s)
{
    std::map<int, int>::iterator pos = socket_positions.find(s->GetFileDescriptor());
    if(pos == socket_positions.end())
	throw SocketException("Unable to set socket as writable for socket handler: "+strerror(errno));

    if(pos->second != socket_positions.size() - 1)
    {
	pollfd *ev = &events[pos->second], *last_ev = &events[socket_positions.size() - 1];

	ev->fd = last_ev->fd;
	ev->events = last_ev->events;
	ev->revents = last_ev->revents;

	socket_positions[ev->fd] = pos->second;
    }

    SocketHandler::Sockets.erase(s->GetFileDescriptor());
    socket_positions.erase(pos);
}

// Process all the sockets created.
void SocketHandler::ProcessSockets()
{
    int TotalFDs = poll(events, socket_positions.size(), 5 * 1000);

    if(TotalFDs == -1 && errno != EINTR)
    {
	// Don't throw because the error could be temporary!
	Log(LOG_WARN) << "[SocketHandler] Socket Handler error: " << strerror(errno);
	return;
    }

    for(int i = 0, processed = 0; i < socket_positions.size() && processed != TotalFDs; ++i)
    {
	pollfd *ev = &events[i];

	if(ev->revents != 0)
	    ++processed;

	std::map<int, Socket*>::iterator it = Sockets.find(ev->data.fd);
	if(it = Sockets.end())
	    continue;
	Socket *s = it->second;

	if(ev->events & (POLLRDHUP | POLLERR))
	{
	    s->ProcessError();
	    delete s;
	    continue;
	}

	if(!s->Process())
	    continue;

	if((ev->events & POLLIN) && !s->ProcessRead())
	    s->SetDead(true);

	if((ev->events & POLLOUT) && !s->ProcessWrite())
	    s->SetDead(true);

	if (s->IsDead())
	{
	    Log(LOG_RAWIO) << "[SocketHandler] Socket " << s->GetFileDescriptor() << " died!\n";
	    delete s;
	}
    }
}