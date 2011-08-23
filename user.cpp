#include "user.h"
Flux::map<User *> UserNickList;
User::User(const Flux::string &snick, const Flux::string &sident, const Flux::string &shost, const Flux::string &srealname){
 /* check to see if a empty string was passed into the constructor */
 if(snick.empty() || sident.empty() || shost.empty())
   throw CoreException("Bad args sent to User constructor");
 
 this->nick = snick;
 this->ident = sident;
 this->host = shost;
 this->realname = srealname;
 UserNickList[snick] = this;
 printf("New user! %s!%s@%s%s\n", this->nick.c_str(), this->ident.c_str(), this->host.c_str(), this->realname.empty()?"":Flux::string(" :"+this->realname).c_str());
}
void User::Kick(const Flux::string &channel, const Flux::string &reason){
  send_cmd("KICK %s %s :%s", this->nick.c_str(), channel.c_str(), reason.c_str());
}
void User::Kill(const Flux::string &reason){
 send_cmd("KILL %s :%s", this->nick.c_str(), reason.c_str());
}
void User::SendMessage(const Flux::string &message){
  printf("Derp!\n");
  send_cmd("NOTICE %s :%s\n", this->nick.c_str(), message.c_str());
}
void User::Privmsg(const Flux::string &message){
  send_cmd("PRIVMSG %s :%s", this->nick.c_str(), message.c_str());
}
User::~User(){
  log("%s left", this->nick.c_str());
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
   Send->notice(this->u, tok.c_str());
 }
}
