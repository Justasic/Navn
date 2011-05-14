#ifndef HELP_H
#define HELP_H
#include "../includes.h"
#include "../flux_net_irc.hpp"
using namespace std;
using namespace flux_net_irc;
void help_m(Socket &sock, irc_string *reply){

  //help replies
    if(reply->said("PRIVMSG "+nick+" :help")){
      sock << notice(unick, "These are the commands: (precede all of them with !)");
      sock << notice(unick, "quit \t \t \tQuits the bot (password needed)");
      sock << notice(unick, "pass\t \t \t Gets the quit password for the bot (must be a bot admin)");\
      sock << notice(unick, "rejoin\t \t Rejoins the bot to the channel.");
      sock << notice(unick, "chown \t \t Changes ownership over the bot (must be a bot admin)");
      sock << notice(unick, "google\t \t Googles something.");
      sock << notice(unick, "youtube \t Youtubes something.");
      sock << notice(unick, "join \t \t \tTells the bot to join the specified channel.");
      sock << notice(unick, "part \t \t \tParts the channel");
      sock << notice(unick, "kick \t \t \tkicks a user from the channel (must be bot owner)");
      sock << notice(unick, "restart \t Restarts the Bot (Password needed)");
      sock << notice(unick, "rehash\t \t Rehashes the Bot (Password needed)");
      sock << notice(unick, "stats \t \t Shows system statistics.");
      sock << notice(unick, "World Clock Module:");
      sock << notice(unick, "time \t \t \tShow time around the world");
      sock << notice(unick, "Da_Goat Module:");
      sock << notice(unick, "info \t \t \tShow info about flux-net");
      sock << notice(unick, "poke \t \t \tTry it");
      sock << notice(unick, "register \tHow-to on registering your nick in IRC");
      sock << notice(unick, "socialinfo\tShow social info about flux-net");
      sock << notice(unick, "rules\t \t \tShow flux-net rules");
      sock << notice(unick, "spam \t \t \tflux-net definition of spam");
      sock << notice(unick, "version\t \tDah_Goat module version");
      sock << notice(unick, "DNS Module:");
      sock << notice(unick, "dns  \t \t \tGet IP from site name");
      sock << notice(unick, "rdns \t \t \tGet site name from IP");
      sock << notice(unick, "About Me Module:");
      sock << notice(unick, "about me \tShow info about yourself");
      log("%s used help command", unick.c_str());
  }

}
#endif