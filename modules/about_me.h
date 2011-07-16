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
class about_me : module 
{

public:
  about_me(string n, bool a):module(n,a){}
  
  void run(string rply, irc_string *reply)
  {
  
		if(reply->said("about me"))
		{
			Send::Message::notice(unick, "Raw: "+reply->raw_string);
			Send::Message::notice(unick, "message: "+msg);
			Send::Message::notice(unick, "Nickname: "+unick);
			Send::Message::notice(unick, "Ident: "+reply->user);
			Send::Message::notice(unick, "Host: "+host);
			Send::Message::notice(unick, "Channel: "+chan);
			Send::Message::notice(unick, "Fullhost: "+fullhost);
			log("%s requested information about themself.", unick.c_str());
		}

		if(reply->said("!decodehost"))
		{
			string nerp = reply->params(1);
			IsoHost* Host = new IsoHost(nerp);
			Send::Message::priv(chan, "Nick: %s", Host->nick.c_str());
			Send::Message::priv(chan, "User: %s", Host->user.c_str());
			Send::Message::priv(chan, "Host: %s", Host->host.c_str());
			Send::Message::priv(chan, "Raw: %s", Host->raw.c_str());
			delete Host;
		}
  
  }

};

/**
 * @}
 */
#endif