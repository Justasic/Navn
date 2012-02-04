#ifndef ABOUT_ME_H
#define ABOUT_ME_H
#include "../includes.h"
#include "../flux_net_irc.hpp"
/**
 * \file about_me.h Header file holding the \a about_me function.
 * \author Justasic. Polished by Lordofsraam.
 */

/**
 * \defgroup about_meM About Me Module
 * This is the module for the about me function. \n
 * This module was originally a test/example module that grew. \n
 * The module just tells you about you and your hostname in IRC (nickname, channel it was said in, hostname, etc.) \n
 * For a better description see the function description.
 * \section commands Commands associated with this module.
 * \subsection about_me about me
 * Say \a about \a me to see your various host information that navn can see and decode.
 * \subsection decodehost !decodehost
 * Say \a !decodehost with a hostname and navn will decode it into ident, nickname, hostname, etc.
 * @{
 */

/**
 * \fn void about_me(Socket &sock, irc_string *reply)
 * \brief Tells you about youself (ie. Host, ident, etc.).
 * Uses irc_string and IsoHost to decode the message you said.
 */
using namespace std;
using namespace flux_net_irc;
void about_me(Socket &sock, irc_string *reply){
  Privmsg s;
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
    if(reply->said("!decodehost")){
	Flux::string nerp = reply->params(1);
	IsoHost* Host = new IsoHost(nerp);
	sock << privmsg(chan, "Nick: %s", Host->nick.c_str());
	sock << privmsg(chan, "User: %s", Host->user.c_str());
	sock << privmsg(chan, "Host: %s", Host->host.c_str());
	sock << privmsg(chan, "Raw: %s", Host->raw.c_str());
	delete Host;
    }
    if(reply->said("TEST!")){
      Flux::string Test = "Test \nTest2";
     s.privmsg(sock, chan, "NERP!\n %s", unick.c_str());
     s.notice(sock, chan, Test.strip());
    }
}
/**
 * @}
 */
#endif