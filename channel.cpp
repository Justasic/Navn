/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#include <channel.h>

Flux::insensitive_map<Channel*> ChanMap;
Channel::Channel(const Flux::string &nname, time_t ts){
  SET_SEGV_LOCATION();
  if(nname.empty())
    throw CoreException("Someone was an idiot and passed an empty channel name into the channel constructor >:d");
  if(!IsValidChannel(nname))
    throw CoreException("An Invalid channel was passed into the Channel constructor :<");
  
  this->name = nname;
  this->creation_time = ts;
  this->topic_time = 0;
  ChanMap[this->name] = this;
  Log(LOG_DEBUG) << "Created new channel: " << nname;
}
Channel::~Channel()
{
  SET_SEGV_LOCATION();
  Log(LOG_DEBUG) << "Deleted channel " << this->name;
  ChanMap.erase(this->name);
}
void Channel::SendJoin(){
 Send->command->join(this->name);
}
void Channel::SendPart(){
 Send->command->part(this->name);
}
void Channel::SendPart(const char *fmt, ...){
  if(fmt){
    char buffer[4096] = "";
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    this->SendPart(Flux::string(buffer));
    va_end(args);
  }
}
void Channel::SendPart(const Flux::string &reason){
  Send->command->part(this->name, reason);
}
void Channel::kick(User *u, const Flux::string &reason){
 u->kick(this->name, reason);
}
void Channel::kick(User *u, const char *fmt, ...){
  if(fmt){
    char buffer[4096] = "";
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    this->kick(u, Flux::string(buffer));
    va_end(args);
  }
}
void Channel::kick(const Flux::string &u, const char *fmt, ...){
  if(fmt){
    char buffer[4096] = "";
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    this->kick(u, Flux::string(buffer));
    va_end(args);
  }
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
  if(fmt){
    char buffer[4096] = "";
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    this->ChangeTopic(Flux::string(buffer));
    va_end(args);
  }
}
void Channel::ChangeTopic(const Flux::string &topicstr){
 Send->command->topic(this->name, topicstr); 
}
void Channel::SendMessage(const char *fmt, ...){
  if(fmt){
    char buffer[4096] = "";
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    this->SendMessage(Flux::string(buffer));
    va_end(args);
  }
}
void Channel::SendMessage(const Flux::string &message){
 Send->privmsg(this->name, message); 
}
void Channel::SendAction(const char *fmt, ...){
  if(fmt){
    char buffer[4096] = "";
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    this->SendMessage(Flux::string(buffer));
    va_end(args);
  }
}
void Channel::SendAction(const Flux::string &message){
 Send->privmsg(this->name, message); 
}
void Channel::SendNotice(const char *fmt, ...){
  if(fmt){
    char buffer[4096] = "";
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    this->SendNotice(Flux::string(buffer));
    va_end(args);
  }
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
    channels += ch->name+' ';
  }
  source.Reply("Channels: %s\n", channels.c_str());
}
Channel *findchannel(const Flux::string &channel){
  Flux::map<Channel *>::iterator it = ChanMap.find(channel);
  if(it != ChanMap.end())
    return it->second;
  return NULL;
}