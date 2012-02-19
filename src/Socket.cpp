/* Navn IRC bot -- Socket Engine
 * 
 * (C) 2011-2012 Flux-Net
 * Contact us at Dev@Flux-Net.net
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
fd_set ReadFD/*, WriteFD, ExceptFD*/;

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
	if (!inet_ntop(AF_INET, &v4->sin_addr, address, sizeof(address))){
	  Log(LOG_DEBUG) << "DNS: " << strerror(errno);
	  return "";
	}
	break;
      case AF_INET6:
	struct sockaddr_in6 *v6;
	v6 = reinterpret_cast<struct sockaddr_in6*>(haddr);
	if (!inet_ntop(AF_INET6, &v6->sin6_addr, address, sizeof(address))){
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
SocketIO::SocketIO(const Flux::string &cserver, const Flux::string &cport) : sockn(-1)
{
  this->server = cserver;
  this->ip = ForwardResolution(cserver);
  this->port = cport;
  throwex = false;

  if(this->ip.find_first_not_of("1234567890.:/") != Flux::string::npos)
    throw SocketException("Unable to resolve IP address from hostname: "+cserver);
  if(this->ip.search(':'))
    this->ipv6 = true;
  /****************************/ 
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

SocketIO::~SocketIO()
{
 if(is_valid()) 
   close(sockn);
 FD_CLR(this->GetFD(), &ReadFD);
}

bool SocketIO::IsIPv6() const { return ipv6; }

bool SocketIO::Connect()
{
  SET_SEGV_LOCATION();
  struct addrinfo *servinfo;
  int connected, rv = 0;
  if((rv = getaddrinfo(this->ip.c_str(), this->port.c_str(), NULL, &servinfo)) != 0)
    throw SocketException(printfify("Could not resolve server (%s:%i): %s",this->ip.c_str(),
				   static_cast<int>(this->port), gai_strerror(rv)).c_str());
  
  for(struct addrinfo *p = servinfo; p != NULL; p = p->ai_next)
  {
    sockn = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (this->GetFD() < 0) 
      continue;
    connected = connect(this->GetFD(), p->ai_addr, p->ai_addrlen);
    if (connected == -1)
    {
      close(sockn);
      continue;
    }
    break;
  }
  
  if (connected == -1)
    throw SocketException("Connection Failed.");
    
  freeaddrinfo(servinfo); //Clear up used memory we dont need anymore
  
  this->SetNonBlocking();
  FD_SET(this->GetFD(), &ReadFD);
  Log(LOG_DEBUG) << "Connected to " << this->server << ":" << this->port << ' ' << '(' << this->ip << ')';
  return true;
}

int SocketIO::Process()
{
  timeval timeout;
  timeout.tv_sec = Config->SockWait;
  timeout.tv_usec = 0; //this timeout keeps the bot from being a CPU hog for no reason :)
  fd_set read = ReadFD/*, write = WriteFD, except = ExceptFD*/;
  
  FD_ZERO(&read);
  FD_SET(this->GetFD(), &read);
  
  int sres = select(this->GetFD() + 1, &read, NULL, NULL, &timeout);
  if(sres == -1 && errno != EINTR){
    Log(LOG_DEBUG) << "Select() error: " << strerror(errno);
    return errno;
  }
  if(throwex) //throw a socket exception if we want to. mainly used for ping timeouts.
    throw SocketException(throwmsg);
  if(FD_ISSET(this->GetFD(), &read) && sres)
  {
  if(this->recv() == -1 && !quitting)
      {
	Log(LOG_RAWIO) << "Socket Error: " << strerror(errno);
	return errno;
      }else
      {
	return this->recv();
      }
  }
  return sres;
}

int SocketIO::recv() const
{
  char tbuf[BUFSIZE + 1] = "";
  memset(tbuf, 0, BUFSIZE + 1);
  size_t i = read(this->GetFD(), tbuf, BUFSIZE);
  if(i <= 0)
    return i;
  sepstream sep(tbuf, '\n');
  Flux::string buf;
  while(sep.GetToken(buf))
  {
    buf.trim();
    LastBuf = buf;
    this->Read(buf);
  }
  return i;
}

int SocketIO::send(const Flux::string buf) const
{
 LastBuf = buf;
 Log(LOG_RAWIO) << "Sent: " << buf << " | Size: " << buf.size();
 if(!protocoldebug)
   Log(LOG_DEBUG) << buf;
 int i = write(this->GetFD(), buf.c_str(), buf.size());
 return i;
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
  sock->send(buffer);
  va_end(args);
}