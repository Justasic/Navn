/* Navn IRC bot -- Join-part management for the bot
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */
#include "modules.h"

/*This Module setup needs serious work!
 * Justasic will work on it but its going to be hard with los's Module class.
 */
class CommandJoin : public Command
{
public:
	CommandJoin(Module *m) : Command(m, "JOIN", C_PRIVATE, 1, 1)
	{
		this->SetDesc("Joins a channel");
		this->SetSyntax("\37channel\37");
	}

	void Run(CommandSource &source, const Flux::vector &params) override
	{
		User *u = source.u;
		Flux::string chan = params[1];

		if(!u->IsOwner())
		{
			source.Reply(ACCESS_DENIED);
			Log(u) << "attempted to make the bot join " << chan;
			return;
		}

		if(!IsValidChannel(chan))
			source.Reply(CHANNEL_X_INVALID, chan.c_str());
		else
		{
			Log(u) << "made the bot join " << chan;
			Channel *c = findchannel(chan);

			if(!c)
			{
				ircproto->join(chan);

				Flux::string WelcomeMessage = Config->WelcomeMessage.replace_all_ci("{botnick}", Config->BotNick);
				WelcomeMessage.trim();

				if(!WelcomeMessage.empty())
					ircproto->privmsg(chan, WelcomeMessage);
			}
			else
			{
				Log(u) << "tried to make bot join " << c->name << " but we're already in that channel";
				source.Reply("Already in \2%s\2", c->name.c_str());
			}
		}
	}

	bool OnHelp(CommandSource &source, const Flux::string &nill) override
	{
		this->SendSyntax(source);
		source.Reply(" ");
		source.Reply("This command makes the bot join a channel.\n"
		             "You must be the bots owner to use this command.");
		return true;
	}
};

class CommandPart : public Command
{
public:
	CommandPart(Module *m): Command(m, "PART", C_PRIVATE, 1, 1)
	{
		this->SetDesc("Part a channel");
		this->SetSyntax("\37channel\37");
	}
	void Run(CommandSource &source, const Flux::vector &params) override
	{
		Flux::string chan = params[1];
		User *u = source.u;

		if(!u->IsOwner())
		{
			source.Reply(ACCESS_DENIED);
			Log(u) << "attempted to make bot part " << chan;
			return;
		}

		if(!IsValidChannel(chan))
			source.Reply(CHANNEL_X_INVALID, chan.c_str());
		else
		{
			Channel *c = findchannel(chan);

			if(c)
				c->SendPart();
			else
				source.Reply("I am not in channel \2%s\2", chan.c_str());

			Log(u) << "made the bot part " << chan;
		}
	}
	bool OnHelp(CommandSource &source, const Flux::string &nill) override
	{
		this->SendSyntax(source);
		source.Reply(" ");
		source.Reply("This command makes the bot part a channel.\n"
		             "You must be the bots owner to use this command.");
		return true;
	}
};

class Join : public Module
{
	CommandJoin cmdjoin;
	CommandPart cmdpart;
public:
	Join(const Flux::string &Name): Module(Name), cmdjoin(this), cmdpart(this)
	{
		this->SetVersion(VERSION);
		this->SetAuthor("Justasic");
	}
};

MODULE_HOOK(Join)
