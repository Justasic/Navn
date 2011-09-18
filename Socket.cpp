/* Socket.cpp */
/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
/**
 *\file  Socket.cpp 
 *\brief Contains the Socket engine.
 */
#include <Socket.h>
#include <fcntl.h>
#include <iostream>
#define NET_BUFSIZE 65535
fd_set ReadFD, WriteFD, ExceptFD;

/* FIXME: please god, when will the hurting stop? This class is so
   f*cking broken it's not even funny */
SocketIO::SocketIO(const Flux::string &cserver, const Flux::string &cport) : sockn(-1){
  this->server = cserver.tostd();
  this->port = cport.tostd();
  
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  /****************************/ 
}
int setNonblocking(int fd)
{
    int flags;
    /* If they have O_NONBLOCK, use the Posix way to do it */
#if defined(O_NONBLOCK)
    /* Fixme: O_NONBLOCK is defined but broken on SunOS 4.1.x and AIX 3.2.5. */
    if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
        flags = 0;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
    /* Otherwise, use the old way of doing it */
    flags = 1;
    return ioctl(fd, FIOBIO, &flags);
#endif
}
SocketIO::~SocketIO(){
 if(is_valid()) 
   close(sockn);
 FD_CLR(sockn, &ReadFD);
 FD_CLR(sockn, &WriteFD);
}
bool SocketIO::get_address()
{
  int rv = 1;
  rv = getaddrinfo(this->server.c_str(), this->port.c_str(), &hints, &servinfo);
  //fprintf(stderr, "getaddrinfo: %i\n", rv);
  if (rv != 0) {
    return false;
    //fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    log(LOG_DEBUG, "getaddrinfo: %s", gai_strerror(rv));
  }
  return true;
}

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

bool SocketIO::connect()
{
  struct addrinfo *p;
  int connected = 0;
  char s[INET6_ADDRSTRLEN];
  log(LOG_RAWIO, "Connecting..");
  
  for(p = servinfo; p != NULL; p = p->ai_next) {
    sockn = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (sockn < 0) continue;
    connected = ::connect(sockn, p->ai_addr, p->ai_addrlen);
    if (connected == -1){
      close(sockn);
      log(LOG_DEBUG, "Connection Failed: %s", strerror(errno));
      continue;
    }
    break;
  }
  
  if (connected == -1) return false;
  freeaddrinfo(servinfo); //Clear up used memory we dont need anymore
  
  inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
  setNonblocking(sockn);
  FD_SET(sockn, &ReadFD);
  log(LOG_DEBUG, "Connected! %i", sockn);
  return true;
}

std::queue<Flux::string> recv_queue;
int receive(int sockn)
{
  char tbuf[NET_BUFSIZE + 1] = "";
  memset(tbuf, 0, NET_BUFSIZE + 1);
  size_t i = read(sockn, tbuf, NET_BUFSIZE);
  if(i <= 0)
    return i;
  sepstream sep(tbuf, '\n');
  Flux::string buf;
  while(sep.GetToken(buf))
  {
    buf.trim();
    recv_queue.push(buf);
    //printf("buf: --> %s\n", Flux::Sanitize(buf).c_str());
  }
  return i; 
}
const int SocketIO::recv() const
{
  timeval timeout;
  timeout.tv_sec = 5;
  timeout.tv_usec = 0; //this timeout keeps the bot from being a CPU hog for no reason :)
  fd_set read = ReadFD, write = WriteFD, except = ExceptFD;
  FD_ZERO(&read);
  FD_SET(sockn, &read);
  int sres = select(sockn + 1, &read, NULL, NULL, &timeout);
  if(sres == -1 && errno != EINTR){
    log(LOG_DEBUG, "Select() error: %s", strerror(errno));
    return errno;
  }
  if(FD_ISSET(sockn, &read) && sres)
  {
      if(receive(sockn) == -1 && !quitting)
      {
	log(LOG_RAWIO, "Socket Error: %s", strerror(errno));
	return errno;
      }else
      {
	return receive(sockn);
      }
  }
  return sres;
}
std::queue<Flux::string> SocketIO::GetBuffer()
{
  this->recv();
  return recv_queue;
}
void SocketIO::popqueue()
{
 recv_queue.pop(); 
}
const int SocketIO::send(const Flux::string &buf) const
{
 log(LOG_RAWIO, "Sent: %s\n", Flux::Sanitize(buf).c_str());
 if(!protocoldebug)
  log(LOG_DEBUG, "%s\n", Flux::Sanitize(buf).c_str());
 int i = write(sockn, buf.c_str(), buf.size());
 return i;
}
void send_cmd(const char *fmt, ...)
{
  char buffer[4096] = "";
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  sock->send(buffer);
  va_end(args);
}