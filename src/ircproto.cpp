/* Navn IRC bot -- IRC Protocol classes
 * 
 * (C) 2011-2012 Flux-Net
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */
#include "ircproto.h"
/**
 *\file  ircproto.cpp
 *\brief Contains the IRCProto class.
 */
IRCProto::IRCProto() {}
/**
 * \brief Sends a IRC private message to the user or channel
 * \param Destination Where the message will go
 * \param Message The message to send to Destination 
 */
void IRCProto::privmsg(const Flux::string &where, const char *fmt, ...){
  va_list args;
  char buffer[BUFSIZE] = "";
  if(fmt)
  {
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    this->privmsg(where, Flux::string(buffer));
    va_end(args);
  }
}
/**
 * \overload void IRCProto::privmsg(Flux::string where, Flux::string msg)
 * \brief Sends a IRC private message to the user or channel
 */
void IRCProto::privmsg(const Flux::string &where, const Flux::string &msg){
 sepstream sep(msg, '\n');
 Flux::string tok;
 while(sep.GetToken(tok))
   send_cmd("PRIVMSG %s :%s\n", where.c_str(), tok.c_str());
}
/**
 * \brief Sends a IRC notice to the user or channel
 * \param Destination Where the message will go
 * \param Message The message to send to Destination 
 */
void IRCProto::notice(const Flux::string &where, const char *fmt, ...){
  va_list args;
  char buffer[BUFSIZE] = "";
  if(fmt)
  {
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    this->notice(where, Flux::string(buffer));
    va_end(args);
  }
}
/**
 * \overload void IRCProto::notice(Flux::string where, Flux::string msg)
 * \brief Sends a IRC notice to the user or channel
 */
void IRCProto::notice(const Flux::string &where, const Flux::string &msg){
 sepstream sep(msg, '\n');
 Flux::string tok;
 while(sep.GetToken(tok))
   send_cmd("NOTICE %s :%s\n", where.c_str(), tok.c_str());
}
/**
 * \brief Sends a IRC action (/me) to the user or channel
 * \param Destination Where the message will go
 * \param Message The message to send to Destination 
 */
void IRCProto::action(const Flux::string &where, const char *fmt, ...){
  if(fmt)
  {
    va_list args;
    char buffer[BUFSIZE] = "";
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    this->action(where, Flux::string(buffer));
    va_end(args);
  }
}
/**
 * \overload void IRCProto::action(Flux::string where, Flux::string msg)
 * \brief Sends a IRC notice to the user or channel
 */
void IRCProto::action(const Flux::string &where, const Flux::string &msg){
 sepstream sep(msg, '\n');
 Flux::string tok;
 while(sep.GetToken(tok))
   send_cmd("PRIVMSG %s :\001ACTION %s\001\n", where.c_str(), tok.c_str());
}
/**
 * \fn void command::kick(Flux::string Channel, Flux::string User, const char *fmt, ...)
 * \brief Handles kick requests
 * \param channel Channel to be kicked from.
 * \param user User to be kicked.
 * \param reason Reason for the kick.
 */
void IRCProto::kick(const Flux::string &Channel, const Flux::string &User, const char *fmt, ...){
  char buffer[BUFSIZE] = "";
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
void IRCProto::topic(const Flux::string &channel, const char *fmt, ...){
  char buffer[BUFSIZE] = "";
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  this->topic(channel, Flux::string(buffer));
  va_end(args);
}
/**
 * \fn void IRCProto::quit(const char *fmt, ...)
 * \brief Handles quitting of irc
 * \param message Quit message
 */
void IRCProto::quit(const char *fmt, ...){
  char buffer[BUFSIZE] = "";
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
void IRCProto::part(const Flux::string &channel, const char *fmt, ...){
  char buffer[BUFSIZE] = "";
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  this->part(channel, Flux::string(buffer));
  va_end(args);
}
/**
 * \overload void command::kick(Flux::string channel, Flux::string user, Flux::string reason)
 */
void IRCProto::kick(const Flux::string &chan, const Flux::string &userstr, const Flux::string &msg){
  send_cmd("KICK %s %s :%s\n", chan.c_str(), userstr.c_str(), msg.c_str());
}

void IRCProto::pass(const Flux::string &password)
{
  send_cmd("PASS %s\n", password.c_str());
}
/**
 * \overload void IRCProto::quit(Flux::string message)
 */
void IRCProto::quit(const Flux::string &message){
  send_cmd("QUIT :%s\n", message.c_str());
}
/**
 * \overload void IRCProto::part(Flux::string channel, Flux::string msg)
 */
void IRCProto::part(const Flux::string &channel, const Flux::string &msg){
  send_cmd("PART %s :%s\n", channel.c_str(), msg.c_str());
}
/**
 * \overload void command::topic(Flux::string channel, Flux::string msg)
 */
void IRCProto::topic(const Flux::string &chan, const Flux::string &msg){
  send_cmd("TOPIC %s :%s\n", chan.c_str(), msg.c_str());
}
/**
 * \fn void command::nick(Flux::string nick)
 * \brief Sets the bots nickname in IRC.
 * \param nickname A Flux::string with the new nickname.
 */
void IRCProto::nick(const Flux::string &bnick){
  send_cmd("NICK %s\n", bnick.c_str());
}
void IRCProto::away(const Flux::string &message){
  send_cmd("AWAY :%s", message.c_str());
}
/**
 * \fn void command::oper(Flux::string oper, Flux::string password)
 * \brief Sends IRC command /oper
 */
void IRCProto::oper(const Flux::string &username, const Flux::string &password){
  send_cmd("OPER %s %s\n", username.c_str(), password.c_str());
}
/**
 * \fn void command::join(Flux::string chan)
 * \brief Makes the bot join a channel
 * \param stringy_chan A Flux::string with the channel you want to join.
 */
void IRCProto::join(const Flux::string &dchan){
  send_cmd("JOIN %s\n", dchan.c_str());
}
/**
 * \overload void command::part(Flux::string channel)
 * \brief Parts channel w/o reason.
 * \param channel Channel to part from.
 */
void IRCProto::part(const Flux::string &fchan){
  send_cmd("PART %s\n", fchan.c_str());
}
/**
 * \fn void IRCProto::who(Flux::string chan)
 * \brief Sends a /who to the channel
 * \param chan A Flux::string with the channel you want to /who.
 */
void IRCProto::who(const Flux::string &chan){
  send_cmd("WHO %s\n", chan.c_str());
}
/**
 * \fn void IRCProto::names(Flux::string &chan)
 * \brief Sends /names to the channel
 * \param chan A Flux::string with the channel you want to /names.
 */
void IRCProto::names(const Flux::string &chan){
  send_cmd("NAMES %s\n", chan.c_str());
}
/**
 * \fn void command::whois(Flux::string Nick)
 * \brief Sends a IRC Whois to Server.
 * \param Nick Nick to query
 */
void IRCProto::whois(const Flux::string &nickname){
  send_cmd("WHOIS %s\n", nickname.c_str());
}
/**
 * \fn void command::mode(Flux::string nickname, Flux::string mode, Flux::string user)
 * \brief Sends a mode to be set in IRC
 * \param nickname Nickname of who we are setting a more to.
 * \param mode The mode to set.
 */
void IRCProto::mode(const Flux::string &chan, const Flux::string &usermode, const Flux::string &usernick){
  send_cmd("MODE %s %s %s\n", chan.c_str(), usermode.c_str(), usernick.c_str());
}
/**
 * \fn void IRCProto::user(Flux::string ident, Flux::string realname)
 * \brief Sends the user gecos to the server
 * \param ident The ident at the beginning of the IRC host.
 * \param realname The real name gecos used in irc.
 */
void IRCProto::user(const Flux::string &ident, const Flux::string &realname){
  send_cmd("USER %s * * :%s\n", ident.c_str(), realname.c_str());
}
/**
 * \overload void command::mode(Flux:;string dest, Flux::string mode)
 * \brief Sends a mode to the server
 * @param dest where to set the mode
 * @param mode mode to set
 */
void IRCProto::mode(const Flux::string &dest, const Flux::string &chanmode){
  send_cmd("MODE %s %s\n", dest.c_str(), chanmode.c_str());
}