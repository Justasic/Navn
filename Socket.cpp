/* Socket.cpp */
/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
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

/* FIXME: please god, when will the hurting stop? This class is so
   f*cking broken it's not even funny */
SocketIO::SocketIO(const Flux::string &cserver, const Flux::string &cport) : sockn(-1){
  SET_SEGV_LOCATION();
  this->server = cserver.std_str();
  this->port = cport.std_str();
  throwex = false;
  
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
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
SocketIO::~SocketIO(){
  SET_SEGV_LOCATION();
 if(is_valid()) 
   close(sockn);
 FD_CLR(this->GetFD(), &ReadFD);
 //FD_CLR(this->GetFD(), &WriteFD);
}
void SocketIO::get_address()
{
  SET_SEGV_LOCATION();
  int rv = 1;
  rv = getaddrinfo(this->server.c_str(), this->port.c_str(), &hints, &servinfo);
  if (rv != 0)
  {
    throw SocketException(fsprintf("Could not resolve server: %s:%i %s",this->server.c_str(), 
				   (int)this->port, gai_strerror(rv)).c_str());
  }
}

void *get_in_addr(struct sockaddr *sa)
{
  SET_SEGV_LOCATION();
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

bool SocketIO::Connect()
{
  SET_SEGV_LOCATION();
  struct addrinfo *p;
  int connected = 0;
  char s[INET6_ADDRSTRLEN];
  
  this->get_address(); // catch any resolution issues before we even attempt to connect..
  
  for(p = servinfo; p != NULL; p = p->ai_next)
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
  
  inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, INET6_ADDRSTRLEN+1);
  this->SetNonBlocking();
  FD_SET(this->GetFD(), &ReadFD);
  Log(LOG_DEBUG) << "Connected to" << this->server << ":" << this->port;
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