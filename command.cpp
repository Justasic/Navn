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
 * \overload void Command::part(Flux::string channel, Flux::string msg)
 */
void Command::part(Flux::string channel, Flux::string msg){
  this->raw("PART %s %s\n", channel.c_str(), msg.c_str());
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
void Command::nick(Flux::string nick){
  this->raw("NICK %s\n", nick.c_str());
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
void Command::join(Flux::string chan){
  this->raw("JOIN %s\n", chan.c_str());
}
/** 
 * \overload void command::part(Flux::string channel)
 * \brief Parts channel w/o reason.
 * \param channel Channel to part from.
 */
void Command::part(Flux::string chan){
  this->raw("PART %s\n", chan.c_str()); 
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
