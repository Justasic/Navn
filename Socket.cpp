/* Socket.cpp */

#include "Socket.h"
#include "string.h"
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <iostream>

Socket::Socket(const std::string s, const std::string p) : m_sock(-1), server(s), port(p)
{
  memset ( &hints, 0, sizeof ( hints ) );
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
}

Socket::~Socket()
{
  if ( is_valid() ) ::close ( m_sock );
}


bool Socket::get_address()
{
  int rv = 1;
  rv = getaddrinfo(server.c_str(), port.c_str(), &hints, &servinfo);
// for debugging:  fprintf(stderr, "getaddrinfo: %i\n", rv);
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

bool Socket::connect ()
{
  struct addrinfo *p;
  int rv, connected;
  char s[INET6_ADDRSTRLEN];

  for(p = servinfo; p != NULL; p = p->ai_next) {
    m_sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (m_sock == -1) continue;
    int connected = ::connect(m_sock, p->ai_addr, p->ai_addrlen);
    if (connected == -1){
      close(m_sock);
      continue;}
    break;
  }

  if (connected == -1) return false;

  inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
  return true;
}

const Socket& Socket::operator >> (std::string& s) const
{
  char buf [ MAXRECV + 1 ];
  s = "";
  memset ( buf, 0, MAXRECV + 1 );
  ::recv ( m_sock, buf, MAXRECV, 0 );
  s = buf;
  if(nofork)
  std::cout << s << "\n";
  }
  return *this;
}

const Socket& Socket::operator << (const std::string& s) const
{
  ::send ( m_sock, s.c_str(), s.size(), 0 );
  if (nofork){
  std::cout << "<-- " + s << "\n";
  }
}