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
 * \fn class about_me(bool a):module("About Me", a, PRIORITY_DONTCARE){ this->SetDesc("Returns the information about yourself"); }
 * \brief Tells you about youself (ie. Host, ident, etc.).
 * Uses irc_string and IsoHost to decode the message you said.
 */
class about_me : public module
{
  
public:
  about_me(bool a):module("About Me", a, PRIORITY_DONTCARE){ this->SetDesc("Returns the information about yourself"); }
  
  ModuleReturn run(SendMessage *Send, Flux::string rply, irc_string *reply)
  {
  if(reply->said("about me")){
	Send->notice(unick, "Raw: "+reply->raw_string);
	Send->notice(unick, "message: "+msg);
	Send->notice(unick, "Nickname: "+unick);
	Send->notice(unick, "Ident: "+reply->user);
	Send->notice(unick, "Host: "+host);
	Send->notice(unick, "Channel: "+chan);
	Send->notice(unick, "Fullhost: "+fullhost);
	log("%s requested information about themself.", unick.c_str());
  }
  if(reply->said("!decodehost")){
	Flux::string host = reply->params(1);
	IsoHost* Host = new IsoHost(host);
	Send->privmsg(chan, "Nick: %s", Host->nick.c_str());
	Send->privmsg(chan, "User: %s", Host->user.c_str());
	Send->privmsg(chan, "Host: %s", Host->host.c_str());
	Send->privmsg(chan, "Raw: %s", Host->raw.c_str());
	delete Host;
  }
  return MOD_RUN;
 }
};
/**
 * @}
 */
#endif