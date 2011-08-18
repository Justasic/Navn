#include "privmsg.h"
/**
 *\file  privmsg.cpp 
 *\brief Contains the SendMessage class.
 */
SendMessage::SendMessage(SocketIO *sock){
  this->s = sock;
  this->command = new Command(sock);
  this->o = new Oper(sock);
}
/**
 * \brief Sends a IRC private message to the user or channel
 * \param Destination Where the message will go
 * \param Message The message to send to Destination 
 */
void SendMessage::privmsg(Flux::string where, const char *fmt, ...){
  va_list args;
  char buffer[4096] = "";
  if(fmt)
  {
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    this->privmsg(where, Flux::string(buffer));
    va_end(args);
  }
}
/**
 * \overload void SendMessage::privmsg(Flux::string where, Flux::string msg)
 * \brief Sends a IRC private message to the user or channel
 */
void SendMessage::privmsg(Flux::string where, Flux::string msg){
 sepstream sep(msg, '\n');
 Flux::string tok;
 while(sep.GetToken(tok))
 {
   this->raw("PRIVMSG %s :%s\n", where.c_str(), tok.c_str());
 }
}
/**
 * \brief Sends a IRC notice to the user or channel
 * \param Destination Where the message will go
 * \param Message The message to send to Destination 
 */
void SendMessage::notice(Flux::string where, const char *fmt, ...){
  va_list args;
  char buffer[4096] = "";
  if(fmt)
  {
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    this->notice(where, Flux::string(buffer));
    va_end(args);
  }
}
/**
 * \overload void SendMessage::notice(Flux::string where, Flux::string msg)
 * \brief Sends a IRC notice to the user or channel
 */
void SendMessage::notice(Flux::string where, Flux::string msg){
 sepstream sep(msg, '\n');
 Flux::string tok;
 while(sep.GetToken(tok))
 {
   this->raw("NOTICE %s :%s\n", where.c_str(), tok.c_str());
 }
}
/**
 * \brief Sends a IRC action (/me) to the user or channel
 * \param Destination Where the message will go
 * \param Message The message to send to Destination 
 */
void SendMessage::action(Flux::string where, const char *fmt, ...){
  va_list args;
  char buffer[4096] = "";
  if(fmt)
  {
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    this->action(where, Flux::string(buffer));
    va_end(args);
  }
}
/**
 * \overload void SendMessage::action(Flux::string where, Flux::string msg)
 * \brief Sends a IRC notice to the user or channel
 */
void SendMessage::action(Flux::string where, Flux::string msg){
 sepstream sep(msg, '\n');
 Flux::string tok;
 while(sep.GetToken(tok))
 {
   this->raw("PRIVMSG %s :\001ACTION %s\001\n", where.c_str(), tok.c_str());
 }
}
/**
 * \fn void command::raw(const char *fmt, ...)
 * \brief Sends data straight to the socket engine
 */
void SendMessage::raw(const char *fmt, ...){
  char buffer[4096] = "";
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  this->s->send(buffer);
  va_end(args);
}