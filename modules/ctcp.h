#ifndef CTCP_H
#define CTCP_H
#include "../includes.h"
#include "../flux_net_irc.hpp"
const string CTCP_VERS = " :\001VERSION\001";
const string CTCP_TIME = " :\001TIME\001";
const string CTCP_SOURCE = " :\001SOURCE\001";
using namespace std;
using namespace flux_net_irc;
void ctcp_m(Socket &sock, irc_string *reply){

  if(reply->said(CTCP_VERS)){ // for CTCP VERSION reply
    cout << "\033[22;31mRecieved CTCP VERSION from "+unick+"\033[22;36m\r\n";
    struct utsname uts;
    if(uname(&uts) < 0)
	    //perror("uname() error");
	    throw CoreException("uname() Error");

    sock << notice(unick, "\001VERSION Navn Bot %s %s %s\001",version.c_str(), uts.sysname, uts.machine);
    log("Recieved CTCP VERSION from %s", unick.c_str());
   }
  if(reply->said(CTCP_TIME)){ // for CTCP TIME reply
    cout << "\033[22;31mRecieved CTCP TIME from "+unick+"\033[22;36m\r\n";
    sock << notice(unick, "\001TIME "+os_time()+"\001");
    log("Recieved CTCP TIME from %s", unick.c_str());
  }
  if(reply->said(CTCP_SOURCE)){
    cout << "\033[22;31mRecieved CTCP SOURCE from "+unick+"\033[22;36m\r\n";
    sock << notice(unick, "\001SOURCE https://gitorious.org/navn/navn\001");
    sock << notice(unick, "\001SOURCE http://flux-net.googlecode.com/svn/branches/Navn/\001");
    log("Recieved CTCP SOURCE from %s", unick.c_str());
  }
}
#endif
