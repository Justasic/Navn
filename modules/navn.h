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
 * \subsection git !git
 * Say \a !git and the bot will give the git link for the repository
 * \subsection kick /msg bot kick
 * Send \a kick with the channel as the 1st arguement and the nickname as the second and the bot will kick the user from the channel.
 * 
 * @{
 */

/**
 * \fn class navn(bool a):module("Navn", a, PRIORITY_DONTCARE){ this->SetDesc("Where some of the navn specific functions are");}
 * \brief Most of navn's function code.
 * Navn's basic functions.
 */
class navn:public module{
public:
  navn(bool a):module("Navn", a, PRIORITY_DONTCARE){ this->SetDesc("Where some of the navn specific functions are");}
  ModuleReturn run(SendMessage *Send, Flux::string rply, irc_string *reply){
    if (reply->said("PRIVMSG "+nick+" :part")){
      Flux::string blah = reply->params(1);
      if(!IsValidChannel(blah)){
        Send->notice(unick, "Channel %s is not a valad channel.", blah.c_str());
        log("%s attempted to make bot part %s", unick.c_str(), blah.c_str());
      }else{
        log("%s made the bot part %s", unick.c_str(), blah.c_str());
        Send->command->part(blah);
      }
    }
  if(reply->said("!bugs")){
    Send->privmsg(chan, "Report Bugs at: http://flux-net.net/bugs/");
  }
  if(reply->said("!git")){
    Send->notice(unick, "Navn git: git://gitorious.org/navn/navn.git");
    log("%s requested Git repository link.", unick.c_str());
  }
  if(reply->said("PRIVMSG "+nick+" :kick")){
    if(unick == owner_nick){
      Flux::string kickee = reply->params(2);
      Flux::string kickchan = reply->params(1);
      Send->command->kick(kickchan, kickee, "Kick from %s", unick.c_str());
    }else{
      Send->notice(unick, access_denied);
    }
  }
  if (reply->said("PRIVMSG "+nick+" :join")){
    Flux::string blah = reply->params(1);
    if(!IsValidChannel(blah)){
      Send->notice(unick, "Channel %s is not a valad channel.", blah.c_str());
      log("%s attempted to make bot join %s", unick.c_str(), blah.c_str());
    }else{
      log("%s made the bot join %s", unick.c_str(), blah.c_str());
      Send->command->join(blah);
      Send->privmsg(blah, welcome_msg, nick.c_str(), nick.c_str());
    }
  }
  if(reply->said("PRIVMSG "+nick+" :nick")){
    Flux::string newnick = reply->params(1).c_str();
    for(unsigned i = 0, end = newnick.length(); i < end; i++){
      if(!isvalidnick(newnick[i])){
	Send->notice(unick, "\2%s\2 is an invalid nickname.");
	nick = newnick;
      }
    }
    if(unick == owner_nick){
      Send->command->nick(newnick);
    }else{
     Send->privmsg(unick, access_denied); 
    }
  }
  return MOD_RUN;
}
};
/**
 * @}
 */
#endif