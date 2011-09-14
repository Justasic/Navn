/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#ifndef SYSTEM_H
#define SYSTEM_H

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

class system_m:public module{
public:
  system_m(bool a):module("System", a, PRIORITY_FIRST){ this->SetDesc("The system module"); }
ModuleReturn run(CommandSource &source, std::vector<Flux::string> &params){
  /* use string to prevent segmentation faults */
  Flux::string cmd = params.empty()?"":params[0];
  User *u = source.u;
  Channel *c = source.c;
  
  if (cmd.equals_ci("pass")){
    if (u->IsOwner()){
      source.Reply("The password is:\2 %s", password.c_str());
      log(LOG_NORMAL, "%s requested the navn quit password: %s", u->nick.c_str(), password.c_str());
    }else{
      source.Reply(ACCESS_DENIED);
      log(LOG_NORMAL, "%s attempted to request the navn quit password.", u->nick.c_str());
    }
  }
  if (cmd.equals_ci("restart")){
    if(u->IsOwner()){
      //Flux::string reason = "-no reason-";
      Send->raw("QUIT :Restarting..\n");
      restart("Restarting..");
    }else{
      source.Reply(ACCESS_DENIED);
    }
  }
  if(cmd.equals_ci("pid")){
    if(u->IsOwner()){
      source.Reply("My PID is: %i", getpid()); 
      log(LOG_NORMAL, "%s used pid function to get PID %i", u->nick.c_str(), getpid());
    }else{
      source.Reply(ACCESS_DENIED);
    }
  }
  if(source.command == "NICK"){
    IsoHost *Host = new IsoHost(u->fullhost);
    Flux::string newnick = Host->nick;
    if (newnick == nick)
      nick = newnick;
    if(newnick == owner_nick)
      owner_nick = newnick;
    delete Host;
  }
  if (cmd.equals_ci("quit")){ //quits the bot.
    Flux::string pass = params.size() == 2 ? params[1] : "";
    if(pass.empty()){
     source.Reply("Syntax: \2quit \37password\15");
     return MOD_STOP;
    }
    if(pass == password || pass == usrpass){
      source.Reply("Quitting..");
      log(LOG_NORMAL, "%s quit the bot with password: \"%s\"", u->nick.c_str(), password.c_str());
      Send->command->quit("Requested From \2%s\17. Pass: \00320%s\017", u->nick.c_str(), password.c_str());
      quitting = true;
    }else{
      source.Reply(ACCESS_DENIED);
      log(LOG_NORMAL, "%s attempted to change ownership of the bot", u->nick.c_str());
    }
  }
  if(cmd.equals_ci("rehash")){
    Flux::string pass = params.size() > 0 ? params[0] : "";
    if(u->nick == owner_nick || pass == password || pass == usrpass){
      source.Reply("Rehashing config file.");
      log(LOG_NORMAL, "%s rehashed config file.", u->nick.c_str());
      Rehash();
    }else{
      source.Reply(ACCESS_DENIED);
      log(LOG_NORMAL, "%s attempted a rehash.", u->nick.c_str());
    }
  }
  if(cmd.equals_ci("chown")){
    Flux::string pass = params.size() > 0 ? params[0] : "";
    if(pass == password || pass == usrpass){
      log(LOG_NORMAL, "Changing ownership from %s to %s", owner_nick.c_str(), u->nick.c_str());
      owner_nick = u->nick; //FIXME: i am broken from the new API!
      source.Reply("New owner for \2%s\2 is \2%s\2", nick.c_str(), owner_nick.c_str());
    }else{
      source.Reply(ACCESS_DENIED);
      log(LOG_NORMAL, "%s attempted to change ownership of the bot", u->nick.c_str());
    }
  }
  /*Find the join numeric (376)
   * This is so you can make things happen only if you know you are in
   * a channel.
   */
  if(source.command == "376"){
    std::cout << "\033[22;31mStarted with PID \033[22;32m" << getpid() << "\033[22;36m" << nl;
    std::cout << "\033[22;34mSession Password: \033[01;32m"+password+"\033[22;36m"<<nl;
    Send->notice(owner_nick, "The randomly generated password is: "+password);
  }
  if(cmd.equals_ci("!chanlist")){
    ListChans(source);
  }
  if(cmd.equals_ci("!userlist")){
    ListUsers(source);
  }
  if(source.command == "004"){
    Send->command->mode(nick, "+B");
    Channel *chan = new Channel(channel);
    chan->SendJoin();
    chan->SendMessage(welcome_msg, nick.c_str(), nick.c_str());
    if(!ouser.empty() || !opass.empty()){
      Send->command->oper(ouser, opass);
      IsOper = true;
      Send->o = new Oper(sock);
    }
    if(!nsacc.empty() || !nspass.empty()){
      Send->privmsg("NickServ", "identify %s %s", nsacc.c_str(), nspass.c_str());
    }
    log(LOG_NORMAL, "Successfully connected to the server \"%s:%s\" Master Channel: %s", server.c_str(), port.c_str(), channel.c_str());
  }
  if(source.command == "433"){
   nick.push_back('_');
   Send->command->nick(nick);
  }
  if(cmd == "\001DCC"){
    source.Reply("I do not accept or support DCC connections.");
  }
  if(irc_string::said(source.message, "This nickname is registered and protected. If it is your")){
    if(!nsacc.empty() || !nspass.empty())
      return MOD_STOP;
    u->SendPrivmsg("NickServ", "identify %s %s", nsacc.c_str(), nspass.c_str());
    log(LOG_NORMAL, "Identified to NickServ with account \"%s\"", nsacc.c_str());
  }
  if (source.command == "JOIN"){ //welcomes everyone who joins the channel
    if(!(u->nick == nick)){
      source.Reply("Welcome %s to %s. Type !time for time or \"/msg %s help\" for help on more commands.", u->nick.c_str(), c->name.c_str(), nick.c_str());
      log(LOG_NORMAL, "%s joined %s", u->nick.c_str(), c->name.c_str());
    }
  }
  if(source.command == "KICK"){
    Flux::string reason = source.params[2].empty()?"":source.params[2];
    printf("%s kicked %s (%s)\n", u->nick.c_str(), cmd.c_str(), reason.c_str());
    if(source.params[1] == nick){
      Send->notice(owner_nick, "%s kicked me from %s! (%s)", u->nick.c_str(), c->name.c_str(), reason.c_str());
      c->SendJoin();
      log(LOG_NORMAL, "%s kicked bot out of channel %s (%s)", u->nick.c_str(), c->name.c_str(), reason.c_str());
    }
  }
  return MOD_RUN;
}
};
/**
 * @}
 */
#endif
