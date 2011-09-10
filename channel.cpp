#include "channel.h"

Flux::map<Channel*> ChanMap;
Channel::Channel(const Flux::string &nname, time_t ts){
  if(nname.empty())
    throw CoreException("Someone was an idiot and passed an empty channel name into the channel constructor >:d");
  if(!IsValidChannel(nname))
    throw CoreException("An Invalid channel was passed into the Channel constructor >:d");
  
  this->name = nname;
  this->creation_time = ts;
  this->topic_time = 0;
  ChanMap[this->name] = this;
  printf("Created new channel: %s\n", this->name.c_str());
  log("Created new channel: %s", this->name.c_str());
}
Channel::~Channel()
{
 printf("Deleted channel: %s\n", this->name.c_str());
 log("Deleted channel: %s", this->name.c_str());
 ChanMap.erase(this->name);
}
void Channel::SendJoin(){
 Send->command->join(this->name);
}
void Channel::SendPart(){
 Send->command->part(this->name);
}
void Channel::SendPart(const char *fmt, ...){
  char buffer[4096] = "";
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  this->SendPart(Flux::string(buffer));
  va_end(args);
}
void Channel::SendPart(const Flux::string &reason){
  Send->command->part(this->name, reason);
}
void Channel::kick(User *u, const Flux::string &reason){
 u->kick(this->name, reason);
}
void Channel::kick(User *u, const char *fmt, ...){
  char buffer[4096] = "";
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  this->kick(u, Flux::string(buffer));
  va_end(args);
}
void Channel::kick(const Flux::string &u, const char *fmt, ...){
  char buffer[4096] = "";
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  this->kick(u, Flux::string(buffer));
  va_end(args);
}
void Channel::kick(const Flux::string &u, const Flux::string &reason){
 Send->command->kick(this->name, u, reason);
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
void Channel::SendAction(const char *fmt, ...){
  char buffer[4096] = "";
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  this->SendMessage(Flux::string(buffer));
  va_end(args);
}
void Channel::SendAction(const Flux::string &message){
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
void Channel::SendWho(){
 Send->command->who(this->name); 
}
/****************************************************************/
void ListChans(CommandSource &source){
  Flux::string channels;
  for(Flux::map<Channel*>::iterator it = ChanMap.begin(), it_end = ChanMap.end(); it != it_end; ++it){
    Channel *ch = it->second;
    channels += ch->name;
    channels.AddSpace();
  }
  source.Reply("Channels: %s\n", channels.c_str());
}
Channel *findchannel(const Flux::string &channel){
  Flux::map<Channel *>::iterator it = ChanMap.find(channel);
  if(it != ChanMap.end())
    return it->second;
  return NULL;
}