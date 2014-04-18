/* Navn IRC bot -- User information Module
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */
#include "modules.h"
/**
 * \file about_me.h Header file holding the \a about_me function.
 * \author Justasic. Polished by Lordofsraam.
 */

/**
 * \defgroup about_meM About Me Module
 * This is the Module for the about me function. \n
 * This Module was originally a test/example Module that grew. \n
 * The Module just tells you about you and your hostname in IRC (nickname, channel it was said in, hostname, etc.) \n
 * For a better description see the function description.
 * \section commands Commands associated with this Module.
 * \subsection about_me about me
 * Say \a about \a me to see your various host information that navn can see and decode.
 * \subsection decodehost !decodehost
 * Say \a !decodehost with a hostname and navn will decode it into ident, nickname, hostname, etc.
 * @{
 */
class CommandDecodeHost : public Command
{
public:
	CommandDecodeHost(Module *m): Command(m, "DECODEHOST", C_CHANNEL, 1, 1)
	{
		this->SetDesc("Decodes a hostname");
		this->SetSyntax("\37fullhost\37");
	}
	void Run(CommandSource &source, const Flux::vector &params) override
	{
		Channel *c = source.c;
		Flux::string host = params[1];
		auto Host = new IsoHost(host);
		c->SendMessage("Nick: %s", Host->nick.c_str());
		c->SendMessage("User: %s", Host->ident.c_str());
		c->SendMessage("Host: %s", Host->host.c_str());
		c->SendMessage("Raw: %s", Host->raw.c_str());
		delete Host;
	}
	bool OnHelp(CommandSource &source, const Flux::string &nill) override
	{
		this->SendSyntax(source);
		source.Reply(" ");
		source.Reply("This command simply shows you your\n"
		             "hostname decoded into separate strings");
		return true;
	}
};

/**
 * \fn class about_me(bool a):Module("About Me", a, PRIORITY_DONTCARE){ this->SetDesc("Returns the information about yourself"); }
 * \brief Tells you about youself (ie. Host, ident, etc.).
 * Uses irc_string and IsoHost to decode the message you said.
 */
class about_me : public Module
{
	CommandDecodeHost host;
public:
	about_me(const Flux::string &Name): Module(Name), host(this)
	{
		this->SetAuthor("Justasic");
		this->SetVersion(VERSION);
		this->SetPriority(PRIORITY_LAST);
		ModuleHandler::Attach(I_OnPrivmsgChannel, this);
	}

	void OnPrivmsgChannel(User *u, Channel *c, const Flux::vector &params) override
	{
		Flux::string msg = ConcatinateVector(params);

		if(msg.equals_ci("about me"))
		{
			u->SendMessage("Raw: %s", sock->GetLastBuf().c_str());
			u->SendMessage("Message: %s", msg.c_str());
			u->SendMessage("Nickname: %s", u->nick.c_str());
			u->SendMessage("Ident: %s", u->ident.c_str());
			u->SendMessage("Host: %s", u->host.c_str());
			u->SendMessage("Channel: %s", c->name.c_str());
			u->SendMessage("Fullhost: %s", u->fullhost.c_str());
			Log() << u->nick << " requested information about themself";
		}
	}
};
/**
 * @}
 */
MODULE_HOOK(about_me)
