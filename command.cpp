#include "command.h"
/**
 *\file  command.cpp 
 *\brief Contains the command class.
 */
Command::Command(SocketIO *sock){
  this->s = sock;
}
/**
 * \fn void command::raw(const char *fmt, ...)
 * \brief Sends data straight to the socket engine
 */
void Command::raw(const char *fmt, ...){
  char buffer[4096] = "";
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  this->s->send(buffer);
  va_end(args);
}
/**
 * \fn void command::kick(Flux::string Channel, Flux::string User, const char *fmt, ...)
 * \brief Handles kick requests
 * \param channel Channel to be kicked from.
 * \param user User to be kicked.
 * \param reason Reason for the kick.
 */
void Command::kick(Flux::string Channel, Flux::string User, const char *fmt, ...){
  char buffer[4096] = "";
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  this->kick(Channel, User, Flux::string(buffer));
  va_end(args);
}
/**
 * \fn void command::topic(Flux::string channel, const char *fmt, ...)
 * \brief Sets channel topic.
 */
void Command::topic(Flux::string channel, const char *fmt, ...){
  char buffer[4096] = "";
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  this->topic(channel, Flux::string(buffer));
  va_end(args);
}
/**
 * \fn void Command::quit(const char *fmt, ...)
 * \brief Handles quitting of irc
 * \param message Quit message
 */
void Command::quit(const char *fmt, ...){
  char buffer[4096] = "";
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  this->quit(Flux::string(buffer));
  va_end(args);
}
/** 
 * \fn void command::part(Flux::string channel, Flux::string reason)
 * \brief Sends part with message
 * \param channel Channel to part from.
 * \param reason Reason for parting.
 */
void Command::part(Flux::string channel, const char *fmt, ...){
  char buffer[4096] = "";
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  this->part(channel, Flux::string(buffer));
  va_end(args);
}
/**
 * \overload void command::kick(Flux::string channel, Flux::string user, Flux::string reason)
 */
void Command::kick(Flux::string chan, Flux::string user, Flux::string msg){
  this->raw("KICK %s %s :%s\n", chan.c_str(), user.c_str(), msg.c_str());
}
/**
 * \overload void Command::quit(Flux::string message)
 */
void Command::quit(Flux::string message){
 this->raw("QUIT :%s", message.c_str());
}
/**
 * \overload void Command::part(Flux::string channel, Flux::string msg)
 */
void Command::part(Flux::string channel, Flux::string msg){
  this->raw("PART %s :%s\n", channel.c_str(), msg.c_str());
}
/**
 * \overload void command::topic(Flux::string channel, Flux::string msg)
 */
void Command::topic(Flux::string chan, Flux::string msg){
  this->raw("TOPIC %s :%s\n", chan.c_str(), msg.c_str());
}
/** 
 * \fn void command::nick(Flux::string nick)
 * \brief Sets the bots nickname in IRC.
 * \param nickname A Flux::string with the new nickname.
 */
void Command::nick(Flux::string bnick){
  this->raw("NICK %s\n", bnick.c_str());
}
/**
 * \fn void command::oper(Flux::string oper, Flux::string password)
 * \brief Sends IRC command /oper
 */
void Command::oper(Flux::string oper, Flux::string password){
  this->raw("OPER %s %s\n", oper.c_str(), password.c_str());
}
/** 
 * \fn void command::join(Flux::string chan)
 * \brief Makes the bot join a channel
 * \param stringy_chan A Flux::string with the channel you want to join.
 */
void Command::join(Flux::string dchan){
  this->raw("JOIN %s\n", dchan.c_str());
}
/** 
 * \overload void command::part(Flux::string channel)
 * \brief Parts channel w/o reason.
 * \param channel Channel to part from.
 */
void Command::part(Flux::string fchan){
  this->raw("PART %s\n", fchan.c_str()); 
}
/** 
 * \fn void command::whois(Flux::string Nick)
 * \brief Sends a IRC Whois to Server.
 * \param Nick Nick to query
 */
void Command::whois(Flux::string user){
  this->raw("WHOIS %s\n", user.c_str());
}
/** 
 * \fn void command::mode(Flux::string nickname, Flux::string mode, Flux::string user)
 * \brief Sends a mode to be set in IRC
 * \param nickname Nickname of who we are setting a more to.
 * \param mode The mode to set.
 */
void Command::mode(Flux::string chan, Flux::string mode, Flux::string user){
  this->raw("MODE %s %s %s\n", chan.c_str(), mode.c_str(), user.c_str());
}
/**
 *\overload void command::mode(Flux:;string dest, Flux::string mode)
 * \brief Sends a mode to the server
 * @param dest where to set the mode
 * @param mode mode to set
 */
void Command::mode(Flux::string dest, Flux::string mode){
  this->raw("MODE %s %s\n", dest.c_str(), mode.c_str()); 
}
/***************************************************************************************/
Oper::Oper(SocketIO *sock){
  this->s = sock;
}
void Oper::raw(const char *fmt, ...){
  char buffer[4096] = "";
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  this->s->send(buffer);
  va_end(args);
}
void Oper::samode(Flux::string target, Flux::string mode){
  this->raw("SAMODE %s %s\n", target.c_str(), mode.c_str());
}
void Oper::samode(Flux::string target, Flux::string mode, Flux::string params){
  this->raw("SAMODE %s %s %s", target.c_str(), mode.c_str(), params.c_str());
}
void Oper::sajoin(Flux::string target, Flux::string channel){
  this->raw("SAJOIN %s %s", target.c_str(), channel.c_str());
}
void Oper::sapart(Flux::string target, Flux::string channel){
  this->raw("SAPART %s %s", target.c_str(), channel.c_str());
}
void Oper::sanick(Flux::string target, Flux::string nickname){
  this->raw("SANICK %s %s", target.c_str(), nickname.c_str());
}
void Oper::sakick(Flux::string channel, Flux::string target, Flux::string reason){
  this->raw("SAKICK %s %s %s", channel.c_str(), target.c_str(), reason.c_str());
}
void Oper::satopic(Flux::string target, Flux::string topic){
  this->raw("SATOPIC %s %s", target.c_str(), topic.c_str());
}
void Oper::satopic(Flux::string target, const char *fmt, ...){
  char buffer[4096] = "";
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  this->satopic(target, Flux::string(buffer));
  va_end(args); 
}
void Oper::sahost(Flux::string target, Flux::string host){
  this->raw("CHGHOST %s %s", target.c_str(), host.c_str());
}
void Oper::saident(Flux::string target, Flux::string ident){
  this->raw("CHGIDENT %s %s", target.c_str(), ident.c_str());
}
void Oper::kill(Flux::string target, Flux::string reason){
  this->raw("KILL %s %s", target.c_str(), reason.c_str());
}
void Oper::saname(Flux::string target, Flux::string name){
  this->raw("CHGNAME %s %s", target.c_str(), name.c_str());
}
void Oper::saname(Flux::string target, const char *fmt, ...){
  char buffer[4096] = "";
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  this->saname(target, Flux::string(buffer));
  va_end(args); 
}
void Oper::wallops(Flux::string message){
  this->raw("WALLOPS %s", message.c_str());
}
void Oper::wallops(const char *fmt, ...){
  char buffer[4096] = "";
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  this->wallops(Flux::string(buffer));
  va_end(args); 
}
void Oper::globops(Flux::string message){
  this->raw("GLOBOPS %s", message.c_str());
}
void Oper::globops(const char *fmt, ...){
  char buffer[4096] = "";
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  this->globops(Flux::string(buffer));
  va_end(args); 
}
void Oper::zline(Flux::string ipmask, Flux::string time, Flux::string reason){
  this->raw("ZLINE %s %s %s", ipmask.c_str(), time.c_str(), reason.c_str());
}
void Oper::qline(Flux::string target, Flux::string time, Flux::string reason){
  this->raw("QLINE %s %s %s", target.c_str(), time.c_str(), reason.c_str());
}
void Oper::kline(Flux::string target, Flux::string time, Flux::string reason){
  this->raw("KLINE %s %s %s", target.c_str(), time.c_str(), reason.c_str());
}
void Oper::gline(Flux::string target, Flux::string time, Flux::string reason){
  this->raw("GLINE %s %s %s", target.c_str(), time.c_str(), reason.c_str());
}