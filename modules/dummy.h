#ifndef DUMMY_H
#define DUMMY_H
#include "../includes.h"
#include "../flux_net_irc.hpp"
using namespace std;
using namespace flux_net_irc;
void dummy(Socket &sock, irc_string *reply){

  if (reply->said("testing testing")){
    sock << chanmsg("I hear you!");
  }

}
#endif
