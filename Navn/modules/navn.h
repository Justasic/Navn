#ifndef NAVN_H
#define NAVN_H
#include "../includes.h"
#include "../flux_net_irc.hpp"
/**
 * \file navn.h Header file holding the \a Navn functions.
 * \author Justasic.
 */

/**
 * \defgroup navnM About Me Module
 * This is the module for Navn's functions. \n
 * For a better description see the function description.
 * \section commands Commands associated with this module.
 * \subsection part /msg bot part
 * Send \a part with a channel as an arguement and the bot will part that channel
 * \subsection join /msg bot join
 * Send \a join with a channel as an arguement and the bot will join that channel
 * \subsection rejoin rejoin
 * Send \a rejoin in any way to the bot and it will rejoin the channel (command soon to be deprecated)
 * \subsection bugs !bugs
 * Say \a !bugs in any channel and the bot will give the flux-net bug tracker link
 * \subsection git ? git
 * Say \a ? \a git and the bot will give the git link for the repository
 * \subsection kick /msg bot kick
 * Send \a kick with the channel as the 1st arguement and the nickname as the second and the bot will kick the user from the channel.
 * 
 * @{
 */

/**
 * \fn void navn_h(Socket &sock, irc_string *reply)
 * \brief Most of navn's function code.
 * Navn's basic functions.
 */
using namespace std;
using namespace flux_net_irc;

void navn_h(Socket &sock, irc_string *reply){
  Privmsg s;
  if (reply->said("PRIVMSG "+nick+" :part")){
    Flux::string blah = reply->params(1);
    if(IsValadChannel(blah)){
      sock << notice(unick, "Channel %s is not a valad channel.", blah.c_str());
      log("%s attempted to make bot part %s", unick.c_str(), blah.c_str());
    }else{
      log("%s made the bot part %s", unick.c_str(), blah.c_str());
      sock << part(blah);
    }
  }
  if (reply->said("rejoin")){ //rejoin the channel
   if(!in_channel){
     sock << join(channel);
     sock << privmsg(channel, welcome_msg, nick.c_str());
     log("%s made the bot rejoin the channel", unick.c_str());
   }else{
     sock << notice(unick, "The bot is already in the channel.");
     log("%s attempted to rejoin the bot into a channel its already in.", unick.c_str());  
  }
  }
  if(reply->said("!bugs")){
    sock.Send(privmsg(chan, "Report Bugs at: http://flux-net.net/bugs/"));
  }
  if(reply->said("? git")){
    sock << notice(unick, "Navn git: git://gitorious.org/navn/navn.git");
    log("%s requested Git repository link.", unick.c_str());
  }
  if(reply->said("PRIVMSG "+nick+" :kick")){
    if(unick == owner_nick){
      Flux::string kickee = reply->params(2);
      Flux::string kickchan = reply->params(1);
      sock << kick(kickchan, kickee, "Kick from %s", unick.c_str());
    }else{
      sock << notice(unick, access_denied);
    }
  }
  if (reply->said("PRIVMSG "+nick+" :join")){
    Flux::string blah = reply->params(1);
    if(IsValadChannel(blah)){
      sock << notice(unick, "Channel %s is not a valad channel.", blah.c_str());
      log("%s attempted to make bot join %s", unick.c_str(), blah.c_str());
    }else{
      log("%s made the bot join %s", unick.c_str(), blah.c_str());
      sock << join(blah);
      sock << privmsg(blah, welcome_msg, nick.c_str(), nick.c_str());
    }
  }
  if(reply->said("PRIVMSG "+nick+" :nick")){
    Flux::string newnick = reply->params(1).c_str();
    for(unsigned i = 0, end = newnick.length(); i < end; i++){
      if(!isvalidnick(newnick[i])){
	s.notice(sock, unick, "\2%s\2 is an invalid nickname.");
	nick = newnick.c_str();
      }
    }
    if(unick == owner_nick){
      sock << setnick(newnick.c_str());
    }else{
     s.privmsg(sock, unick, access_denied); 
    }
  }
}
/**
 * @}
 */
#endif