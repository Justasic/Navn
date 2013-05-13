/* Navn IRC bot -- Channel for logging
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */
#include "modules.h"

Flux::string NoTermColor(const Flux::string &ret)
{
	Flux::string str;
	bool in_term_color = false;

	for(unsigned i = 0; i < ret.length(); ++i)
	{
		char c = ret[i];

		if(in_term_color)
		{
			if(c == 'm')
				in_term_color = false;

			continue;
		}

		if(c == '\033')
		{
			in_term_color = true;
			continue;
		}

		if(!in_term_color)
			str += c;
	}

	return str;
}

class LogChan : public Module
{
public:
	LogChan(const Flux::string &Name): Module(Name)
	{
		this->SetVersion(VERSION);
		this->SetAuthor("Justasic");
		ModuleHandler::Attach(I_OnLog, this);

		if(Config->LogChan.empty())
			throw ModuleException("Log channel is empty, cannot load m_logchan!");
	}

	EventResult OnLog(Log *l)
	{
		Channel *c = findchannel(Config->LogChan);

		if(!c)
			return EVENT_CONTINUE;

		std::stringstream logstream;
		Flux::string message = Flux::Sanitize(l->buffer.str());

		if(l->u && !l->c)
			message = l->u->nick + " " + message;

		if(l->u && l->c)
			message = l->u->nick + " used " + l->c->name + " " + message;

		switch(l->type)
		{
			case LOG_NORMAL:
				logstream << message;
				break;
			case LOG_THREAD:

				if(protocoldebug)
					logstream << "[THREAD] " << message;

				break;
			case LOG_DEBUG:

				if(dev || protocoldebug)
					logstream << message;

				break;
//       case LOG_DEVEL:
//  if(!protocoldebug && dev)
//    logstream << message;
//  break;
			case LOG_RAWIO:
				return EVENT_CONTINUE;
			case LOG_CRITICAL:
				logstream << "\0034[CRITICAL] " << message << "\017";
				break;
			case LOG_WARN:
				logstream << "\0037[WARNING]\017 " << message;
				break;
			default:
				break;
		}

		c->SendMessage(NoTermColor(logstream.str()));
		return EVENT_CONTINUE;
	}
};

MODULE_HOOK(LogChan)