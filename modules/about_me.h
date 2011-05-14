#ifndef ABOUT_ME_H
#define ABOUT_ME_H
#include "../includes.h"
#include "../flux_net_irc.hpp"
using namespace std;
using namespace flux_net_irc;
void about_me(Socket &sock, irc_string *reply){

  if(reply->said("about me")){
    sock << notice(unick, "Raw: "+reply->raw_string);
    sock << notice(unick, "message: "+msg);
    sock << notice(unick, "Nickname: "+unick);
    sock << notice(unick, "Ident: "+reply->user);
    sock << notice(unick, "Host: "+host);
    sock << notice(unick, "Channel: "+chan);
    sock << notice(unick, "Fullhost: "+fullhost);
    log("%s requested information about themself.", unick.c_str());
   }

}
#endif