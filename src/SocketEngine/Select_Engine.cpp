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
// This is the Select() implementation of the SocketHandler
#include <sys/select.h> // Include the multiplexer
#include <sys/time.h> // Used for determining the blocking time of the program
#include <unistd.h>
#include <errno.h>
#include <cstdio>
#include <cstring>
#include "Networking_Multiplexer.h"
#include "log.h"

// These are select only variables, therefore we cannot include them into the SocketHandler class
// Current maximum file descriptor count
static int MaxFD;
// Current file descriptor count
static unsigned FDCount;
// Select stuff
static fd_set ReadFDs;
static fd_set WriteFDs;
#ifdef _WIN32
WSAInstance *winsockinstance;
#endif

std::map<int, Socket*> SocketHandler::Sockets;

void SocketHandler::Initialize()
{
    #ifdef _WIN32
    winsockinstance = new WSAInstance();
    #endif
    MaxFD = 0;
    FDCount = 0;
    FD_ZERO(&ReadFDs);
    FD_ZERO(&WriteFDs);
    Log(LOG_RAWIO) << "[SocketHandler] Using Select Socket Handler";
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
    #ifdef _WIN32
    delete winsockinstance;
    #endif
}

// Set the socket as writable so we can send through the multiplexer
void SocketHandler::SetWritable(Socket *s)
{
    if(s && !(s->flags & SF_WRITABLE))
    {
	s->flags |= SF_WRITABLE;
	FD_SET(s->GetFileDescriptor(), &WriteFDs);
    }
}

// Clear the writable status
void SocketHandler::ClearWritable(Socket *s)
{
    if(s && (s->flags & SF_WRITABLE))
    {
	s->flags &= ~SF_WRITABLE;
	FD_CLR(s->GetFileDescriptor(), &WriteFDs);
    }
}

// Add a socket to handle
void SocketHandler::AddSocket(Socket *s)
{
    if (s->GetFileDescriptor() > MaxFD)
	MaxFD = s->GetFileDescriptor();
    FD_SET(s->GetFileDescriptor(), &ReadFDs);
    SocketHandler::Sockets.insert(std::make_pair(s->GetFileDescriptor(), s));
    ++FDCount;
}

// Remove a socket to handle
void SocketHandler::RemoveSocket(Socket *s)
{
    if (s->GetFileDescriptor() == MaxFD)
	--MaxFD;
    FD_CLR(s->GetFileDescriptor(), &ReadFDs);
    FD_CLR(s->GetFileDescriptor(), &WriteFDs);
    SocketHandler::Sockets.erase(s->GetFileDescriptor());
    --FDCount;
}

// Process all the sockets created.
void SocketHandler::ProcessSockets()
{
    fd_set read = ReadFDs, write = WriteFDs, error = ReadFDs;
    timeval tval;
    tval.tv_sec = 5;
    tval.tv_usec = 0;

    int sresult = select(MaxFD + 1, &read, &write, &error, &tval);

    if (sresult == -1) // Don't throw because the error could be temporary!
	Log(LOG_WARN) << "[SocketHandler] Socket Handler error: " << strerror(errno);

    if(sresult < 1)
	return; // Nothing to do..
    else if (sresult)
    {
	int processed = 0;
	for (std::map<int, Socket*>::iterator it = SocketHandler::Sockets.begin(), it_end = SocketHandler::Sockets.end(); it != it_end && processed != sresult;)
	{
	    Socket *s = it->second;
	    ++it;

	    bool has_read = FD_ISSET(s->GetFileDescriptor(), &read);
	    bool has_write = FD_ISSET(s->GetFileDescriptor(), &write);
	    bool has_error = FD_ISSET(s->GetFileDescriptor(), &error);

	    if (has_read || has_write || has_error)
		++processed;

	    if (has_error)
	    {
		s->ProcessError();
		s->SetDead(true);
		continue;
	    }

	    if(!s->Process())
		continue;

	    if (has_read && !s->ProcessRead())
		s->SetDead(true);

	    if (has_write && !s->ProcessWrite())
		s->SetDead(true);

	    if (s->IsDead())
	    {
		Log(LOG_RAWIO) << "[SocketHandler] Socket " << s->GetFileDescriptor() << " died!\n";
		delete s;
	    }
	}
    }
}
