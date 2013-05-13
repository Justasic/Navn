/* Navn IRC bot -- ?
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */

#include "modules.h"

XMLFile *xf = new XMLFile("myxml.xml");

class QuickReply : public Module
{
public:
	QuickReply(const Flux::string &Name): Module(Name)
	{
		this->SetVersion("1.0");
		this->SetAuthor("Lordofsraam");
		this->SetPriority(PRIORITY_LAST);
		Implementation i[] = { I_OnPrivmsgChannel, I_OnNotice };
		ModuleHandler::Attach(i, this, sizeof(i) / sizeof(Implementation));
	}
	void OnPrivmsgChannel(User *u, Channel *c, const Flux::vector &params)
	{
		Flux::string msg;

		for(unsigned i = 0; i < params.size(); ++i)
			msg += params[i] + ' ';

		if(msg.search_ci("!rehash"))
		{
			u->SendMessage("Rehashing xml file...");
			xf = new XMLFile("myxml.xml");
			u->SendMessage("XML file has been rehashed.");
		}

		if(msg.search_ci(xf->Tags["trigger"].Attributes["word"].Value))
		{
			c->SendMessage(xf->Tags["trigger"].Content);
		}
	}
	void OnNotice(User *u, const Flux::vector &params)
	{
		Flux::string msg;

		for(unsigned i = 0; i < params.size(); ++i)
			msg += params[i] + ' ';
	}
};
MODULE_HOOK(QuickReply)
