#include "user.h"
Flux::map<User *> UserNickList;
size_t usercnt = 0, maxusercnt = 0;
User::User(const Flux::string &snick, const Flux::string &sident, const Flux::string &shost, const Flux::string &srealname){
 /* check to see if a empty string was passed into the constructor */
 if(snick.empty() || sident.empty() || shost.empty())
   throw CoreException("Bad args sent to User constructor");
 
 this->nick = snick;
 this->ident = sident;
 this->host = shost;
 this->realname = srealname;
 this->fullhost = snick+"!"+sident+"@"+shost;
 UserNickList[snick] = this;
 printf("New user! %s!%s@%s%s\n", this->nick.c_str(), this->ident.c_str(), this->host.c_str(), this->realname.empty()?"":Flux::string(" :"+this->realname).c_str());
 ++usercnt;
 if(usercnt > maxusercnt){
  maxusercnt = usercnt;
  printf("New maximum user count: %i\n", maxusercnt);
 }
}
void User::kick(const Flux::string &channel, const Flux::string &reason){
  Send->command->kick(channel, this->nick, reason);
  //send_cmd("KICK %s %s :%s", this->nick.c_str(), channel.c_str(), reason.c_str());
}
void User::kill(const Flux::string &reason){
  //Send->command->Kill(this->nick, reason);
 send_cmd("KILL %s :%s", this->nick.c_str(), reason.c_str());
}
void User::SendMessage(const Flux::string &message){
  Send->notice(this->nick, message);
  //send_cmd("NOTICE %s :%s\n", this->nick.c_str(), message.c_str());
}
void User::Privmsg(const Flux::string &message){
  Send->privmsg(this->nick, message);
  send_cmd("PRIVMSG %s :%s", this->nick.c_str(), message.c_str());
}
User::~User(){
  printf("Deleting user %s!%s@%s%s\n", this->nick.c_str(), this->ident.c_str(), this->host.c_str(), this->realname.empty()?"":Flux::string(" :"+this->realname).c_str());
  log("Deleting user %s!%s@%s%s\n", this->nick.c_str(), this->ident.c_str(), this->host.c_str(), this->realname.empty()?"":Flux::string(" :"+this->realname).c_str());
  UserNickList.erase(this->nick);
}
User *finduser(const Flux::string &nick){
  Flux::map<User *>::iterator it = UserNickList.find(nick);
  if(it != UserNickList.end())
    return it->second;
  return NULL;
}
void CommandSource::Reply(const char *fmt, ...){
  va_list args;
  char buf[4096];
  if(fmt){
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    this->Reply(Flux::string(buf));
    va_end(args);
  }
}
void CommandSource::Reply(const Flux::string &msg){
 sepstream sep(msg, '\n');
 Flux::string tok;
 while(sep.GetToken(tok))
 {
   this->u->SendMessage(tok);
   //Send->notice(this->u->nick, tok.c_str());
 }
}
/*******************************************************************/
channel_map ChanMap;
Channel::Channel(const Flux::string &nname, time_t ts){
  if(nname.empty())
    throw CoreException("Someone was an idiot and passed an empty channel name into the channel constructor >:d");
  
  this->name = nname;
  ChanMap[this->name] = this;
  this->creation_time = ts;
  this->topic_time = 0;
  printf("Created new channel: %s\n", this->name.c_str());
  log("Created new channel: %s", this->name.c_str());
}
Channel::~Channel()
{
 printf("Deleted channel: %s\n", this->name.c_str());
 log("Deleted channel: %s", this->name.c_str());
 ChanMap.erase(this->name);
}
void Channel::join(){
 Send->command->join(this->name);
}
void Channel::kick(User *u, const Flux::string &reason){
 u->kick(this->name, reason); 
}
void Channel::SetMode(const Flux::string &mode){
 if(mode[0] == '+'){
   Send->command->mode(this->name, mode);
 }else{
   mode == '+' + mode;
   Send->command->mode(this->name, mode);
 }
}
void Channel::SetMode(User *u, const Flux::string &mode){
 if(mode[0] == '+'){
   Send->command->mode(this->name, mode, u->nick);
 }else{
   mode == '+' + mode;
   Send->command->mode(this->name, mode, u->nick);
 }
}
void Channel::RemoveMode(const Flux::string &mode){
  if(mode[0] == '-'){
    Send->command->mode(this->name, mode);
  }else{
    
    mode == '-' + mode;
    Send->command->mode(this->name, mode);
  }
}
void Channel::RemoveMode(User *u, const Flux::string &mode){
  if(mode[0] == '-'){
    Send->command->mode(this->name, mode, u->nick);
  }else{
    mode == '-' + mode;
    Send->command->mode(this->name, mode, u->nick);
  }
}
void Channel::ChangeTopic(const char *fmt, ...){
  char buffer[4096] = "";
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  this->ChangeTopic(Flux::string(buffer));
  va_end(args);
}
void Channel::ChangeTopic(const Flux::string &topic){
 Send->command->topic(this->name, topic); 
}
void Channel::SendMessage(const char *fmt, ...){
  char buffer[4096] = "";
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  this->SendMessage(Flux::string(buffer));
  va_end(args);
}
void Channel::SendMessage(const Flux::string &message){
 Send->privmsg(this->name, message); 
}
void Channel::SendNotice(const char *fmt, ...){
  char buffer[4096] = "";
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  this->SendNotice(Flux::string(buffer));
  va_end(args); 
}
void Channel::SendNotice(const Flux::string &message){
 Send->notice(this->name, message); 
}
Channel *findchannel(const Flux::string &channel){
  Flux::map<Channel *>::iterator it = ChanMap.find(channel);
  if(it != ChanMap.end())
    return it->second;
  return NULL;
}