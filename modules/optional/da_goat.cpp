/* Navn IRC bot -- Da_Goat script in C++
 *
 * (C) 2011-2012 Azuru
 * Contact us at Dev@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */
#include "modules.h"

/**
 * \file da_goat.h Header file holding the \a Da_Goat function.
 * \author Lordofsraam & Justasic
 * maintained by \a DeathBlade
 */

/**
 * \defgroup da_goatM Da Goat Module
 * This is the Module for the Da Goat.
 * For a better description see the function description.
 * \section commands Commands associated with this Module.
 * \subsection poke !poke
 * Say \a !poke to poke Da Goat!
 * \subsection info !info
 * Say \a !info to get information about Azuru.
 * \subsection rename !rename
 * Say \a !rename to learn how to rename yourself.
 * \subsection register !register
 * Say \a !register to learn how to reegister yourself.
 * \subsection socialinfo !socialinfo
 * Say \a !socialinfo to learn about contacting Azuru.
 * \subsection help !help
 * Say \a !help to get help on IRC.
 * \subsection ts3 !ts3
 * Say \a !ts3 to get information about Azuru's TeamSpeak 3 server.
 * \subsection uptime !uptime
 * Say \a !uptime to get the uptime of the server that Navn is running on.
 * \subsection rules !rules
 * Say \a !rules to learn about our IRC rules.
 * \subsection spam !spam
 * Say \a !spam to get Azuru's definition of
 * \subsection version !version
 * Say \a !version to get Navn's verion.
 * \subsection everything !everything
 * Say \a !everything
 * @{
 */

/**
 * \fn class Da_Goat(bool a):Module("Da_Goat", a, PRIORITY_DONTCARE){ this->SetDesc("Da_Goat bot"); }
 * \brief Runs Da_Goat
 * Da Goat is used to repeat a lot of things we got tired of saying, like teaching new people how to do thigns and what the
 * rules of the server were.
 */
class CommandCVersion: public Command
{
public:
	CommandCVersion(Module *m): Command(m, "VERSION", C_CHANNEL)
	{
		this->SetDesc("Displays the bots version info into the channel");
	}

	void Run(CommandSource &source, const Flux::vector &params) override
	{
		User *u = source.u;
		Channel *c = source.c;
		c->SendMessage("The Current Navn Bot Version is \002\0037v%s\017", VERSION);
		c->SendMessage("Navn's code can be found at \002git://gitorious.org/navn/navn.git");
		c->SendMessage("Report all bugs at: \2http://bugs.Azuru.net\2");
		c->SendMessage("%s is managed by: \2%s\2", Config->BotNick.c_str(), ConcatinateVector(Config->Owners).c_str());
		Log(u, this) << "command in " << c->name;
	}

	bool OnHelp(CommandSource &source, const Flux::string&) override
	{
		this->SendSyntax(source);
		source.Reply(" ");
		source.Reply("This command shows %s's version information\n"
		             "it also shows where to report bugs to, who the\n"
		             "bots owner is, and the git repository.", Config->BotNick.c_str());
		return true;
	}
};

class CommandCSpam: public Command
{
public:
	CommandCSpam(Module *m): Command(m, "SPAM", C_CHANNEL)
	{
		this->SetDesc("Displays the spam warning");
	}

	void Run(CommandSource &source, const Flux::vector &params) override
	{
		Log(source.u, this) << "command in " << source.c->name;
		source.c->SendMessage("Spam is the abuse of electronic messaging systems. This includes (but not limited to) external links, Flooding, mass join/quit messages, mass private messages or notices, mIRC color code abuse, CTCP abuse, mass nick changes, etc. If you violate the spam policy you will be kicked.");
	}

	bool OnHelp(CommandSource &source, const Flux::string&) override
	{
		this->SendSyntax(source);
		source.Reply(" ");
		source.Reply("This command shows the rules and policies\n"
		             "Azuru has on channel spam");
		return true;
	}
};

class CommandCASK: public Command
{
public:
	CommandCASK(Module *m): Command(m, "ASK", C_CHANNEL)
	{
		this->SetDesc("Displays the Ask protocol");
	}

	void Run(CommandSource &source, const Flux::vector &params) override
	{
		Log(source.u, this) << "command in " << source.c->name;
		source.c->SendMessage("The Ask To Ask protocol wastes more bandwidth than any version of the Ask protocol, so just ask your question. (Read: don't ask to ask a question, just ask your question)");
	}

	bool OnHelp(CommandSource &source, const Flux::string&) override
	{
		this->SendSyntax(source);
		source.Reply(" ");
		source.Reply("This command shows the Ask to Ask protocol");
		return true;
	}
};

class CommandCRules: public Command
{
public:
	CommandCRules(Module *m): Command(m, "RULES", C_CHANNEL)
	{
		this->SetDesc("Displays the rules of the chatroom");
	}
	
	void Run(CommandSource &source, const Flux::vector &params) override
	{
		Channel *c = source.c;
		User *u = source.u;
		c->SendMessage("There are only a few simple rules for %s.", c->name.c_str());
		c->SendMessage("Do NOT hate on others in any way. Basically do not troll in any shape or form.");
		c->SendMessage("Do not ask for op status. you will be granted op status when the moderators feel you deserve op status.");
		c->SendMessage("Do not Mini-mod the chat room. The moderators are there for a reason, we don't need more.");
		c->SendMessage("Do not spam the chat room. This includes flooding by text, private messages, join/quit commands, External links, etc.");
		c->SendMessage("If you violate any of these rules you will be kicked and possibly banned from %s.", c->name.c_str());
		Log(u, this) << "command in " << c->name;
	}
	
	bool OnHelp(CommandSource &source, const Flux::string&) override
	{
		this->SendSyntax(source);
		source.Reply(" ");
		source.Reply("This command simply shows the channel rules for\n"
		             "Azuru's main channel #Computers, this command\n"
		             "is pretty specific to Azuru");
		return true;
	}
};

class CommandCUptime: public Command
{
	struct sysinfo sys_info;
public:
	CommandCUptime(Module *m): Command(m, "UPTIME", C_CHANNEL)
	{
		this->SetDesc("Prints Syatem Uptime in the channel");
	}

	void Run(CommandSource &source, const Flux::vector &params) override
	{
		Channel *c = source.c;
		User *u = source.u;
#ifndef _WIN32

		if(sysinfo(&sys_info) != 0)
			perror("sysinfo");

		// Uptime
		int days, hours, mins;
		days = sys_info.uptime / 86400;
		hours = (sys_info.uptime / 3600) - (days * 24);
		mins = (sys_info.uptime / 60) - (days * 1440) - (hours * 60);
		c->SendMessage("System Uptime: %d days, %d hours, %d minutes, %ld seconds", days, hours, mins, sys_info.uptime % 60);
		c->SendMessage("Bot Uptime: %s", duration(time(nullptr) - starttime).c_str());
#else
		source.Reply("This is currently not available on windows systems, sorry.");
#endif
		Log(u, this) << "command in " << c->name;
	}

	bool OnHelp(CommandSource &source, const Flux::string&) override
	{
		this->SendSyntax(source);
		source.Reply(" ");
		source.Reply("This command shows both the bots uptime\n"
		             "and the Operating System's uptime. The\n"
		             "command can be used to compare uptimes\n"
		             "if there is more than one bot in the chanenl");
		return true;
	}
};

class CommandCSocialInfo: public Command
{
public:
	CommandCSocialInfo(Module *m): Command(m, "SOCIALINFO", C_CHANNEL)
	{
		this->SetDesc("Displays Azuru's Social Information");
	}

	void Run(CommandSource &source, const Flux::vector &params) override
	{
		Channel *c = source.c;
		User *u = source.u;
		c->SendMessage("Our IRC server: \002irc.Azuru.net:6667\002");
		c->SendMessage("Minecraft Server: \002 Minecraft.Azuru.net port: 25565 (default)\002");
		c->SendMessage("The Azuru TeamSpeak 3 server is:\002Pulsar.Azuru.net:9987\002");
		Log(u, this) << "command in " << c->name;
	}

	bool OnHelp(CommandSource &source, const Flux::string&) override
	{
		this->SendSyntax(source);
		source.Reply(" ");
		source.Reply("This command shows Azuru's information\n"
		             "used to tell people where to go for various\n"
		             "other activities in the network");
		return true;
	}
};

class CommandCRegister: public Command
{
public:
	CommandCRegister(Module *m): Command(m, "REGISTER", C_CHANNEL)
	{
		this->SetDesc("Displays how to register your nickname");
	}

	void Run(CommandSource &source, const Flux::vector &params) override
	{
		Channel *c = source.c;
		User *u = source.u;
		c->SendMessage("To Register your nickname type: /register <Your password>");
		c->SendMessage("REMEMBER THIS PASSWORD! YOU WILL USE IT EVERY TIME YOU JOIN!");
		Log(u, this) << "command in " << c->name;
	}

	bool OnHelp(CommandSource &source, const Flux::string&) override
	{
		this->SendSyntax(source);
		source.Reply(" ");
		source.Reply("This command is pretty simple. It shows\n"
		             "users how to register their nickname(s) to\n"
		             "the networks services\n"
		             "Note: Registration information was based on Anope IRC services"
		            );
		return true;
	}

};

class CommandCRename: public Command
{
public:
	CommandCRename(Module *m): Command(m, "RENAME", C_CHANNEL)
	{
		this->SetDesc("Displays how to rename");
	}

	void Run(CommandSource &source, const Flux::vector &params) override
	{
		Channel *c = source.c;
		User *u = source.u;
		c->SendMessage("This channel is a Nickname only channel. This means that you MUST have your own Nickname! If you do not choose your own nick name you WILL be kicked.");
		c->SendMessage("To change your nickname type (without quotes) '/nick MyNewNickname' to change your nickname. (replacing MyNewNickname with a personal nickname).");
		Log(u, this) << "command in " << c->name;
	}

	bool OnHelp(CommandSource &source, const Flux::string&) override
	{
		this->SendSyntax(source);
		source.Reply(" ");
		source.Reply("This command shows how to change your\n"
		             "nickname in IRC for new users who may\n"
		             "not understand how to use IRC"
		            );
		return true;
	}

};

class CommandCInfo : public Command
{
public:
	CommandCInfo(Module *m): Command(m, "INFO", C_CHANNEL)
	{
		this->SetDesc("General Da_Goat info");
	}

	void Run(CommandSource &source, const Flux::vector &params) override
	{
		Channel *c = source.c;
		User *u = source.u;
		c->SendMessage("Our forum is at \037http://forum.Azuru.net/\017");
		c->SendMessage("Our Website is \002www.Azuru.net\017");
		c->SendMessage("FTP server \002178.63.127.231\002 login anonymous \002-no password-\002, Files in dir \002/ftp/pub\002");
		Log(u, this) << "command in " << c->name;
	}

	bool OnHelp(CommandSource &source, const Flux::string&) override
	{
		this->SendSyntax(source);
		source.Reply(" ");
		source.Reply("This command shows all of Azuru's\n"
		             "information, such as the forums url");
		return true;
	}

};

class Da_Goat : public Module
{
	CommandCInfo info;
	CommandCRename rename;
	CommandCRegister reg;
	CommandCSocialInfo sinfo;
	CommandCUptime uptime;
	CommandCRules rules;
	CommandCSpam spam;
	CommandCVersion ver;
	CommandCASK ask;
public:
	Da_Goat(const Flux::string &Name): Module(Name), info(this), rename(this), reg(this), sinfo(this), uptime(this), rules(this), spam(this), ver(this), ask(this)
	{
		this->SetAuthor("Justasic & Lordofsraam");
		this->SetVersion(VERSION);
		ModuleHandler::Attach(I_OnPrivmsgChannel, this);
	}

	void OnPrivmsgChannel(User *u, Channel *c, const Flux::vector &params) override
	{
		Flux::string cmd = params.empty() ? "" : params[0], msg = ConcatinateVector(params);

		/******************************Easter Eggs*********************************/
		if(cmd.equals_ci("!poke"))
		{
			//Easter egg ;P
			Flux::string person = params.size() == 2 ? params[1] : "";
			person.trim();
			User *u2 = finduser(person);

			if(u2 && u2->IsOwner())
			{
				c->SendMessage("why would I wanna kick my master!?");
				return;
			}
			else if(!u2)
			{
				c->SendAction("is angry at %s", u->nick.c_str());
				c->kick(u, "\002\00315Don't poke me!\017");
				Log() << u->nick << " found Da_Goats !poke command in " << c->name;
			}
			else
			{
				c->SendAction("is angry at %s", u2->nick.c_str());
				c->kick(u2, "\002\00315Don't poke me!\017");
				Log(u) << "used Da_Goats !poke command in " << c->name << "to poke " << u2->nick;
			}
		}

		/****************************************slap command*******************************************/
		if(cmd.equals_ci("!slap"))
		{
			//Easter egg ;P
			Flux::string person = params.size() == 2 ? params[1] : "";
			person.trim();
			User *u2 = finduser(person);

			if(u2 && u2->IsOwner())
			{
				c->SendMessage("why would I wanna slap my master!?");
				return;
			}
			else if(!u2)
			{
				c->SendAction("bitch slaps %s", u->nick.c_str());
				c->kick(u, "\002\00315Don't slap me!\017");
				Log() << u->nick << " found Da_Goats !poke command in " << c->name;
			}
			else
			{
				c->SendAction("bitch slaps %s", u2->nick.c_str());
				c->kick(u2, "\002\00315Don't slap me!\017");
				Log(u) << "used Da_Goats !slap command in " << c->name << "to poke " << u2->nick;
			}
		}

		/***************************************************************************************************/
//     if(msg.search_ci("no u!") || msg.search_ci("no u") || msg.search_ci("no you") || msg.search_ci("no you!"))
//       c->SendMessage("NO U!");
		if(cmd.equals_ci("!everything"))
			c->SendMessage("Yes, there is a script for everything...\007");

		/***********************End Da_Goat Functions******************************/
		if(cmd.equals_ci("!bugs"))
			c->SendMessage("Report Bugs at: http://bugs.Azuru.net/");

		if(cmd.equals_ci("!git"))
			u->SendMessage("Navn git: git://github.com/Justasic/Navn.git");

//     if(msg.search_ci("the game") || msg.search_ci("won the game"))
//       c->SendMessage("YOU JUST LOST THE GAME.");
//     if(msg.search_ci("i win"))
//       c->SendMessage("YOU LOSE!");
		if(msg.search_ci("!navn"))
			c->SendMessage("That's my name don't wear it out!");

		if(msg.search_ci("!login"))
			c->SendMessage("Type: /login <yourpassword>");

		if(msg.search_ci("brb"))
			c->SendMessage("I miss %s", u->nick.c_str());

//     if(msg.search_ci("shut up"))
//       c->SendMessage("U SHUT UP!");
//     if(msg.search_ci("shut the fuck up"))
//       c->SendMessage("U SHUT THE FUCK UP!");
		/*if(params[0].search_ci("why") && c->name.equals_ci("#Minecraft"))
		  c->SendMessage("Because you touch yourself at night!");*/
	}
};

MODULE_HOOK(Da_Goat)
/**
 * @}
 */
