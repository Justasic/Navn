/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#ifndef NAVN_H
#define NAVN_H
#include "includes.h"
#include "flux_net_irc.hpp"

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
    User *u = source.u;
    Channel *c = source.c;
    
    if (cmd.equals_ci("part")){
      Flux::string blah = params.size() == 2 ? params[1] : "";
      if(blah.empty()){
	source.Reply("Syntax: \2part \37channel\15");
	return MOD_STOP;
      }
      if(!IsValidChannel(blah)){
        source.Reply("Channel %s is not a valad channel.", blah.c_str());
        log(LOG_NORMAL, "%s attempted to make bot part %s", u->nick.c_str(), blah.c_str());
      }else{
	Channel *ch = findchannel(blah);
	if(!ch){
	 source.Reply("I am not in channel \2%s\2", blah.c_str()); 
	 return MOD_STOP;
	}
        log(LOG_NORMAL, "%s made the bot part %s", u->nick.c_str(), blah.c_str());
        ch->SendPart(blah);
      }
    }
  if(cmd.equals_ci("!bugs")){
    c->SendMessage("Report Bugs at: http://flux-net.net/bugs/");
  }
  if(cmd.equals_ci("!git")){
    source.Reply("Navn git: git://gitorious.org/navn/navn.git");
    log(LOG_NORMAL, "%s requested Git repository link.", u->nick.c_str());
  }
  if(cmd == "!finduser"){
    Flux::string username = params.size() == 2?params[1]:"";
    if(username.empty()){
      source.Reply("Syntax: \2!finduser \37user\15");
      return MOD_STOP;
    }
    User *u2 = finduser(username);
    if(!u2)
      source.Reply("User \2%s\2 does not exist.", username.c_str());
    else
      source.Reply("User: %s", u2->nick.c_str());
  }
  if(cmd.equals_ci("nick")){
    Flux::string newnick = params.size() == 2 ? params[1]:"";
    if(newnick.empty()){
      source.Reply("Syntax: \002nick \37newnickname\15"); 
      return MOD_STOP;
    }
    for(unsigned i = 0, end = newnick.length(); i < end; i++){
      if(!isvalidnick(newnick[i])){
	source.Reply("\2%s\2 is an invalid nickname.");
	nick = newnick;
      }
    }
    if(u->IsOwner()){
      Send->command->nick(newnick);
    }else{
     Send->privmsg(u->nick, ACCESS_DENIED); 
    }
  }
  return MOD_RUN;
}
};
/**
 * @}
 */
#endif