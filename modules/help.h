#ifndef HELP_H
#define HELP_H
#include "../includes.h"
#include "../flux_net_irc.hpp"
using namespace std;
using namespace flux_net_irc;

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

void help_m(Socket &sock, irc_string *reply){

  //help replies
    if(reply->said("PRIVMSG "+nick+" :help")){
      sock << notice(unick, "There are 11 commands:");
      sock << notice(unick, "quit \t \t \tQuits the bot (password needed)");
      sock << notice(unick, "pass\t \t \t Gets the quit password for the bot (must be a bot admin)");
      sock << notice(unick, "gdb\t \t \t \tDisplays how to use gdb.");
      sock << notice(unick, "rejoin\t \t Rejoins the bot to the channel.");
      sock << notice(unick, "chown \t \t Changes ownership over the bot (must be a bot admin)");
      sock << notice(unick, "join \t \t \tTells the bot to join the specified channel.");
      sock << notice(unick, "part \t \t \tParts the channel");
      sock << notice(unick, "kick \t \t \tkicks a user from the channel (must be bot owner)");
      sock << notice(unick, "restart \t Restarts the Bot (Password needed)");
      sock << notice(unick, "rehash\t \t Rehashes the Bot (Password needed)");
      sock << notice(unick, "stats \t \t Shows system statistics.");
      log("%s used help command", unick.c_str());
  }

}
#endif