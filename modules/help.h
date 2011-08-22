#ifndef HELP_H
#define HELP_H
#include "../includes.h"
#include "../flux_net_irc.hpp"

/**
 * \file help.h Header file holding the \a help function.
 * \author Lordofsraam. Polished by Justasic.
 */

/**
 * \defgroup helpM Help Module
 * This is the module for the help_m function.
 * Returns help information about controlling Navn.
 * \section commands Commands associated with this module.
 * \subsection help Private message: help
 * Send a private message saying \a help to get information.
 * @{
 */

/**
 * \fn void help_m(Socket &sock, irc_string *reply)
 * \brief Returns help info on Navn.
 * Tells you most of the commands you can use on Navn.
 */
class help_m:public module{
public:
  help_m(bool a):module("Help", a, PRIORITY_DONTCARE){ this->SetDesc("The Help module"); }
  ModuleReturn run(CommandSource &source, std::vector<Flux::string> &params){
    Flux::string cmd = params.empty()?"":params[0];
  //help replies
    if(cmd == "help"){
      Send->notice(unick, "There are 10 commands:");
      Send->notice(unick, "quit \t \t \tQuits the bot (password needed)");
      Send->notice(unick, "pass\t \t \t Gets the quit password for the bot (must be a bot admin)");
      Send->notice(unick, "rejoin\t \t Rejoins the bot to the channel.");
      Send->notice(unick, "chown \t \t Changes ownership over the bot (must be a bot admin)");
      Send->notice(unick, "join \t \t \tTells the bot to join the specified channel.");
      Send->notice(unick, "part \t \t \tParts the channel");
      Send->notice(unick, "kick \t \t \tkicks a user from the channel (must be bot owner)");
      Send->notice(unick, "restart \t Restarts the Bot (Password needed)");
      Send->notice(unick, "rehash\t \t Rehashes the Bot (Password needed)");
      Send->notice(unick, "stats \t \t Shows system statistics.");
      Send->notice(unick, "nick \t \t \tSets the bots nickname");
      
      log("%s used help command", unick.c_str());
   }
   return MOD_RUN;
  }
};
/**
 * @}
 */
#endif