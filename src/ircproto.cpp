/* Navn IRC bot -- IRC Protocol classes
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */
#include "ircproto.h"

// An object for tracking how many lines we have sent and such
struct SendQobject
{
	// The Queue of all messages, buffered;
	std::queue<Flux::string> SendQ;
	// Number of lines sent before burst
	int linessent;
	// Have we sent the burst yet?
	inline bool HasBurst() const
	{
		return (linessent <= Config->BurstRate);
	}
} sqo;

// Buffer all messages except critical ones and send them in a timely manner
class SendQTimer : public Timer
{
	int sent;
public:
	SendQTimer() : Timer(Config->SendQRate, time(NULL), true), sent(0)
	{
		Log(LOG_DEBUG) << "Initialized a SengQ Timer";
		sqo.linessent = 0;
	}

	~SendQTimer()
	{
		Log(LOG_DEBUG) << "Destroying SendQ Timer";

		while(!sqo.SendQ.empty())
			sqo.SendQ.pop();
	}

	void Tick(time_t)
	{
		while(!sqo.SendQ.empty() && ++sent <= Config->SendQLines)
		{
			if(sock)
				sock->send(sqo.SendQ.front());
			else
				Log(LOG_WARN) << "Attempted to send \"" << sqo.SendQ.front() << "\" to the server but no socket exists!";

			sqo.SendQ.pop();
		}

		if(sqo.SendQ.empty())
			sqo.linessent = 0;
		else
			Log(LOG_RAWIO) << "SendQ buffer size: " << sqo.SendQ.size();

		sent = 0;
	}
};

/**
 *\file  ircproto.cpp
 *\brief Contains the IRCProto class.
 */
IRCProto::IRCProto() : o(NULL)
{
	if(Config->SendQEnabled) new SendQTimer();
}

IRCProto::~IRCProto()
{
	if(this->o)
		delete this->o;

	this->o = NULL;
}

void IRCProto::Raw(const char *fmt, ...)
{
	va_list args;
	char buffer[BUFSIZE] = "";
	va_start(args, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, args);

	if(Config->SendQEnabled)
	{
		if(sqo.HasBurst() && sock)
		{
			sqo.linessent++; // Spam for a few lines
			sock->send(buffer);
		}
		else // Oh well.. fun while it lasted lol
			sqo.SendQ.push(Flux::string(buffer));
	}
	else // Send Unlimited if there's no sendq enabled. Whee!
		sock->send(buffer);

	va_end(args);
}

/**
 * \brief Sends a IRC private message to the user or channel
 * \param Destination Where the message will go
 * \param Message The message to send to Destination
 */
void IRCProto::privmsg(const Flux::string &where, const char *fmt, ...)
{
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
void IRCProto::privmsg(const Flux::string &where, const Flux::string &msg)
{
	sepstream sep(msg, '\n');
	Flux::string tok;

	while(sep.GetToken(tok))
		this->Raw("PRIVMSG %s :%s\n", where.c_str(), tok.c_str());
}
/**
 * \brief Sends a IRC notice to the user or channel
 * \param Destination Where the message will go
 * \param Message The message to send to Destination
 */
void IRCProto::notice(const Flux::string &where, const char *fmt, ...)
{
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
void IRCProto::notice(const Flux::string &where, const Flux::string &msg)
{
	sepstream sep(msg, '\n');
	Flux::string tok;

	while(sep.GetToken(tok))
		this->Raw("NOTICE %s :%s\n", where.c_str(), tok.c_str());
}
/**
 * \brief Sends a IRC action (/me) to the user or channel
 * \param Destination Where the message will go
 * \param Message The message to send to Destination
 */
void IRCProto::action(const Flux::string &where, const char *fmt, ...)
{
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
void IRCProto::action(const Flux::string &where, const Flux::string &msg)
{
	sepstream sep(msg, '\n');
	Flux::string tok;

	while(sep.GetToken(tok))
		this->Raw("PRIVMSG %s :\001ACTION %s\001\n", where.c_str(), tok.c_str());
}
/**
 * \fn void command::kick(Flux::string Channel, Flux::string User, const char *fmt, ...)
 * \brief Handles kick requests
 * \param channel Channel to be kicked from.
 * \param user User to be kicked.
 * \param reason Reason for the kick.
 */
void IRCProto::kick(const Flux::string &Channel, const Flux::string &User, const char *fmt, ...)
{
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
void IRCProto::topic(const Flux::string &channel, const char *fmt, ...)
{
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
void IRCProto::quit(const char *fmt, ...)
{
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
void IRCProto::part(const Flux::string &channel, const char *fmt, ...)
{
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
void IRCProto::kick(const Flux::string &chan, const Flux::string &userstr, const Flux::string &msg)
{
	Flux::string message = msg;

	if(message.size() > static_cast<unsigned>(isupport.KickLen))
	{
		message.erase(isupport.KickLen - 1);
		Log(LOG_WARN) << "Max kick length is " << isupport.KickLen << " but kick message is " << msg.size() << ", kick message is too long!";
	}

	this->Raw("KICK %s %s :%s\n", chan.c_str(), userstr.c_str(), message.c_str());
}

void IRCProto::pass(const Flux::string &servpass)
{
	this->Raw("PASS %s\n", servpass.c_str());
}
/**
 * \overload void IRCProto::quit(Flux::string message)
 */
void IRCProto::quit(const Flux::string &message)
{
	send_cmd("QUIT :%s\n", message.c_str());
}
/**
 * \overload void IRCProto::part(Flux::string channel, Flux::string msg)
 */
void IRCProto::part(const Flux::string &channel, const Flux::string &msg)
{
	if(msg.empty())
		this->Raw("PART %s\n", channel.c_str());
	else
		this->Raw("PART %s :%s\n", channel.c_str(), msg.c_str());
}
/**
 * \overload void command::topic(Flux::string channel, Flux::string msg)
 */
void IRCProto::topic(const Flux::string &chan, const Flux::string &msg)
{
	Flux::string message = msg;

	if(message.size() > static_cast<unsigned>(isupport.TopicLen))
	{
		message.erase(isupport.TopicLen - 1);
		Log(LOG_WARN) << "Max topic length is " << isupport.TopicLen << " but topic is " << msg.size() << ", topic is too long!";
	}

	this->Raw("TOPIC %s :%s\n", chan.c_str(), message.c_str());
}
/**
 * \fn void command::nick(Flux::string nick)
 * \brief Sets the bots nickname in IRC.
 * \param nickname A Flux::string with the new nickname.
 */
void IRCProto::nick(const Flux::string &bnick)
{
	Flux::string message = bnick;

	if(message.size() > static_cast<unsigned>(isupport.NickLen))
	{
		message.erase(isupport.NickLen - 1);
		Log(LOG_WARN) << "Max nick length is " << isupport.NickLen << " but nick is " << bnick.size() << ", nick is too long!";
	}

	this->Raw("NICK %s\n", message.c_str());
}

void IRCProto::away(const Flux::string &msg)
{
	Flux::string message = msg;

	if(message.size() > static_cast<unsigned>(isupport.AwayLen))
	{
		message.erase(isupport.AwayLen - 1);
		Log(LOG_WARN) << "Max Away length is " << isupport.AwayLen << " but away message is " << msg.size() << ", away message is too long!";
	}

	this->Raw("AWAY :%s", message.c_str());
}
/**
 * \fn void command::oper(Flux::string oper, Flux::string password)
 * \brief Sends IRC command /oper
 */
void IRCProto::oper(const Flux::string &username, const Flux::string &operpass)
{
	this->Raw("OPER %s %s\n", username.c_str(), operpass.c_str());
}
/**
 * \fn void command::join(Flux::string chan)
 * \brief Makes the bot join a channel
 * \param stringy_chan A Flux::string with the channel you want to join.
 */
void IRCProto::join(const Flux::string &dchan)
{
	this->Raw("JOIN %s\n", dchan.c_str());
}
/**
 * \fn void IRCProto::who(Flux::string chan)
 * \brief Sends a /who to the channel
 * \param chan A Flux::string with the channel you want to /who.
 */
void IRCProto::who(const Flux::string &chan)
{
	this->Raw("WHO %s\n", chan.c_str());
}
/**
 * \fn void IRCProto::names(Flux::string &chan)
 * \brief Sends /names to the channel
 * \param chan A Flux::string with the channel you want to /names.
 */
void IRCProto::names(const Flux::string &chan)
{
	this->Raw("NAMES %s\n", chan.c_str());
}
/**
 * \fn void command::whois(Flux::string Nick)
 * \brief Sends a IRC Whois to Server.
 * \param Nick Nick to query
 */
void IRCProto::whois(const Flux::string &nickname)
{
	this->Raw("WHOIS %s\n", nickname.c_str());
}
/**
 * \fn void command::mode(Flux::string nickname, Flux::string mode, Flux::string user)
 * \brief Sends a mode to be set in IRC
 * \param nickname Nickname of who we are setting a more to.
 * \param mode The mode to set.
 */
void IRCProto::mode(const Flux::string &chan, const Flux::string &usermode, const Flux::string &usernick)
{
	this->Raw("MODE %s %s %s\n", chan.c_str(), usermode.c_str(), usernick.c_str());
}
/**
 * \fn void IRCProto::introduce_client(const Flux::string &nick, const Flux::string &ident, const Flux::string &realname)
 * \brief Sends the user gecos to the server on connect
 * \param nick The nickname to set on connect
 * \param ident The ident at the beginning of the IRC host.
 * \param realname The real name gecos used in irc.
 */
void IRCProto::introduce_client(const Flux::string &nickname, const Flux::string &ident, const Flux::string &realname)
{
	// Get our hostname of our system
	char hostname[256];
	gethostname(hostname, 256);

	send_cmd("NICK %s\n", nickname.c_str());
	send_cmd("USER %s %s %s :%s\n", ident.c_str(), hostname, Config->Server.c_str(), realname.c_str());

	if(!Config->ServerPassword.empty())
		send_cmd("PASS %s\n", Config->ServerPassword.c_str());
}

/**
 * \overload void command::mode(Flux:;string dest, Flux::string mode)
 * \brief Sends a mode to the server
 * @param dest where to set the mode
 * @param mode mode to set
 */
void IRCProto::mode(const Flux::string &dest, const Flux::string &chanmode)
{
	this->Raw("MODE %s %s\n", dest.c_str(), chanmode.c_str());
}

void IRCProto::invite(const Flux::string &user, const Flux::string &channel)
{
	this->Raw("INVITE %s %s\n", user.c_str(), channel.c_str());
}

void IRCProto::version(const Flux::string &server)
{
	if(server.empty())
		this->Raw("VERSION\n");
	else
		this->Raw("VERSION %s\n", server.c_str());
}

void IRCProto::stats(const Flux::string &query, const Flux::string &server)
{
	if(server.empty())
		this->Raw("STATS %s\n", query.c_str());
	else
		this->Raw("STATS %s %s\n", query.c_str(), server.c_str());
}

void IRCProto::links(const Flux::string &remote, const Flux::string &mask)
{
	if(remote.empty() && mask.empty())
		this->Raw("LINKS\n");
	else if(!remote.empty() && mask.empty())
		this->Raw("LINKS %s\n", remote.c_str());
	else if(!remote.empty() && !mask.empty())
		this->Raw("LINKS %s %s\n", remote.c_str(), mask.c_str());
}

void IRCProto::time(const Flux::string &server)
{
	if(server.empty())
		this->Raw("TIME\n");
	else
		this->Raw("TIME %s\n", server.c_str());
}

void IRCProto::admin(const Flux::string &server)
{
	if(server.empty())
		this->Raw("ADMIN\n");
	else
		this->Raw("ADMIN %s\n", server.c_str());
}

void IRCProto::info(const Flux::string &server)
{
	if(server.empty())
		this->Raw("INFO\n");
	else
		this->Raw("INFO %s\n", server.c_str());
}

void IRCProto::whowas(const Flux::string &nickname, int count, const Flux::string &server)
{
	if(count != 0 && !server.empty())
		this->Raw("WHOWAS %s %d %s\n", nickname.c_str(), count, server.c_str());
	else if(count != 0 && server.empty())
		this->Raw("WHOWAS %s %d\n", nickname.c_str());
	else if(count == 0 && !server.empty())
		this->Raw("WHOWAS %s %s\n", nickname.c_str(), server.c_str());
	else
		this->Raw("WHOWAS %s\n", nickname.c_str());
}

void IRCProto::users(const Flux::string &server)
{
	if(server.empty())
		this->Raw("USERS\n");
	else
		this->Raw("USERS %s\n", server.c_str());
}

void IRCProto::userhost(const Flux::string &nickname)
{
	this->Raw("USERHOST %s\n", nickname.c_str());
}
