#ifndef SYSTEM_H
#define SYSTEM_H
#include "../includes.h"
#include "../flux_net_irc.hpp"
using namespace flux_net_irc;
/**
 * \file system.h Header file holding the \a core \a bot \a functions.
 * \author Justasic. Polished by Lordofsraam.
 */

/**
 * \defgroup systemM Bot System Module
 * This is the module for the basic IRC functions. \n
 * Ths module is basically just a module that holds all the function that me and lordofsraam dont know where they would fit
 * It holds most of the error handling functions (so it doesn't do what Justasic calls "10,000 returns" into the terminal (using high ammounts
 * of CPU and ram). It also houses some basic bot management functions and identifies when it joins a channel or when its been killed.
 * For a better description see the function description.
 * \section commands Commands associated with this module.
 * \subsection pass /msg bot pass
 * Send \a pass and the bot will give you (if you are the bot owner) the session password for other functions.
 * \subsection restart /msg bot restart
 * Send \a restart and the bot will (if you are the bot owner) terminate and do a full restary (generating a new PID)
 * \subsection pid /msg bot pid
 * Send \a pid and the bot will give you (if you are the bot owner) the session Process ID (PID).
 * \subsection quit /msg bot quit
 * Send \a quit with the randomly generated session password and the bot will quit IRC and shutdown.
 * \subsection rehash /msg bot rehash
 * Send \a rehash and the bot will (if you are the bot owner or know the config set passwd) the bot will re-read the config file.
 * \subsection chown /msg bot chown
 * Send \a chown with the 1 argument bein the new owner_nick and the bot will change ownership 
 * (if you know the bots passwd) to the specified user.
 * \subsection DCC DCC send's
 * If the bot recieves a DCC request, it is auto denied.
 * \subsection Killed Killed
 * If the bot is killed, it is to automatically shutdown.
 * @{
 */

/**
 * \fn class system_m(bool a):module("System", a, PRIORITY_FIRST){ this->SetDesc("The system module"); }
 * \brief module hook
 * Module hook for the module
 */
class system_m:module{
public:
  system_m(bool a):module("System", a, PRIORITY_FIRST){ this->SetDesc("The system module"); }
ModuleReturn run(SendMessage *Send, Flux::string rply, irc_string *reply){

  if (reply->said("PRIVMSG "+nick+" :pass")){
    if (unick == owner_nick){
      Send->notice(unick, "The password is:\2 "+password);
      log("%s requested the navn quit password: %s", unick.c_str(), password.c_str());
    }else{
      Send->notice(unick, access_denied);
      log("%s attempted to request the navn quit password.", unick.c_str());
    }
  }
  if (reply->said("PRIVMSG "+nick+" :restart")){
    if(reply->params(2) == password || reply->params(2) == usrpass){
      //Flux::string reason = "-no reason-";
      Send->raw("QUIT :Restarting..\n");
      restart("Restarting..");
    }else{
      Send->notice(unick, access_denied);
    }
  }
  if(reply->said("PRIVMSG "+nick+" :pid") || reply->said("PRIVMSG "+nick+" :PID")){
    if(unick == owner_nick){
      Send->notice(unick, "My PID is: %i", getpid()); 
      log("%s used pid function to get PID %i", unick.c_str(), getpid());
    }else{
      Send->notice(unick, access_denied);
    }
  }
  if(reply->said(" NICK ")){
    Flux::string newnickhost = reply->params(0).substr(0,-1);
    IsoHost *Host = new IsoHost(newnickhost);
    Flux::string newnick = Host->nick;
    if (newnick == nick)
      nick = reply->params(2);
  }
  if (reply->said("PRIVMSG "+nick+" :quit "+password)){ //quits the bot.
    Send->notice(unick, "Quitting..");
    log("%s quit the bot with password: \"%s\"", unick.c_str(), password.c_str());
    DoQuit(Send, "Requested From \2"+unick+"\17. Pass: \00320"+password+"\017");
  }
  if(reply->said("PRIVMSG "+nick+" :rehash")){
    Flux::string getpass = reply->params(1);
    if(unick == owner_nick || getpass == usrpass){
      Send->notice(unick, "Rehashing config file.");
      log("%s rehashed config file.", unick.c_str());
      Rehash(Send);
    }else{
      Send->notice(unick, access_denied);
      log("%s attempted a rehash.", unick.c_str());
    }
  }
  if(reply->said("PRIVMSG "+nick+" :chown")){
    Flux::string givinpass = reply->params(2);
    if(givinpass == usrpass){
      log("Changing ownership from %s to %s", owner_nick.c_str(), unick.c_str());
      owner_nick = reply->params(1);
      Send->notice(unick, "New owner for \2%s\2 is \2%s\2", nick.c_str(), owner_nick.c_str());
    }else{
      Send->notice(unick, access_denied);
      log("%s attempted to change ownership of the bot", unick.c_str());
    }
  }
  /*Find the join numeric (252)
   * If found it makes in_channel true.
   * This is so you can make things happen only if you know you are in
   * a channel.
   */
  if((reply->said("353 "+nick))){
    cout << "\033[22;31mChannel join confirmation\033[22;30m... \033[1m\033[22;32mCHECK\033[1m\033[22;36m"<<nl;
    cout << "\033[22;31mReading Config File\033[22;30m... \033[1m\033[22;32mCHECK\033[1m\033[22;36m"<<nl;
    cout << "\033[22;31mSending password to owner\033[22;30m... \033[22;32mCHECK\033[22;36m"<<nl;;
    cout << "\033[22;31mStarted with PID \033[22;32m" << getpid() << "\033[22;36m" << nl;
    cout << "\033[22;34mSession Password: \033[01;32m"+password+"\033[22;36m"<<nl;
    Send->notice(owner_nick, "The randomly generated password is: "+password);
    Send->privmsg(channel, welcome_msg, nick.c_str(), nick.c_str());
    in_channel = true;
  }
  if((reply->said("376 "+nick))){
    Send->command->mode(nick, "+B");
    Send->command->join(channel);
    if(ouser.empty() || opass.empty()){
    }else{
      Send->command->oper(ouser, opass);
      IsOper = true;
    }if(nsacc.empty() || nspass.empty()){
      return MOD_RUN;
    }else{
      Send->privmsg("NickServ", "identify "+nsacc+" "+nspass);
    }
    log("Successfully connected to the server \"%s:%s\" Master Channel: %s", server.c_str(), port.c_str(), channel.c_str());
  }
  if(reply->said("PRIVMSG "+nick+":DCC ")){
    Send->notice(unick, "I do not accept or support DCC connections.");
  }
  if(reply->isnumeric(482)){
    cout << "\033[22;31mI require op to preform this function\033[22;36m" << nl;
    Send->notice(owner_nick, "I require op to run the last command!");
    log("Op is required in %s", chan.c_str());  
  }
  if(reply->said("This nickname is registered and protected")){
    Send->privmsg(unick, "identify %s %s", nsacc.c_str(), nspass.c_str());
    log("Identified to NickServ with account \"%s\"", nsacc.c_str());
  }
  if (reply->said("JOIN :"+chan) && in_channel){ //welcomes everyone who joins the channel
    if(unick == nick){return MOD_RUN;}else{
      Send->notice(unick, "Welcome "+unick+" to "+strip(chan)+". Type !time for time or \"/msg "+nick+" help\" for help on more commands.");
      log("%s joined %s", unick.c_str(), strip(chan).c_str());
    }
  }
  if(reply->said("KICK "+chan+" "+nick)){
    Send->notice(owner_nick, "%s kicked me from %s!", unick.c_str(), chan.c_str());
    Send->command->join(chan);
    in_channel = false;
    log("%s kicked bot out of channel %s", unick.c_str(), chan.c_str());
  }
  return MOD_RUN;
}
};
/**
 * @}
 */
#endif
