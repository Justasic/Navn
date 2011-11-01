/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#include <user.h>
Flux::insensitive_map<User *> UserNickList;
size_t usercnt = 0, maxusercnt = 0, enter = 0;
User::User(const Flux::string &snick, const Flux::string &sident, const Flux::string &shost, const Flux::string &srealname, const Flux::string &sserver){
 SET_SEGV_LOCATION();
 /* check to see if a empty string was passed into the constructor */
 if(snick.empty() || sident.empty() || shost.empty())
   throw CoreException("Bad args sent to User constructor");
 
 this->nick = snick;
 this->ident = sident;
 this->host = shost;
 this->realname = srealname;
 this->server = sserver;
 this->fullhost = snick+"!"+sident+"@"+shost;
 UserNickList[snick] = this;
 if(protocoldebug)
   Log(LOG_TERMINAL) << "New user! " << this->nick << '!' << this->ident << '@' << this->host << (this->realname.empty()?"":" :"+this->realname);
 ++usercnt;
 if(usercnt > maxusercnt){
  maxusercnt = usercnt;
  Log(LOG_TERMINAL) << "New maximum user count: " << maxusercnt;
 }
}
User::~User(){
  SET_SEGV_LOCATION();
  Log() << "Deleting user " << this->nick << '!' << this->ident << '@' << this->host << (this->realname.empty()?"":" :"+this->realname);
  UserNickList.erase(this->nick);
}
void User::kick(const Flux::string &channel, const Flux::string &reason){
  Send->command->kick(channel, this->nick, reason);
}
void User::kick(Channel *ch, const Flux::string &reason){
  Send->command->kick(ch->name, this->nick, reason);
}
void User::SendWho(){
 Send->command->who(this->nick); 
}
void User::kill(const Flux::string &reason){
  if(Send->o)
   Send->o->kill(this->nick, reason);
 //send_cmd("KILL %s :%s", this->nick.c_str(), reason.c_str());
}
void User::SendMessage(const char *fmt, ...){
  char buffer[4096] = "";
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  this->SendMessage(Flux::string(buffer));
  va_end(args);
}
void User::SendPrivmsg(const char *fmt, ...){
 char buffer[4096] = "";
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  this->SendPrivmsg(Flux::string(buffer));
  va_end(args); 
}
bool User::IsOwner(){
 if(this->nick.equals_ci(Config->Owner))
   return true;
 return false;
}
void User::SetNewNick(const Flux::string &newnick)
{
  if(newnick.empty())
    throw CoreException("User::SetNewNick() was called with empty arguement");
  
  UserNickList.erase(this->nick);
  this->nick = newnick;
  UserNickList[this->nick] = this;
}
void User::AddChan(Channel *c)
{
  if(!c)
    ChannelList[c] = this;
}
void User::DelChan(Channel *c)
{
  CList::iterator it = ChannelList.find(c);
  if(it != ChannelList.end())
    ChannelList.erase(it);
}
Channel *User::findchannel(Channel *c)
{
  printf("entered User::findchannel %i\n", (int)++enter);
  if(enter > 5000)
    raise(SIGSEGV);
  if(!c)
    return NULL;
  CList::iterator it = ChannelList.find(c);
  if(it != ChannelList.end())
    return it->first;
  return NULL;
}
void User::SendMessage(const Flux::string &message){
  Send->notice(this->nick, message);
}
void User::SendPrivmsg(const Flux::string &message){
  Send->privmsg(this->nick, message);
}
User *finduser(const Flux::string &fnick){
  Flux::map<User *>::iterator it = UserNickList.find(fnick);
  if(it != UserNickList.end())
    return it->second;
  return NULL;
}
void ListUsers(CommandSource &source){
  Flux::string users;
  for(Flux::map<User *>::iterator it = UserNickList.begin(), it_end = UserNickList.end(); it != it_end; ++it){
    User *u2 = it->second;
    users += u2->nick+' ';
  }
  source.Reply("Users: %s\n", users.c_str());
}