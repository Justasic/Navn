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

// This is the WSAWaitForMultipleObjects implementation of the SocketHandler
// This may need to be I/O Completion Ports as [WSA]WaitFor{Single|Multiple}{Event|Object}[s] is considered expensive
// TODO: this.

// Incase someone derps
#ifndef _WIN32
# error This socket engine is designed to only work in windows
#endif
#include "Networking_Multiplexer.h"
#include "log.h"

// Need to link with Ws2_32.lib
#pragma comment(lib, "ws2_32.lib")

#error This socket engine is incomplete!

static unsigned long EventTotal;
static WSAEVENT EventArray[WSA_MAXIMUM_WAIT_EVENTS];
static WSANETWORKEVENTS NetworkEvents;
WSAInstance *winsocketinstance;

void SocketHandler::Initialize()
{
    winsocketinstance = new WSAInstance();
    EventArray[EventTotal] = WSACreateEvent();
    EventTotal++;
    Log(LOG_RAWIO) << "[SocketHandler] Using Windows Socket Handler";
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

    delete winsocketinstance;
}

// Set the socket as writable so we can send through the multiplexer
void SocketHandler::SetWritable(Socket *s)
{
    if(s && !(s->flags & SF_WRITABLE))
    {


	s->flags |= SF_WRITABLE;
    }
}

// Clear the writable status
void SocketHandler::ClearWritable(Socket *s)
{
    if(s && (s->flags & SF_WRITABLE))
    {


	s->flags &= ~SF_WRITABLE;
    }
}

// Add a socket to handle
void SocketHandler::AddSocket(Socket *s)
{


    SocketHandler::Sockets[s->FileDescriptor()] = s;
}

// Remove a socket to handle
void SocketHandler::RemoveSocket(Socket *s)
{

    SocketHandler::Sockets.erase(s->FileDescriptor());
}

// Process all the sockets created.
void SocketHandler::ProcessSockets()
{
    int ReturnVal = WSAWaitForMultipleObjects()

    if(ReturnVal == -1 && errno != EINTR)
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
	    Log(LOG_RAWIO) << "[SocketHandler] Socket " << s->FileDescriptor() << " died!";
	    delete s;
	}
    }
}