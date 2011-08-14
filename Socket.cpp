/* Socket.cpp */
/**
 *\file  Socket.cpp 
 *\brief Contains the Socket engine.
 */
#include "Socket.h"
#include "string.h"
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#define NET_BUFSIZE 65535

using namespace std;

Flux::string Sanitize(const Flux::string &string){
 static struct special_chars{
   Flux::string character;
   Flux::string replace;
   special_chars(const Flux::string &c, const Flux::string &r) : character(c), replace(r) { }
 }
 special[] = {
  special_chars("\n",""),
  special_chars("\002",""),
  special_chars("\003",""),
  special_chars("\035",""),
  special_chars("\037",""),
  special_chars("\026",""),
  special_chars("\001",""),
  special_chars("","")
 };
  Flux::string ret = string.c_str();
  for(int i = 0; special[i].character.empty() == false; ++i){
    ret = ret.replace_all_cs(special[i].character, special[i].replace);
  }
  return ret.c_str(); 
}
/*********************************************************************************************/

int recvlen;
/* FIXME: please god, when will the hurting stop? This class is so
   f*cking broken it's not even funny */
SocketIO::SocketIO(const Flux::string cserver, const Flux::string cport) : sockn(-1){
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
   ::close(sockn);
}
bool SocketIO::get_address()
{
  int rv = 1;
  rv = getaddrinfo(this->server.c_str(), this->port.c_str(), &hints, &servinfo);
  //fprintf(stderr, "getaddrinfo: %i\n", rv);
  if (rv != 0) {
    return false;
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
  }
  return true;
  //freeaddrinfo(servinfo);
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
  printf("Connecting..\n");
  
  for(p = servinfo; p != NULL; p = p->ai_next) {
    sockn = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (sockn < 0) continue;
    int connected = ::connect(sockn, p->ai_addr, p->ai_addrlen);
    if (connected == -1){
      ::close(sockn);
      printf("Connection failed.\n");
      continue;
    }
    break;
  }
  
  if (connected == -1) return false;
  freeaddrinfo(servinfo);
  
  inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
  setNonblocking(sockn);
  printf("Connected!\n");
  return true;
}

const int SocketIO::recv(Flux::string& buffer) const{
  char tbuf[NET_BUFSIZE + 1] = "";
  memset(tbuf, 0, NET_BUFSIZE + 1);
  //size_t i = ::recv(sockn, tbuf, NET_BUFSIZE, 0);
  int i = read(sockn, tbuf, NET_BUFSIZE);
  if(i <= 0)
    return i;
  sepstream sep(tbuf, '\n');
  Flux::string buf;
  buffer = tbuf;
  while(sep.GetToken(buf)){
    buf.trim();
    //buffer = buf;
    printf("--> %s\n", Sanitize(buf).c_str());
  }
  return i;
}
const int SocketIO::send(const Flux::string buf) const{
 printf("<-- %s\n", Sanitize(buf).c_str());
 //int i = ::send(sockn, buf.c_str(), buf.size(), 0);
 int i = write(sockn, buf.c_str(), buf.size());
 return i;
}