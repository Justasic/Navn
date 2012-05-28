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
  for(unsigned i=0; i < ret.length(); ++i)
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

Flux::string TranslateColors(const Flux::string &string)
{
  Flux::string str = string;
  int attrcolor = 0;
  int colorcode = 0;
  size_t substrpos = 0;
  size_t substrposend = 0;
  bool in_term_color = false;

  str = str.replace_all_cs("\033[0m", "\017");
  str = str.replace_all_cs("\033[0;0m", "\017");
  
  for(unsigned i=0; i < str.length(); ++i)
  {
    char c = str[i];
    if(c == '\033')
    {
      substrpos = i;
      continue;

      if(c == 'm')
      {
	in_term_color = false;
	substrposend = i;
	continue;
      }
      
      if(c == '[')
      {
	in_term_color = true;
	continue;
      }

      if(in_term_color && c-- == '[' && isdigit(c))
      {
	Flux::string attrcode = c;
	if(isdigit(++c))
	  attrcode += ++c;
	attrcolor = (int)attrcode;
	continue;
      }
      
      if(in_term_color && c-- == ';' && isdigit(c))
      {
	Flux::string ccode = c;
	if(isdigit(++c))
	  ccode += ++c;
	colorcode = (int)ccode;
      }
      

    }
  }

  Flux::string build;
  // decode all the attribute codes we can.
  switch(attrcolor)
  {
    case 0:
      break;
    case 1: // Bold
      build += '\002';
    case 4:
      build += '\037';
    default:
      break;
  }

  // convert colors to irc formatting
  build += "\003" + colorcode;
  str = str.substr(0, substrpos) + build + str.substr(substrposend);
  return NoTermColor(str);
}

class LogChan : public module
{
public:
  LogChan(const Flux::string &Name):module(Name)
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
    if(!c || l->type == LOG_RAWIO)
      return EVENT_CONTINUE;

    c->SendMessage(TranslateColors(l->buffer.str()));
    return EVENT_CONTINUE;
  }
};

MODULE_HOOK(LogChan)