/* Navn IRC bot -- eightball game
 *
 * (C) 2011-2012 Azuru
 * Contact us at Dev@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 * Module based on the original code of eightball.c from sigyn IRC bot
 */

#include "modules.h"

class CommandEightBall : public Command
{
public:
	CommandEightBall(Module *m) : Command(m, "EIGHTBALL", C_CHANNEL, 1, -1)
	{
		this->SetDesc("Ask 8-ball a question");
		this->SetSyntax("\37[question]\37");
	}

	void Run(CommandSource &source, const Flux::vector &params) override
	{
		static Flux::string responses[21] =
		{
			"It is certain.",
			"It is decidedly so.",
			"Without a doubt.",
			"Yes -- definitely.",
			"You may rely on it.",
			"As I see it, yes.",
			"Most likely.",
			"Outlook good.",
			"Yes.",
			"Signs point to yes.",
			"Reply hazy, try again.",
			"Ask again later.",
			"Better not tell you now.",
			"Cannot predict now.",
			"Concentrate and ask again.",
			"Don't count on it.",
			"My reply is no.",
			"My sources say no.",
			"Outlook not so good.",
			"Very doubtful.",
			"No."
		};

		source.Reply(responses[(rand() % 21)]);
	}

	bool OnHelp(CommandSource &source, const Flux::string &nill) override
	{
		this->SendSyntax(source);
		source.Reply(" ");
		source.Reply("This command is a basic 8-ball game."
		             "type \"!eightball\" in any channel");
		return true;
	}
};

class EightBall : public Module
{
	CommandEightBall command8ball;
public:
	EightBall(const Flux::string &Name) : Module(Name), command8ball(this)
	{
		this->SetAuthor("Justasic");
		this->SetVersion(VERSION);
		this->SetPriority(PRIORITY_LAST);
	}
};

MODULE_HOOK(EightBall)