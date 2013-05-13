/* Navn IRC bot -- Example Module
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */

#include "modules.h"

class CommandSimple : public Command
{
public:
	CommandSimple(Module *m): Command(m, "HI", C_PRIVATE)
	{
		this->SetDesc("Says Hi");
	}

	void Run(CommandSource &source, const Flux::vector &params)
	{
		source.Reply("HI!");
	}

	bool OnHelp(CommandSource &source, const Flux::string &nill)
	{
		this->SendSyntax(source);
		source.Reply(" ");
		source.Reply("This is the help for the simple Module\n"
		             "This is line 2 of the help in the simple Module");
		return true;
	}
};

class SimpleMod : public Module
{
	CommandSimple s;
public:
	SimpleMod(const Flux::string &Name): Module(Name), s(this)
	{
		this->SetVersion("1.0");
		this->SetAuthor("Justasic & Lordofsraam");
		this->SetPriority(PRIORITY_LAST);
		Implementation i[] = { I_OnPrivmsg, I_OnNotice };
		ModuleHandler::Attach(i, this, sizeof(i) / sizeof(Implementation));
	}

	void OnPrivmsg(User *u, const Flux::vector &params)
	{
		Flux::string msg = ConcatinateVector(params);

		if(msg.search("I am a very long command"))
			Log(LOG_TERMINAL) << "PRIVMSG: " << msg;
	}

	void OnNotice(User *u, const Flux::vector &params)
	{
		Flux::string msg = ConcatinateVector(params);

		if(msg.search("I am a very long notice"))
			Log(LOG_TERMINAL) << "NOTICE: " << msg;
	}
};
MODULE_HOOK(SimpleMod)
