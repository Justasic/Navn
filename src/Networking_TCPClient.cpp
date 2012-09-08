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
#include "Networking_TCPClient.h"
#include "log.h"

// Use a file descriptor from a premade socket
TCPSocket::TCPSocket(int FileDescriptor, int iptype, int socktype) : Socket(FileDescriptor, iptype, socktype), resolver(0)
{
    printf("File descriptor %d is %svalid.\n", this->GetFileDescriptor(), SocketHandler::IsValidFD(this->GetFileDescriptor()) ? "" : "NOT ");
    //Log(LOG_TERMINAL) << "File descriptor " << this->GetFileDescriptor() << " is "
    //<< (SocketHandler::IsValidFD(this->GetFileDescriptor()) ? "" : "NOT") << " a valid file descriptor.";
}

// Connect to a specific address
TCPSocket::TCPSocket(const Flux::string &addr, unsigned short pn, bool ipv6) : Socket(-1, ipv6 ? AF_INET6 : AF_INET, SOCK_STREAM), port(pn), resolver(new DNSInstance(addr, pn))
{
    printf("File descriptor %d is %svalid.\n", this->GetFileDescriptor(), SocketHandler::IsValidFD(this->GetFileDescriptor()) ? "" : "NOT ");
}

TCPSocket::~TCPSocket()
{
    while(!Buffer.empty())
	Buffer.pop();

    Log(LOG_RAWIO) << "Destroying socket " << this->GetFileDescriptor();
}

void TCPSocket::Connect()
{
    if (!resolver)
	throw SocketException("No server specified.");

    // Clear connection flags before we connect
    this->flags &= ~(SF_CONNECTED | SF_CONNECTING);

    struct sockaddr addr;
    addr = *resolver->GetSocketAddress();

    Log(LOG_TERMINAL) << "File descriptor " << this->GetFileDescriptor() << " is "
    << (SocketHandler::IsValidFD(this->GetFileDescriptor()) ? "" : "NOT ") << "a valid file descriptor.";

    /* Establish connection */
    int c = connect(this->GetFileDescriptor(), &addr, sizeof(addr));
    if (c == -1)
    {
	if(errno != EINPROGRESS)
	    this->OnError(strerror(errno));
	else
	{
	    SocketHandler::SetWritable(this);
	    this->flags |= SF_CONNECTING;
	}
    }
    else
    {
	// Notify the socket that we've connected successfully
	this->flags |= SF_CONNECTED;
	this->ConnectNotify();
    }
}

// Process reading
bool TCPSocket::ProcessRead()
{
    Flux::string message;
    int recvbytes = this->InternalReceive(message);
    if (recvbytes < 0)
    {
	Log(LOG_WARN) << "Failed to receive bytes from server: " << strerror(errno);
	return false;
    }

    sepstream sep(message, '\n');
    Flux::string buf;
    while(sep.GetToken(buf))
    {
	if(!this->Read(buf))
	    return false;
    }
    return true;
}

// Actually write out the socket.
bool TCPSocket::ProcessWrite()
{
    while(!Buffer.empty())
    {
	// Send to the Socket class to send out the socket
	int sendbytes = this->InternalSend(Buffer.front());
	if(sendbytes < 0)
	{
	    Log(LOG_WARN) << "Cannot send through socket: " << strerror(errno);
	    return false;
	}
	// Pop the first message in the buffer
	Buffer.pop();
    }
    // We don't need to write anymore.
    SocketHandler::ClearWritable(this);
    return true;
}

void TCPSocket::ProcessError()
{
    int optval = 0;
    socklen_t optlen = sizeof(optval);
    getsockopt(this->GetFileDescriptor(), SOL_SOCKET, SO_ERROR, reinterpret_cast<char *>(&optval), &optlen);
    errno = optval;
    this->OnError(optval ? strerror(errno) : "");
}

bool TCPSocket::Process()
{
    try
    {
	if(this->flags & SF_CONNECTED)
	    return true;
	else if(this->flags & SF_CONNECTING)
	{
	    int optval = 0;
	    socklen_t optlen = sizeof(optval);
	    if (!getsockopt(this->GetFileDescriptor(), SOL_SOCKET, SO_ERROR, reinterpret_cast<char *>(&optval), &optlen) && !optval)
	    {
		this->flags &= ~SF_CONNECTING;
		this->flags |= SF_CONNECTED;
		this->ConnectNotify();
		return true;
	    }
	    else
	    {
		errno = optval;
		this->OnError(optval ? strerror(errno) : "");
		Log(LOG_RAWIO) << "Socket " << this->GetFileDescriptor() << " Set dead on in TCPSocket::Process()!";
		return false;
	    }
	}
	else
	    return false;
    }
    catch (const SocketException &ex)
    {
	ex.Print();
    }
    return false;
}

void TCPSocket::Send(const Flux::string &msg)
{
    // Set this socket as writable
    SocketHandler::SetWritable(this);
    // Buffer the messages and let the send handler send it.
    Buffer.push(msg);
}

bool TCPSocket::Read(const Flux::string&)
{
    // Nothing to do.
    return true;
}
