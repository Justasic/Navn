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
  if (reply->said("PRIVMSG "+nick+" :part")){
    string blah = reply->params(1);
    if(IsValadChannel(blah)){
      sock << notice(unick, "Channel %s is not a valad channel.", blah.c_str());
      log("%s attempted to make bot part %s", unick.c_str(), blah.c_str());
    }else{
      log("%s made the bot part %s", unick.c_str(), blah.c_str());
      sock << part(blah);
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
      string kickee = reply->params(2);
      string kickchan = reply->params(1);
      sock << kick(kickchan, kickee, "Kick from %s", unick.c_str());
    }else{
      sock << notice(unick, access_denied);
    }
  }
  if (reply->said("PRIVMSG "+nick+" :join")){
    string blah = reply->params(1);
    if(IsValadChannel(blah)){
      sock << notice(unick, "Channel %s is not a valad channel.", blah.c_str());
      log("%s attempted to make bot join %s", unick.c_str(), blah.c_str());
    }else{
      log("%s made the bot join %s", unick.c_str(), blah.c_str());
      sock << join(blah);
      sock << privmsg(blah, welcome_msg, nick.c_str(), nick.c_str());
    }
  }
}
/**
 * @}
 */
#endif