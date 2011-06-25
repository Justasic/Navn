/* Socket.cpp */

#include "Socket.h"
#include "string.h"
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <iostream>

using namespace std;

string strip(const string &buf){
	string newbuf = buf;
	char c = newbuf[newbuf.size() - 1];
	while (c == '\n' || c == '\r'){
		newbuf.erase(newbuf.end() - 1);
		c = newbuf[newbuf.size() - 1];
	}
	return newbuf;
}

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
void recvlog(const char *fmt, ...){
  fstream log;
  log.open("Raw-recv-socket.log", fstream::in | fstream::out | fstream::app);
  if(!log.is_open())
     cerr << "Failed to open log file.\n";
  va_list args;
  va_start(args, fmt);
  time_t t = time(NULL);
  struct tm *tm = localtime(&t);
  char buf[512];
  strftime(buf, sizeof(buf) - 1, "[%b %d %H:%M:%S %Y] ", tm);
  log << buf;
  vsnprintf(buf, sizeof(buf), fmt, args);
  log << buf << endl;
  va_end(args);
  va_end(args);
  log.close();
}
const Socket& Socket::operator >> (std::string& s) const
{
  char buf [ MAXRECV + 1 ];
  s = "";
  memset ( buf, 0, MAXRECV + 1 );
  ::recv ( m_sock, buf, MAXRECV, 0 );
  s = buf;
  std::cout << strip(s) << "\n";
  //recvlog("%s", s.c_str());
  return *this;
}
void sendlog(const char *fmt, ...){
  fstream log;
  log.open("Raw-send-socket.log", fstream::in | fstream::out | fstream::app);
  if(!log.is_open())
     cerr << "Failed to open log file.\n";
  va_list args;
  va_start(args, fmt);
  time_t t = time(NULL);
  struct tm *tm = localtime(&t);
  char buf[512];
  strftime(buf, sizeof(buf) - 1, "[%b %d %H:%M:%S %Y] ", tm);
  log << buf;
  vsnprintf(buf, sizeof(buf), fmt, args);
  log << buf << endl;
  va_end(args);
  va_end(args);
  log.close();
}
const Socket& Socket::operator << (const std::string& s) const
{
  ::send ( m_sock, s.c_str(), s.size(), 0 );
  std::cout << "<-- " + strip(s) << "\n";
  //sendlog("%s", s.c_str());
}
void Socket::Send(const std::string& s) const
{
	::send ( m_sock, s.c_str(), s.size(), 0 );
	std::cout << "<-- " << strip(s) << "\n";
	//sendlog("%s", s.c_str());
}
