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
// This is the Kqueue implementation of the SocketHandler
#include <sys/time.h> // Used for determining the blocking time of the program
#include <unistd.h>
#include <errno.h>
#include <cstdio>
#include <cstring>
#include "Networking_Multiplexer.h"
#include "log.h"

#include <sys/types.h>
#include <sys/event.h>

static int kq_fd, max_fds, change_count;
static struct kevent *change_events, *event_events;

static struct kevent *GetChangeEvent()
{
    if(change_count == max_fds)
    {
	timespec zero_timespec = { 0, 0 }
	for(int i = 0; i < change_count; ++i)
	    kevent(kq_fd, &change_events[i], 1, NULL, 0, &zero_timespec);
	change_count = 0;
    }
    return &change_events[change_count++];
}

void SocketHandler::Initialize()
{
    kq_fd = kqueue();
    max_fds = getdtablesize();
    change_count = 0;

    if(kq_fd < 0)
	throw SocketException("Unable to create kqueue engine: "+strerror(errno));
    else if(max_fds <= 0)
	throw SocketException("Cannot determine maximum number of open sockets");

    change_events = new struct kevent[max_fds];
    event_events = new struct kevent[max_fds];
    Log(LOG_RAWIO) << "[SocketHandler] Using Kqueue Socket Handler with " << max_fds << " max sockets";
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

    delete[] change_events;
    delete[] event_events;
}

// Set the socket as writable so we can send through the multiplexer
void SocketHandler::SetWritable(Socket *s)
{
    if(s && !(s->flags & SF_WRITABLE))
    {
	struct kevent *event = GetChangeEvent();
	EV_SET(event, s->GetFileDescriptor(), EVFILT_WRITE, EV_ADD, 0, 0, NULL);

	s->flags |= SF_WRITABLE;
    }
}

// Clear the writable status
void SocketHandler::ClearWritable(Socket *s)
{
    if(s && (s->flags & SF_WRITABLE))
    {
	struct kevent *event = GetChangeEvent();
	EV_SET(event, s->GetFileDescriptor(), EVFILT_WRITE, EV_DELETE, 0, 0, NULL);

	s->flags &= ~SF_WRITABLE;
    }
}

// Add a socket to handle
void SocketHandler::AddSocket(Socket *s)
{
    struct kevent *event = GetChangeEvent();
    EV_SET(event, s->GetFileDescriptor(), EVFILT_READ, EV_ADD, 0, 0, NULL);
    SocketHandler::Sockets[s->GetFileDescriptor()] = s;
}

// Remove a socket to handle
void SocketHandler::RemoveSocket(Socket *s)
{
    struct kevent *event = GetChangeEvent();
    EV_SET(event, s->GetFileDescriptor(), EVFILT_READ, EV_DELETE, 0, 0, NULL);

    event = GetChangeEvent();
    EV_SET(event, s->GetFileDescriptor(), EVFILT_WRITE, EV_DELETE, 0, 0, NULL);

    SocketHandler::Sockets.erase(s->GetFileDescriptor());
}

// Process all the sockets created.
void SocketHandler::ProcessSockets()
{
    static timespec kq_timespec = { 5, 0 };
    int TotalFDs = kevent(kq_fd, change_events, change_count, event_events, max_fds, &kq_timespec);
    change_count = 0;

    if(TotalFDs == -1 && errno != EINTR)
    {
	// Don't throw because the error could be temporary!
	Log(LOG_WARN) << "[SocketHandler] Socket Handler error: " << strerror(errno);
	return;
    }

    for(int i = 0, processed = 0; i < socket_positions.size() && processed != TotalFDs; ++i)
    {
	struct kevent *event = &event_events[i];
	if(event->flags & EV_ERROR)
	    continue;

	std::map<int, Socket*>::iterator it = Sockets.find(ev->data.fd);
	if(it = Sockets.end())
	    continue;
	Socket *s = it->second;

	if(event->flags & EV_EOF)
	{
	    s->ProcessError();
	    delete s;
	    continue;
	}

	if(!s->Process())
	    continue;

	if(event->filter == EVFILT_READ && !s->ProcessRead())
	    s->SetDead(true);

	if(event->filter == EVFILT_WRITE && !s->ProcessWrite())
	    s->SetDead(true);

	if(s->IsDead())
	{
	    Log(LOG_RAWIO) << "[SocketHandler] Socket " << s->GetFileDescriptor() << " died!\n";
	    delete s;
	}
    }
}