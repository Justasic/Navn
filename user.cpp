#include "user.h"
Flux::map<User *> UserList;
User::User(const Flux::string &snick, const Flux::string &sident, const Flux::string &shost, const Flux::string &srealname){
 /* check to see if a empty string was passed into the constructor */
 if(snick.empty() || sident.empty() || shost.empty())
   throw CoreException("Bad args sent to User constructor");
 
 this->nick = snick;
 this->ident = sident;
 this->host = shost;
 this->realname = srealname;
 UserList[snick] = this;
 printf("passed into user constructor\n");
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
 UserList.erase(this->nick);
}
User *finduser(const Flux::string &nick){
  Flux::map<User *>::iterator it = UserList.find(nick);
  if(it != UserList.end())
    return it->second;
  return NULL;
}