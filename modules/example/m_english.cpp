/* Navn IRC bot -- Example English corrector for common english mistakes
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */

// I wrote this Module mostly because I am disappointed by the lack of
// proper and correct English. Mastering proper English is NOT HARD and
// people should learn to not take shortcuts.
#include "modules.h"

class English : public Module
{
public:
	English(const Flux::string &Name): Module(Name)
	{
		this->SetAuthor("Justasic"); //Set the author
		this->SetVersion(VERSION);
		ModuleHandler::Attach(I_OnPrivmsgChannel, this);
	}

	void OnPrivmsgChannel(User *u, Channel *c, const Flux::vector &params) override
	{
		Flux::string sentence = ConcatinateVector(params);

		// Capitalize the 1st letter of every sentence
		if(islower(params[0][0]))
			c->SendMessage("%s, The '%c' in \"%s\" should be capitalized.", u->nick.capwords().c_str(), params[0][0], params[0].c_str());

		// Make sure all sentences end with periods, short phrases shouldn't be annoying enough to correct
		char ch = sentence[sentence.size() - 1];

		if(ch != '.' && params.size() > 7 && (ch != '?' || ch != '!'))
			c->SendMessage("%s, Your sentence should end with a period.", u->nick.capwords().c_str());

		// Make sure the bots nickname is capitalized.
		if(sentence.search_ci(Config->BotNick))
			for(auto & param : params)
			{
				if(param.search_ci(Config->BotNick) && !param.search(Config->BotNick))
				{
					// Make sure we're not lower case or it would be ironic to say "capitalize" when we're lowercase
					if(!islower(Config->BotNick[0]))
						c->SendMessage("%s, You should've capitalized the '%c' in my nickname.", u->nick.capwords().c_str(), Config->BotNick[0]);
				}
			}

		// Common shortcuts
		if(sentence.search_ci(" u "))
			c->SendMessage("*you");

		if(sentence.search_ci("ikr"))
			c->SendMessage("*I know right");

		if(sentence.search_ci(" u r "))
			c->SendMessage("*you are");

		if(sentence.search_ci(" ur "))
			c->SendMessage("*your");

		if(sentence.search_ci("alot"))
			c->SendMessage("*a lot (alot is not a word)");

		if(sentence.search(" i "))
			c->SendMessage("*I");

		if(sentence.search_ci("idk"))
			c->SendMessage("*I don't know");

		if(sentence.search_ci("sry"))
			c->SendMessage("*sorry");

		if(sentence.search_ci("cuz"))
			c->SendMessage("*because");

		if(sentence.search_ci("sry"))
			c->SendMessage("*sorry");

		if(sentence.search_ci("stfu"))
			c->SendMessage("*Shut the fuck up");

		if(sentence.search_ci("plz"))
			c->SendMessage("*please");

		if(sentence.search_ci("imma"))
			c->SendMessage("*I am going to");

		// American misspellings
		if(sentence.search_ci("criticise"))
			c->SendMessage("We're in America, use \"criticize\".");

		// Common mistakes or shortcuts, these have extended sentences.
		if(sentence.search_ci("theyre"))
			c->SendMessage("THERE'S AN APOSTROPHE IN THEY'RE!");

		if(sentence.search_ci("haus"))
			c->SendMessage("In German the word \"Haus\" or \"das Haus\" means House, this is English so spell it with an 'e'!");

		// Words where people don't like apostrophes (therefore they're not words)
		if(sentence.search_ci("cant"))
			c->SendMessage("*can't");

		if(sentence.search_ci("dont"))
			c->SendMessage("*don't");

		if(sentence.search_ci("wont"))
			c->SendMessage("*won't");

		if(sentence.search_ci("arent"))
			c->SendMessage("*aren't");

		if(sentence.search_ci("wont"))
			c->SendMessage("*won't");

		if(sentence.search_ci("isnt"))
			c->SendMessage("*isn't");

		if(sentence.search_ci("wasnt"))
			c->SendMessage("*wasn't");

		if(sentence.search_ci("couldnt"))
			c->SendMessage("*couldn't");

		if(sentence.search_ci("shouldnt"))
			c->SendMessage("*shouldn't");

		if(sentence.search_ci("havent"))
			c->SendMessage("*haven't");

		if(sentence.search_ci("hadnt"))
			c->SendMessage("*hadn't");

		if(sentence.search_ci("doesnt"))
			c->SendMessage("*doesn't");
	}
};

MODULE_HOOK(English)