/* Navn IRC bot -- Socket Engine
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */
/**
 *\file  Socket.cpp
 *\brief Contains the Socket engine.
 */
#include "SocketException.h"
#include "Socket.h"
#include <fcntl.h>
#include <iostream>
bool throwex;
Flux::string throwmsg;
fd_set ReadFD, WriteFD, ExceptFD;

Flux::string ForwardResolution(const Flux::string &hostname)
{
  struct addrinfo *result, *res;
  int err = getaddrinfo(hostname.c_str(), NULL, NULL, &result);
  
  if(err != 0)
  {
    Log(LOG_TERMINAL) << "Failed to resolve " << hostname << ": " << gai_strerror(err);
    return "";
  }
  
  bool gothost = false;
  Flux::string ret = hostname;
  for(res = result; res != NULL && !gothost; res = res->ai_next)
  {
    struct sockaddr *haddr;
    haddr = res->ai_addr;
    char address[INET6_ADDRSTRLEN + 1] = "";
    switch(haddr->sa_family)
    {
      case AF_INET:
	struct sockaddr_in *v4;
	v4 = reinterpret_cast<struct sockaddr_in*>(haddr);
	if (!inet_ntop(AF_INET, &v4->sin_addr, address, sizeof(address)))
	{
	  Log(LOG_DEBUG) << "DNS: " << strerror(errno);
	  return "";
	}
	break;
      case AF_INET6:
	struct sockaddr_in6 *v6;
	v6 = reinterpret_cast<struct sockaddr_in6*>(haddr);
	if (!inet_ntop(AF_INET6, &v6->sin6_addr, address, sizeof(address)))
	{
	  Log(LOG_DEBUG) << "DNS: " << strerror(errno);
	  return "";
	}
	break;
    }
    
    ret = address;
    gothost = true;
  }
  
  freeaddrinfo(result);
  return ret;
}

/* FIXME: please god, when will the hurting stop? This class is so
   f*cking broken it's not even funny */
SocketIO::SocketIO(const Flux::string &cserver, const Flux::string &cport) : server(cserver), port(cport), sockn(-1), ipv6(false), iswritable(false)
{
  FD_ZERO(&ReadFD);
  FD_ZERO(&WriteFD);
  FD_ZERO(&ExceptFD);

  this->ip = ForwardResolution(cserver);
  throwex = false;

  if(this->ip.find_first_not_of("1234567890.:/") != Flux::string::npos)
    throw SocketException("Unable to resolve IP address from hostname: "+cserver);
  if(this->ip.search(':'))
    this->ipv6 = true;
  /****************************/
  sockn = socket(this->IsIPv6() ? AF_INET6 : AF_INET, SOCK_STREAM, 0);
  this->SetNonBlocking();
  FD_SET(this->GetFD(), &ReadFD);
}

SocketIO::~SocketIO()
{
  if(is_valid())
    close(sockn);
  
  FD_CLR(this->GetFD(), &ReadFD);
  FD_CLR(this->GetFD(), &WriteFD);
}

int SocketIO::GetFD() const { return sockn; }
void SocketIO::ThrowException(const Flux::string &msg) { throwex = true; throwmsg = msg; }

bool SocketIO::SetNonBlocking()
{
  int flags = fcntl(this->GetFD(), F_GETFL, 0);
  return !fcntl(this->GetFD(), F_SETFL, flags | O_NONBLOCK);
}

bool SocketIO::SetBlocking()
{
  int flags = fcntl(this->GetFD(), F_GETFL, 0);
  return !fcntl(this->GetFD(), F_SETFL, flags & ~O_NONBLOCK);
}

bool SocketIO::IsIPv6() const { return ipv6; }

void SocketIO::Connect()
{
  SET_SEGV_LOCATION();
  
  struct addrinfo *servinfo;
  int rv = 0;
  if((rv = getaddrinfo(this->ip.c_str(), this->port.c_str(), NULL, &servinfo)) != 0)
    throw SocketException(printfify("Could not resolve server (%s:%u): %s", this->ip.c_str(), this->port.c_str(), gai_strerror(rv)));

  int c = connect(this->GetFD(), servinfo->ai_addr, servinfo->ai_addrlen);
  if( c == -1)
  {
    if(errno != EINPROGRESS)
      throw SocketException(printfify("Cannot connect to %s: %s", this->ip.c_str(), strerror(errno)));
    else
    {
      FD_SET(this->GetFD(), &WriteFD);
    }
  }
  else
  {
    // put OnConnect stuff here?
    Log(LOG_DEBUG) << "Connected to " << this->server << ":" << this->port << ' ' << '(' << this->ip << ')';
  }
//   for(struct addrinfo *p = servinfo; p != NULL; p = p->ai_next)
//   {
//     sockn = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
//     if (this->GetFD() < 0)
//       continue;
//     connected = connect(this->GetFD(), p->ai_addr, p->ai_addrlen);
//     if (connected == -1)
//     {
//       close(sockn);
//       continue;
//     }
//     break;
//   }
// 
//   if (connected == -1)
//     throw SocketException("Connection Failed.");

  freeaddrinfo(servinfo); //Clear up used memory we don't need anymore
//   FD_SET(this->GetFD(), &ReadFD);
}

void SocketIO::Process()
{
  SET_SEGV_LOCATION();
  timeval timeout;
  timeout.tv_sec = Config->SockWait;
  timeout.tv_usec = 0; //this timeout keeps the bot from being a CPU hog for no reason :)
  fd_set read = ReadFD, write = WriteFD, except = ExceptFD;

  FD_ZERO(&read);
  FD_SET(this->GetFD(), &read);

  int sres = select(this->GetFD() + 1, &read, &write, &except, &timeout);
  if(sres == -1 && errno != EINTR)
  {
    Log(LOG_DEBUG) << "Select() error: " << strerror(errno);
    return;
  }

  if(throwex) //throw a socket exception if we want to. mainly used for ping timeouts.
    throw SocketException(throwmsg);

  bool has_read = FD_ISSET(this->GetFD(), &read);
  bool has_write = FD_ISSET(this->GetFD(), &write);
  bool has_error = FD_ISSET(this->GetFD(), &except);

  if(has_error)
  {
    int optval = 0;
    socklen_t optlen = sizeof(optval);
    getsockopt(this->GetFD(), SOL_SOCKET, SO_ERROR, reinterpret_cast<char *>(&optval), &optlen);
    errno = optval;
    FOREACH_MOD(I_OnSocketError, OnSocketError(optval ? strerror(errno) : "Unknown socket error"));
    throw SocketException("Socket error");
  }
  
  if(has_read)
  {
    char tbuf[BUFSIZE + 1] = "\0";
    size_t i = recv(this->GetFD(), tbuf, BUFSIZE, 0);
    
    if(i <= 0)
      throw SocketException(printfify("Socket Error: %s", strerror(errno)));
    
    sepstream sep(tbuf, '\n');
    Flux::string buf;
    while(sep.GetToken(buf))
    {
      this->LastBuf.clear();
      this->LastBuf = buf;
      if(!this->Read(buf))
	throw SocketException("Error reading socket");
    }
    
    FD_CLR(this->GetFD(), &ReadFD);
  }

  if(has_write)
  {
    Flux::string buf;
    while(!this->WriteBuffer.empty())
    {
      buf = this->WriteBuffer.front();
      this->WriteBuffer.pop();

      int i = ::send(this->GetFD(), buf.c_str(), buf.size(), 0);

      Log(LOG_RAWIO) << "Sent: " << buf << " | Size: " << buf.size();
      
      this->LastBuf.clear();
      this->LastBuf = buf;
      
      if(i <= -1)
	throw SocketException("Error writing \"" + buf + "\" to socket");

      buf.clear();
    }

    if(!this->iswritable)
    {
      FD_CLR(this->GetFD(), &WriteFD);
      this->iswritable = false;
    }
  }
}

Flux::string SocketIO::GetLastBuf() const
{
  return this->LastBuf;
}

void SocketIO::send(const Flux::string buf)
{
  if(!protocoldebug)
    Log(LOG_DEBUG) << buf;

  this->WriteBuffer.push(buf);

  if(this->iswritable)
  {
    FD_SET(this->GetFD(), &WriteFD);
    this->iswritable = true;
  }
}

/**
 * \fn bool SocketIO::Read(const Flux::string &buf) const
 * \brief Read from a socket for the IRC processor
 * \param buffer Raw socket buffer
 */
bool SocketIO::Read(const Flux::string &buf) const
{
  if(buf.search_ci("ERROR :Closing link:"))
  {
    FOREACH_MOD(I_OnSocketError, OnSocketError(buf));
    return false;
  }
  Log(LOG_RAWIO) << "Received: " << Flux::Sanitize(buf);
  process(buf); /* Process the buffer for navn */
  return true;
}

/** \fn void send_cmd(const char *fmt, ...)
 * \brief Sends something directly out the socket after being processed by vsnprintf
 * \param char* a string of what to send to the server including printf style format
 * \param va_list all the variables to be replaced with the printf style variables
 */
void send_cmd(const char *fmt, ...)
{
  char buffer[BUFSIZE] = "";
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  if(sock)
    sock->send(buffer);
  else
    Log(LOG_WARN) << "Attempted to send \"" << buffer << "\" to the server but no socket exists!";
  va_end(args);
}
