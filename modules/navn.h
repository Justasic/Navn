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
  ModuleReturn run(CommandSource &source, std::vector<Flux::string> &params){
    Flux::string cmd = params.empty()?"":params[0];
    
    if (cmd == "part"){
      Flux::string blah = params.size() == 2 ? params[1] : "";
      if(blah.empty()){
	Send->notice(unick, "Syntax: \2part \37channel\15");
	return MOD_STOP;
      }
      if(!IsValidChannel(blah)){
        Send->notice(unick, "Channel %s is not a valad channel.", blah.c_str());
        log("%s attempted to make bot part %s", unick.c_str(), blah.c_str());
      }else{
        log("%s made the bot part %s", unick.c_str(), blah.c_str());
        Send->command->part(blah);
      }
    }
  if(cmd == "!bugs"){
    Send->privmsg(chan, "Report Bugs at: http://flux-net.net/bugs/");
  }
  if(cmd == "!git"){
    Send->notice(unick, "Navn git: git://gitorious.org/navn/navn.git");
    log("%s requested Git repository link.", unick.c_str());
  }
  if(cmd == "kick"){
    if(unick == owner_nick){
      if(params.size() > 2 || params.empty() || params.size() < 2){
	Send->notice(unick, "Syntax: \2kick channel \37nick\15"); 
	return MOD_STOP;
      }
      Flux::string kickchan = params.size() == 2 ? params[1] : "";
      Flux::string kickee = params.size() == 2 ? params[2] : "";
      if(kickee.empty() || kickchan.empty()){
	Send->notice(unick, "Syntax: \2kick channel \37nick\15");
	return MOD_STOP;
      }
      if(!IsValidChannel(kickchan)){
	Send->notice(unick, "Channel \2%s\2 is not a valad channel.", kickchan.c_str()); 
	return MOD_STOP;
      }
      
      Send->command->kick(kickchan, kickee, "Kick from %s", unick.c_str());
    }else{
      Send->notice(unick, access_denied);
    }
  }
  if (cmd == "join"){
    Flux::string blah = params.size() == 2 ? params[1] : "";
    if(blah.empty()){
      Send->notice(unick, "Syntax: \2Join \37channel\15"); 
      return MOD_STOP;
    }
    if(!IsValidChannel(blah)){
      Send->notice(unick, "Channel \2%s\2 is not a valad channel.", blah.c_str());
      log("%s attempted to make bot join %s", unick.c_str(), blah.c_str());
    }else{
      log("%s made the bot join %s", unick.c_str(), blah.c_str());
      Send->command->join(blah);
      Send->privmsg(blah, welcome_msg, nick.c_str(), nick.c_str());
    }
  }
  if(cmd == "nick"){
    Flux::string newnick = params.size() == 2 ? params[1]:"";
    if(newnick.empty()){
      Send->notice(unick, "Syntax: \002nick \37newnickname\15"); 
      return MOD_STOP;
    }
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