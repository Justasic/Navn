#ifndef PING_PONG_H
#define PING_PONG_H
#include "../includes.h"
#include "../flux_net_irc.hpp"
using namespace std;
using namespace flux_net_irc;
void ping_pong(Socket &sock, irc_string *reply, string re){

  if (reply->said("PING :")){
    sock << "PONG :"+re.substr(6,-1);
  }
  
}
#endif