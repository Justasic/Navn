/* Navn IRC bot -- User class and functions
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */
#include "user.h"
Flux::insensitive_map<User *> UserNickList;
uint32_t usercnt = 0, maxusercnt = 0;

User::User(const Flux::string &snick, const Flux::string &sident, const Flux::string &shost, const Flux::string &srealname, const Flux::string &sserver) : nick(snick), host(shost), realname(srealname), ident(sident), server(sserver)
{
	/* check to see if a empty string was passed into the constructor */
	if(snick.empty() || sident.empty() || shost.empty())
		throw CoreException("Bad args sent to User constructor");

	this->fullhost = snick + "!" + sident + "@" + shost;
	UserNickList[snick] = this;

	Log(LOG_RAWIO) << "New user! " << this->nick << '!' << this->ident << '@' << this->host << (this->realname.empty() ? "" : " :" + this->realname);

	++usercnt;

	if(usercnt > maxusercnt)
	{
		maxusercnt = usercnt;
		Log(LOG_TERMINAL) << "New maximum user count: " << maxusercnt;
	}
}

User::~User()
{
	for(auto & elem : ChannelList)
		if(elem.first)
			elem.first->DelUser(this);

	ChannelList.clear();

	Log() << "Deleting user " << this->nick << '!' << this->ident << '@' << this->host << (this->realname.empty() ? "" : " :" + this->realname);
	UserNickList.erase(this->nick);
}

void User::kick(const Flux::string &channel, const Flux::string &reason)
{
	ircproto->kick(channel, this->nick, reason);
}

void User::kick(Channel *ch, const Flux::string &reason)
{
	ircproto->kick(ch->name, this->nick, reason);
}

void User::SendWho()
{
	ircproto->who(this->nick);
}

void User::kill(const Flux::string &reason)
{
	if(ircproto->o)
		ircproto->o->kill(this->nick, reason);

//send_cmd("KILL %s :%s", this->nick.c_str(), reason.c_str());
}

void User::SendMessage(const char *fmt, ...)
{
	char buffer[BUFSIZE] = "";
	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, args);
	this->SendMessage(Flux::string(buffer));
	va_end(args);
}

void User::SendPrivmsg(const char *fmt, ...)
{
	char buffer[BUFSIZE] = "";
	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, args);
	this->SendPrivmsg(Flux::string(buffer));
	va_end(args);
}

bool User::IsOwner()
{
	for(auto & elem : Config->Owners)
	{
		if(this->nick.equals_ci(elem))
			return true;
	}

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
	if(c)
		ChannelList[c] = this;
}

void User::DelChan(Channel *c)
{
	auto it = ChannelList.find(c);

	if(it != ChannelList.end())
		ChannelList.erase(it);
}

Channel *User::findchannel(const Flux::string &name)
{
	auto it1 = ChanMap.find(name);
	Channel *c = it1->second;

	if(!c)
		return nullptr;

	auto it = ChannelList.find(c);

	if(it != ChannelList.end())
		return it->first;

	return nullptr;
}

void User::SendMessage(const Flux::string &message)
{
	ircproto->notice(this->nick, message);
}

void User::SendPrivmsg(const Flux::string &message)
{
	ircproto->privmsg(this->nick, message);
}

User *finduser(const Flux::string &fnick)
{
	auto it = UserNickList.find(fnick);

	if(it != UserNickList.end())
		return it->second;

	return nullptr;
}
