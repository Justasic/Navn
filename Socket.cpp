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

SocketIO::SocketIO(const Flux::string cserver, const Flux::string cport) : sockn(-1){
  this->server = cserver.tostd();
  this->port = cport.tostd();
  
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
}

SocketIO::~SocketIO(){
 if(is_valid()) 
   ::close(sockn);
}
void SocketIO::close(){
 ::close(sockn);
}
bool SocketIO::get_address()
{
  int rv = 1;
  rv = getaddrinfo(this->server.c_str(), this->port.c_str(), &hints, &servinfo);
  //fprintf(stderr, "getaddrinfo: %i\n", rv);
  if (rv != 0) return false;
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
  int count = 0;
  int connected = 0;
  char s[INET6_ADDRSTRLEN];
  
  for(p = servinfo; p != NULL; p = p->ai_next) {
    count++;
    sockn = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (sockn == -1) continue;
    int connected = ::connect(sockn, p->ai_addr, p->ai_addrlen);
    if (connected == -1){
      ::close(sockn);
      continue;
    }
    break;
  }
  
  if (connected == -1) return false;
  
  inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
  return true;
}

const int SocketIO::recv(Flux::string& buffer) const{
  char tbuf[NET_BUFSIZE + 1] = "";
  memset(tbuf, 0, NET_BUFSIZE + 1);
  size_t i = ::recv(sockn, tbuf, NET_BUFSIZE, 0);
  sepstream sep(tbuf, '\n');
  Flux::string buf;
  buffer = tbuf;
  while(sep.GetToken(buf))
   printf("--> %s\n", Sanitize(buf).c_str());
  return i;
}
const int SocketIO::send(const Flux::string buf) const{
 printf("<-- %s\n", Sanitize(buf).c_str());
 int i = ::send(sockn, buf.c_str(), buf.size(), 0);
 return i;
}