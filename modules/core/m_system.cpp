/* Navn IRC bot -- Generic Bot management functions
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */

#include "modules.h"

struct sysinfo sys_info;

class CommandRehash : public Command
{
public:
	CommandRehash(Module *m): Command(m, "REHASH", C_PRIVATE, 0, 0)
	{
		this->SetDesc("Rehashes the config file");
	}
	void Run(CommandSource &source, const Flux::vector &params)
	{
		User *u = source.u;

		if(!u->IsOwner())
			source.Reply(ACCESS_DENIED);
		else
		{
			source.Reply("Rehashing Configuration file");
			Log(u, this) << "to rehash the configuration";
			Rehash();
		}
	}

	bool OnHelp(CommandSource &source, const Flux::string &nill)
	{
		this->SendSyntax(source);
		source.Reply(" ");
		source.Reply("This command rehashes the bots configuration\n"
		             "which allows for various configuration changes\n"
		             "to be made while the bot is running\n"
		             "Note: you must be the bots owner and does not\n"
		             "change all values in the config (such as IRC nicknames)");
		return true;
	}
};

class CommandNick : public Command
{
public:
	CommandNick(Module *m): Command(m, "NICK", C_PRIVATE, 1, 1)
	{
		this->SetDesc("Change the bots nickname");
		this->SetSyntax("\37nickname\37");
	}
	void Run(CommandSource &source, const Flux::vector &params)
	{
		User *u = source.u;
		Flux::string newnick = params[1];

		if(newnick.empty())
		{
			this->SendSyntax(source);
			return;
		}

		if(!u->IsOwner())
		{
			source.Reply(ACCESS_DENIED);
			return;
		}

		for(unsigned i = 0, end = newnick.length(); i < end; i++)
			if(!isvalidnick(newnick[i]))
			{
				source.Reply("\2%s\2 is an invalid nickname.");
				Config->BotNick = newnick;
			}

		ircproto->nick(newnick);
		Log(u, this) << "to change the bots nickname to " << newnick;
	}
	bool OnHelp(CommandSource &source, const Flux::string &nill)
	{
		this->SendSyntax(source);
		source.Reply(" ");
		source.Reply("This command changes the bots nickname\n"
		             "in IRC to a valid IRC nickname that is\n"
		             "checked before being set.\n"
		             "Note: You must be the bots owner to use this command");
		return true;
	}
};

class CommandRestart : public Command
{
public:
	CommandRestart(Module *m): Command(m, "RESTART", C_PRIVATE, 0, 1)
	{
		this->SetDesc("Restarts the bot");
		this->SetSyntax("\37reason\37");
	}
	void Run(CommandSource &source, const Flux::vector &params)
	{
		if(source.u->IsOwner())
		{
			ircproto->quit("Restarting..");
			restart("Restarting..");
			Log(source.u, this) << " to restart the bot.";
		}
		else
			source.Reply(ACCESS_DENIED);
	}
	bool OnHelp(CommandSource &source, const Flux::string &nill)
	{
		this->SendSyntax(source);
		source.Reply(" ");
		source.Reply("This command restarts the bot. This will\n"
		             "make the bot quit IRC, terminate its current\n"
		             "process, and start a new one with the same\n"
		             "settings as before\n"
		             "Note: You must be the bots owner to use this command");
		return true;
	}
};

class CommandKick : public Command
{
public:
	CommandKick(Module *m): Command(m, "KICK", C_PRIVATE, 2, 3)
	{
		this->SetDesc("Kick a user from the channel");
		this->SetSyntax("channel \37nick\15");
	}
	void Run(CommandSource &source, const Flux::vector &params)
	{
		User *u = source.u;

		if(u->IsOwner())
		{
			Flux::string kickchan = params[0];
			Flux::string kickee = params[1];
			Flux::string msg = params[2];

			if(kickee.empty() || kickchan.empty())
			{
				this->SendSyntax(source);
				return;
			}

			if(!IsValidChannel(kickchan))
			{
				source.Reply(CHANNEL_X_INVALID, kickchan.c_str());
				return;
			}

			Channel *c = findchannel(kickchan);

			if(!c)
			{
				source.Reply("I am not in channel \2%s\2", kickchan.c_str());
				return;
			}

			c->kick(kickee, "%s(%s)", u->nick.c_str(), msg.empty() ? msg.c_str() : "No message");
		}
		else
			source.Reply(ACCESS_DENIED);
	}
	bool OnHelp(CommandSource &source, const Flux::string &nill)
	{
		this->SendSyntax(source);
		source.Reply(" ");
		source.Reply("This command kicks a user from the channel\n"
		             "if the bot is an op higher than the user\n"
		             "to be kicked\n"
		             "Note: You must be a bot owner to use this command");
		return true;
	}
};

class CommandChown : public Command
{
public:
	CommandChown(Module *m): Command(m, "CHOWN", C_PRIVATE, 1, 1)
	{
		this->SetDesc("Change ownership over the bot");
		this->SetSyntax("\37owner\37");
	}
	void Run(CommandSource &source, const Flux::vector &params)
	{
		source.Reply("This command is broken!");
	}
	bool OnHelp(CommandSource &source, const Flux::string &nill)
	{
		this->SendSyntax(source);
		source.Reply(" ");
		source.Reply("This command changes the bots ownership to\n"
		             "the one specified\n"
		             "Note: You must know the user set password in\n"
		             "the bots configuration");
		return true;
	}
};

class CommandQuit : public Command
{
public:
	CommandQuit(Module *m): Command(m, "QUIT", C_PRIVATE, 1, 1)
	{
		this->SetDesc("Quits the bot from IRC");
		this->SetSyntax("\37randompass\37");
	}
	void Run(CommandSource &source, const Flux::vector &params)
	{
		User *u = source.u;
		Flux::string pass = params[1];

		if(pass == password || pass == Config->UserPass)
		{
			source.Reply("Quitting..");
			Log(u) << "quit the bot with password: \"" << password << "\"";
			ircproto->quit("Requested From \2%s\17. Pass: \00320%s\017", u->nick.c_str(), password.c_str());
			quitting = true;
		}
		else
		{
			source.Reply(ACCESS_DENIED);
			Log(u) << "Attempted to quit the bot";
		}
	}
	bool OnHelp(CommandSource &source, const Flux::string &nill)
	{
		this->SendSyntax(source);
		source.Reply(" ");
		source.Reply("This command instructs the bot to quit IRC\n"
		             "with the quit message being the user who quit\n"
		             "the bot and the randomly generated password\n"
		             "Note: You must be the bots owner or know the\n"
		             "configuration set password to use this command");
		return true;
	}
};

class CommandTopic: public Command
{
public:
	CommandTopic(Module *m): Command(m, "TOPIC", C_PRIVATE, 2, 2)
	{
		this->SetDesc("Set the topic on a channel");
		this->SetSyntax("\37channel\37 [\37topic\37]");
	}

	void Run(CommandSource &source, const Flux::vector &params)
	{
		Flux::string tchan = params[0];
		Flux::string msg = params[1];
		User *u = source.u;
		Channel *ch = findchannel(tchan);

		if(!u->IsOwner())
		{
			source.Reply(ACCESS_DENIED);
			return;
		}

		if(!IsValidChannel(tchan))
		{
			source.Reply(CHANNEL_X_INVALID, tchan.c_str());
			return;
		}

		if(!ch)
		{
			source.Reply("I am not in channel \2%s\2", tchan.c_str());
			return;
		}

		ch->ChangeTopic(msg);
		std::fstream topic;
		topic.open("topic.tmp", std::fstream::in | std::fstream::out | std::fstream::app);

		if(!topic.is_open())
		{
			source.Reply("Unable to write topic temp file");
			Log(u, this) << " in an attempt to change the topic in " << tchan << " and failed to write to temp file 'topic.tmp'";
		}
		else
		{
			topic << "<?xml version=\"1.0\" ?><rss version=\"2.0\"><channel><topic> " << tchan << " Topic: " << msg.strip() << " </topic></channel></rss>" << std::endl;
			system("sh ftp.sh");
		}

		Log(u, this) << "to change " << tchan << "'s topic to \"" << msg << "\"";
	}

	bool OnHelp(CommandSource &source, const Flux::string &nill)
	{
		this->SendSyntax(source);
		source.Reply(" ");
		source.Reply("This command changes the channels topic\n"
		             "and sends it to a web server for the changed\n"
		             "topic in IRC. This uses the ftp.sh file\n"
		             "Note: You must be a bot owner to use this command");
		return true;
	}
};

class CommandPID: public Command
{
public:
	CommandPID(Module *m): Command(m, "PID", C_PRIVATE)
	{
		this->SetDesc("Gets the bots Process ID");
	}
	void Run(CommandSource &source, const Flux::vector &params)
	{
		User *u = source.u;

		if(u->IsOwner())
		{
			source.Reply("My PID is: \2%i\2", static_cast<int>(getpid()));
			Log(u, this) << "command to get navn's PID " << getpid();
		}
		else
		{
			source.Reply(ACCESS_DENIED);
		}
	}
	bool OnHelp(CommandSource &source, const Flux::string &nill)
	{
		this->SendSyntax(source);
		source.Reply(" ");
		source.Reply("This command returns the bots current\n"
		             "Process ID in linux or windows.\n"
		             "Note: You must be a bot owner to use this command");
		return true;
	}
};

class CommandPass: public Command
{
public:
	CommandPass(Module *m): Command(m, "PASS", C_PRIVATE)
	{
		this->SetDesc("Gets the bots Random Password");
	}
	void Run(CommandSource &source, const Flux::vector &params)
	{
		User *u = source.u;

		if(u->IsOwner())
		{
			source.Reply("The password is:\2 %s", password.c_str());
			Log(u, this) << "to request navn's quit password: " << password;
		}
		else
		{
			source.Reply(ACCESS_DENIED);
			Log(u, this) << "to attempt to request navn's quit password.";
		}
	}
	bool OnHelp(CommandSource &source, const Flux::string &nill)
	{
		this->SendSyntax(source);
		source.Reply(" ");
		source.Reply("This command displays the bots randomly\n"
		             "generated password to the user so they may\n"
		             "quit the bot or use a command which requires\n"
		             "the random password\n"
		             "Note: You must be the bots owner to use this command");
		return true;
	}
};

class CommandStats: public Command
{
public:
	CommandStats(Module *m): Command(m, "STATS", C_PRIVATE)
	{
		this->SetDesc("Shows system stats");
	}
	void Run(CommandSource &source, const Flux::vector &params)
	{
		int days, hours, mins;

//#ifndef _WIN32
		if(sysinfo(&sys_info) != 0)
		{
			//now here Justasic got pissed because c strings suck ass
			Flux::string fuckingshit = Flux::string("sys_info Error: ") + Flux::string(strerror(errno));
			throw ModuleException(fuckingshit.c_str());
		}

		struct utsname uts;

		if(uname(&uts) < 0)
		{
			Flux::string fuckingshit = Flux::string("uname() Error: ") + Flux::string(strerror(errno));
			throw ModuleException(fuckingshit.c_str());
		}

		// Uptime
		days = sys_info.uptime / 86400;
		hours = (sys_info.uptime / 3600) - (days * 24);
		mins = (sys_info.uptime / 60) - (days * 1440) - (hours * 60);

		source.Reply("System Uptime: %d days, %d hours, %d minutes, %ld seconds",
		             days, hours, mins, sys_info.uptime % 60);
		source.Reply("Bot Uptime: %s", duration(time(NULL) - starttime).c_str());

		// Load Averages for 1,5 and 15 minutes
		source.Reply("Load Avgs: 1min(%ld) 5min(%ld) 15min(%ld)",
		             sys_info.loads[0], sys_info.loads[1], sys_info.loads[2]);

		// Total and free ram.
		source.Reply("Total Ram: %ldk\tFree: %ldk", sys_info.totalram / 1024,
		             sys_info.freeram / 1024);

		// Shared and buffered ram.
		source.Reply("Shared Ram: %ldk", sys_info.sharedram / 1024);
		source.Reply("Buffered Ram: %ldk", sys_info.bufferram / 1024);

		// Swap space
		source.Reply("Total Swap: %ldk\tFree: %ldk", sys_info.totalswap / 1024, sys_info.freeswap / 1024);

		// Number of processes currently running.
		source.Reply("Number of processes: %d", sys_info.procs);
		source.Reply("");
		source.Reply("System Name: %s\tRelease: %s %s\tMachine: %s", uts.nodename, uts.sysname, uts.release, uts.machine);
		source.Reply("System Version: %s", uts.version);
#ifndef _WIN32
		source.Reply(execute("grep 'model name' /proc/cpuinfo").strip());
		source.Reply("CPUs: %s", execute("grep 'model name' /proc/cpuinfo | wc -l").c_str());
#endif
//#else
//    source.Reply("This is currently not available on windows syetems, sorry.");
//#endif
		Log(source.u, this) << "command";
	}
	bool OnHelp(CommandSource &source, const Flux::string &nill)
	{
		this->SendSyntax(source);
		source.Reply(" ");
		source.Reply("This command shows the system statistics that\n"
		             "the bot is running on. This can be useful to\n"
		             "quickly see how much stress the system is under\n"
		             "that may affect the bot or to see system information");
		return true;
	}
};

class m_system : public Module
{
	CommandChown cmdchown;//So many commands! .-.
	CommandKick cmdkick;
	CommandRehash cmdrehash;
	CommandQuit cmdquit;
	CommandRestart cmdrestart;
	CommandTopic topic;
	CommandStats stats;
	CommandNick nick;
	CommandPID pid;
	CommandPass pass;
private:
	Flux::string orig;
public:
	m_system(const Flux::string &Name): Module(Name), cmdchown(this), cmdkick(this), cmdrehash(this), cmdquit(this), cmdrestart(this),
		topic(this), stats(this), nick(this), pid(this), pass(this)
	{
		Implementation i[] = { I_OnNumeric, I_OnJoin, I_OnKick, I_OnNotice, I_OnChannelOp };
		ModuleHandler::Attach(i, this, sizeof(i) / sizeof(Implementation));
		this->SetAuthor("Justasic");
		this->SetVersion(VERSION);
		this->SetPermanent(true);
		orig = Config->BotNick;
	}
	void OnNumeric(int i, const std::vector<Flux::string> &params)
	{
		if((i == 5))
		{
			// Skip the nickname and the "are supported by this server" parts of the message
			for(unsigned o = 1; o < params.size() - 1; ++o)
			{
				Flux::vector sentence = ParamitizeString(params[o], '=');
				Flux::string word = sentence[0];
				Flux::string param = sentence.size() > 1 ? sentence[1] : "";

				if(word.equals_ci("NETWORK"))
					isupport.Network = param;

				if(word.equals_ci("CHANTYPES"))
					isupport.ChanTypes = param;

				if(word.equals_ci("AWAYLEN"))
					isupport.AwayLen = static_cast<int>(param);

				if(word.equals_ci("KICKLEN"))
					isupport.KickLen = static_cast<int>(param);

				if(word.equals_ci("MAXBANS"))
					isupport.MaxBans = static_cast<int>(param);

				if(word.equals_ci("MAXCHANNELS"))
					isupport.MaxChannels = static_cast<int>(param);

				if(word.equals_ci("CHANNELLEN"))
					isupport.MaxChannelLen = static_cast<int>(param);

				if(word.equals_ci("NICKLEN"))
					isupport.NickLen = static_cast<int>(param);

				if(word.equals_ci("TOPICLEN"))
					isupport.TopicLen = static_cast<int>(param);

				isupport.other[word] = param;
			}
		}

		if((i == 4))
		{
			isupport.ServerHost = params[1];
			isupport.IRCdVersion = params[2];
			isupport.UserModes = params[3];
			isupport.ChanModes = params[4];

			if(params[3].search('B')) //Set bot mode if the network has it.
				ircproto->mode(Config->BotNick, "+B");

			sepstream cs(Config->Channel, ',');
			Flux::string tok;

			while(cs.GetToken(tok))
			{
				tok.trim();
				Channel *c = new Channel(tok);
				c->SendJoin();

				Flux::string WelcomeMessage = Config->WelcomeMessage.replace_all_ci("{botnick}", Config->BotNick);
				WelcomeMessage.trim();

				if(!WelcomeMessage.empty())
					c->SendMessage(WelcomeMessage.c_str());
			}

			if(!Config->OperatorAccount.empty() || !Config->OperatorPass.empty())
			{
				ircproto->oper(Config->OperatorAccount, Config->OperatorPass);
				IsOper = true;
				ircproto->o = new Oper();
			}

			Log() << "Successfully connected to the server \"" << Config->Server + ":" + Config->Port << "\" Master Channel(s): " << Config->Channel;
		}

		if((i == 433))
		{
			Config->BotNick.push_back(Flux::RandomNickString(5));
			ircproto->nick(Config->BotNick);
			istempnick = true;
		}

		if((i == 376))
		{
			Log(LOG_TERMINAL) << "\033[22;31mStarted with PID \033[22;32m" << getpid() << Config->LogColor;
			Log(LOG_TERMINAL) << "\033[22;34mSession Password: \033[01;32m" << password << Config->LogColor;

			for(unsigned o = 0; o < Config->Owners.size(); ++o)
				ircproto->notice(Config->Owners[o], "The randomly generated password is: %s", password.c_str());

			started = true;

			/* Identify to the networks services */
			if((!Config->ServicesAccount.empty() || !Config->ServicesPass.empty()) && Config->IdentOnConn)
			{
				Flux::string Sendns = Config->ServicesSendString.replace_all_ci("%a", Config->ServicesAccount).replace_all_ci("%p", Config->ServicesPass);
				ircproto->privmsg(Config->ServicesService, Sendns);
				Log() << "Identified to " << Config->ServicesService << " with account \"" << Config->ServicesAccount << "\"";
			}

			//Try and regain our nickname
			if(istempnick)
			{
				Config->BotNick = orig + "_";
				ircproto->nick(Config->BotNick);
				istempnick = false;
			}
		}
	}

	void OnJoin(User *u, Channel *c)
	{
		u->SendMessage("Welcome %s to %s. Type !time for time or \"/msg %s help\" for help on more commands.", u->nick.c_str(),
		               c->name.c_str(), Config->BotNick.c_str());
		Log(u) << "joined " << c->name;
	}

	void OnKick(User *u, User *kickee, Channel *c, const Flux::string &reason)
	{
		if(kickee && kickee->nick.equals_ci(Config->BotNick))
		{
			Log(u) << "kicked me from " << c->name << '(' << reason << ')';
			c->SendJoin();
		}
	}

	void OnNotice(User *u, const Flux::vector &params)
	{
		Flux::string msg = ConcatinateVector(params);

		// Auto-Identify
		if(msg.search(Config->AutoIdentString))
		{
			if((!Config->ServicesPass.empty() || !Config->ServicesAccount.empty()) && u->nick == Config->ServicesService && Config->IdentOnConn)
			{
				Flux::string Sendns = Config->ServicesSendString.replace_all_ci("%a", Config->ServicesAccount).replace_all_ci("%p", Config->ServicesPass);
				u->SendPrivmsg(Sendns);
				Log() << "Identified to " << u->nick << " with account \"" << Config->ServicesAccount << "\"";
			}
		}
	}
	void OnNickChange(User *u, const Flux::string &newnick)
	{
		if(u->nick == Config->BotNick)
			Config->BotNick = newnick;

		// Keep owners in the owners vector updated..
		if(u->IsOwner())
		{
			for(unsigned i = 0; i < Config->Owners.size(); ++i)
				if(newnick.equals_ci(Config->Owners[i]))
					Config->Owners[i] = newnick;
		}
	}
};

MODULE_HOOK(m_system)
