#ifndef DUMMY_H
#define DUMMY_H
#include "../includes.h"
#include "../flux_net_irc.hpp"
using namespace std;
using namespace flux_net_irc;
void ChannelLogger(Socket &sock, irc_string *reply){

  if(reply->said() != NULL){
     // This is where Justasic plans on starting a channel logger
     
     
  }
}
#endif
