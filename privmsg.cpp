/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#include <privmsg.h>
/**
 *\file  privmsg.cpp 
 *\brief Contains the SendMessage class.
 */
SendMessage::SendMessage(){
  this->command = new Commands();
}
/**
 * \brief Sends a IRC private message to the user or channel
 * \param Destination Where the message will go
 * \param Message The message to send to Destination 
 */
void SendMessage::privmsg(const Flux::string &where, const char *fmt, ...){
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
 * \overload void SendMessage::privmsg(Flux::string where, Flux::string msg)
 * \brief Sends a IRC private message to the user or channel
 */
void SendMessage::privmsg(const Flux::string &where, const Flux::string &msg){
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
void SendMessage::notice(const Flux::string &where, const char *fmt, ...){
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
 * \overload void SendMessage::notice(Flux::string where, Flux::string msg)
 * \brief Sends a IRC notice to the user or channel
 */
void SendMessage::notice(const Flux::string &where, const Flux::string &msg){
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
void SendMessage::action(const Flux::string &where, const char *fmt, ...){
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
 * \overload void SendMessage::action(Flux::string where, Flux::string msg)
 * \brief Sends a IRC notice to the user or channel
 */
void SendMessage::action(const Flux::string &where, const Flux::string &msg){
 sepstream sep(msg, '\n');
 Flux::string tok;
 while(sep.GetToken(tok))
   send_cmd("PRIVMSG %s :\001ACTION %s\001\n", where.c_str(), tok.c_str());
}