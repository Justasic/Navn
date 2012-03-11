/* Navn IRC bot -- Search url generator
 * 
 * (C) 2011-2012 Flux-Net
 * Contact us at Dev@Flux-Net.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */
#include "flux_net_irc.hpp"

/**
 * \file searcher.h Header file holding the \a searcher function.
 * \author Lordofsraam. Polished by Justasic.
 */

/**
 * \defgroup searcherM Search Engine Module
 * This is the module for the searcher function.
 * This allows you to get search links for different sites inside IRC.
 * \section commands Commands associated with this module.
 * \subsection google !google
 * Say \a !google to get a search link for the rest of the message.
 * For example the message "!google hot footed waffle scooter" will give you the link to a google search for "hot footed waffle scooter"
 * The rest of the commands work in a similar fashion.
 * \subsection youtube !youtube
 * Say \a !youtube to get a results link for a youtube search.
 * \subsection tpb !tpb
 * Say \a !tpb to get a results link for a thepiratebay.com torrent search.
 * \subsection define !define
 * Say \a !define to get a results link for a dictionary.com word lookup.
 * \subsection urban !urban
 * Say \a !urban to get a results link for an urbandictionary.com search.
 * \subsection movie !movie
 * Say \a !movie to get a results link for a letmewatchthis.com movie search.
 * @{
 */

/** 
 * \fn Flux::string search(Flux::string s, Flux::string command)
 * \brief generates search links for url's
 * This is what generates the search links.
 */
Flux::string search(const Flux::string &text, const Flux::string &command){
  Flux::string searchstring = text.url_str();
  if (searchstring.empty())
    return "Empty searchstring.";
  else{
    if(command.equals_ci("!google"))
      return "http://www.google.com/search?q="+searchstring;
    else if(command.equals_ci("!youtube"))
      return "http://www.youtube.com/results?search_query="+searchstring;
    else if(command.equals_ci("!tpb"))
      return "http://thepiratebay.org/search/"+searchstring;
    else if(command.equals_ci("!define"))
      return "http://dictionary.reference.com/browse/"+searchstring;
    else if(command.equals_ci("!urban"))
      return "http://www.urbandictionary.com/define.php?term="+searchstring;
    else if(command.equals_ci("!movie"))
      return "www.letmewatchthis.ch/index.php?search_keywords="+searchstring;
    else if(command.equals_ci("!lmgtfy"))
      return "http://lmgtfy.com/?q="+searchstring;
    else
      return "http://www.google.com/search?q="+searchstring;
  }
}

class CommandCGoogle : public Command
{
public:
  CommandCGoogle(module *m):Command(m, "!GOOGLE", C_CHANNEL, 1,1)
  {
    this->SetDesc("Generate a Google search url");
    this->SetSyntax("\37message\37");
  }
  void Run(CommandSource &source, const Flux::vector &params)
  {
    Flux::string str = search(params[0], this->name);
    source.c->SendMessage(str);
    Log() << "Channel Google Search from " << source.u->nick << " \"" << str << "\"";
  }
  bool OnHelp(CommandSource &source, const Flux::string &nill)
  {
    this->SendSyntax(source);
    source.Reply(" ");
    source.Reply("This command generates a google search link\n"
		 "for the requested text");
    return true;
  }
};

class CommandCYoutube : public Command
{
public:
  CommandCYoutube(module *m):Command(m, "!YOUTUBE", C_CHANNEL, 1,1)
  {
    this->SetDesc("Generate a youtube search url");
    this->SetSyntax("\37message\37");
  }
  void Run(CommandSource &source, const Flux::vector &params)
  {
    Flux::string str = search(params[0], this->name);
    source.c->SendMessage(str);
    Log() << "Channel youtube Search from " << source.u->nick << "\"" << str << "\"";
  }
  bool OnHelp(CommandSource &source, const Flux::string &nill)
  {
    this->SendSyntax(source);
    source.Reply(" ");
    source.Reply("This command generates a youtube search link\n"
		 "for the requested text");
    return true;
  }
};

class CommandCPirateBay : public Command
{
public:
  CommandCPirateBay(module *m):Command(m, "!TPB", C_CHANNEL, 1,1)
  {
    this->SetDesc("Generate a Pirate Bay search url");
    this->SetSyntax("\37message\37");
  }
  void Run(CommandSource &source, const Flux::vector &params)
  {
    Flux::string str = search(params[0], this->name);
    source.c->SendMessage(str);
    Log() << "Channel The Pirate Bay Search from " << source.u->nick << " \"" << str << "\"";
  }
  bool OnHelp(CommandSource &source, const Flux::string &nill)
  {
    this->SendSyntax(source);
    source.Reply(" ");
    source.Reply("This command generates a 'The Pirate Bay' search link\n"
		 "for the requested text");
    return true;
  }
};

class CommandCDefine : public Command
{
public:
  CommandCDefine(module *m):Command(m, "!DEFINE", C_CHANNEL, 1,1)
  {
    this->SetDesc("Generate a Dictionary search url");
    this->SetSyntax("\37message\37");
  }
  void Run(CommandSource &source, const Flux::vector &params)
  {
    Flux::string str = search(params[0], this->name);
    source.c->SendMessage(str);
    Log() << "Channel define Search from " << source.u->nick << " \"" << str << "\"";
  }
  bool OnHelp(CommandSource &source, const Flux::string &nill)
  {
    this->SendSyntax(source);
    source.Reply(" ");
    source.Reply("This command generates a Dictionary.com search link\n"
		 "for the requested text");
    return true;
  }
};

class CommandCUrban : public Command
{
public:
  CommandCUrban(module *m):Command(m, "!URBAN", C_CHANNEL, 1,1)
  {
    this->SetDesc("Generate a Urban Dictionary search url");
    this->SetSyntax("\37message\37");
  }
  void Run(CommandSource &source, const Flux::vector &params)
  {
    Flux::string str = search(params[0], this->name);
    source.c->SendMessage(str);
    Log() << "Channel urban dictionary Search from " << source.u->nick << " \"" << str << "\"";
  }
  bool OnHelp(CommandSource &source, const Flux::string &nill)
  {
    this->SendSyntax(source);
    source.Reply(" ");
    source.Reply("This command generates a Urban Dictionary search link\n"
		 "for the requested text");
    return true;
  }
};

class CommandCMovie : public Command
{
public:
  CommandCMovie(module *m):Command(m, "!MOVIE", C_CHANNEL, 1,1)
  {
    this->SetDesc("Generate a movie search url");
    this->SetSyntax("\37message\37");
  }
  void Run(CommandSource &source, const Flux::vector &params)
  {
    Flux::string str = search(params[0], this->name);
    source.c->SendMessage(str);
    Log() << "Channel movie Search from " << source.u->nick << " \"" << str << "\"";
  }
  bool OnHelp(CommandSource &source, const Flux::string &nill)
  {
    this->SendSyntax(source);
    source.Reply(" ");
    source.Reply("This command generates a Internet Movie Database (imdb)\n"
		 "search link for the requested text");
    return true;
  }
};

class CommandCWiki : public Command
{
public:
  CommandCWiki(module *m):Command(m, "!WIKI", C_CHANNEL, 1,1)
  {
    this->SetDesc("Generate a wikipedia search url");
    this->SetSyntax("\37message\37");
  }
  void Run(CommandSource &source, const Flux::vector &params)
  {
    Flux::string str = search(params[0], this->name);
    source.c->SendMessage(str);
    Log() << "Channel Wikipedia Search from " << source.u->nick << " \"" << str << "\"";
  }
  bool OnHelp(CommandSource &source, const Flux::string &nill)
  {
    this->SendSyntax(source);
    source.Reply(" ");
    source.Reply("This command generates a Wikipedia search link\n"
		 "for the requested text");
    return true;
  }
};

class CommandCMusic : public Command
{
public:
  CommandCMusic(module *m):Command(m, "!MUSIC", C_CHANNEL, 1,1)
  {
    this->SetDesc("Generate a music search url");
    this->SetSyntax("\37message\37");
  }
  void Run(CommandSource &source, const Flux::vector &params)
  {
    Flux::string str = search(params[0], this->name);
    source.c->SendMessage(str);
    Log() << "Channel music Search from " << source.u->nick << " \"" << str << "\"";
  }
  bool OnHelp(CommandSource &source, const Flux::string &nill)
  {
    this->SendSyntax(source);
    source.Reply(" ");
    source.Reply("This command generates a music search link\n"
		 "for the requested text");
    return true;
  }
};

class CommandClmgtfy : public Command
{
public:
  CommandClmgtfy(module *m):Command(m, "!LMGTFY", C_CHANNEL, 1,1)
  {
    this->SetDesc("Generate a Let me google that for you search url");
    this->SetSyntax("\37message\37");
  }
  void Run(CommandSource &source, const Flux::vector &params)
  {
    Flux::string str = search(params[0], this->name);
    source.c->SendMessage(str);
    Log() << "Channel music Search from " << source.u->nick << " \"" << str << "\"";
  }
  bool OnHelp(CommandSource &source, const Flux::string &nill)
  {
    this->SendSyntax(source);
    source.Reply(" ");
    source.Reply("This command generates a Let me google that for you\n"
    "search link for the requested text");
    return true;
  }
};

/**
 * \fn class searcher(bool a):module("Searcher Handler", a, PRIORITY_DONTCARE){ this->SetDesc("Search Module used to search for stuff"); }
 * \brief Returns search links for different sites.
 * Searches for the search command, then uses the \a search function from \a flux_net_irc to return search links.
 */
class searcher:public module
{
  CommandCGoogle google;
  CommandCYoutube yt;
  CommandCPirateBay tpb;
  CommandCDefine d;
  CommandCUrban urban;
  CommandCMovie movie;
  CommandCWiki wiki;
  CommandCMusic music;
  CommandClmgtfy lmgtfy;
public:
  searcher(const Flux::string &Name):module(Name), google(this), yt(this), tpb(this), d(this), urban(this),
  movie(this), wiki(this), music(this), lmgtfy(this)
  {
    this->SetAuthor("Lordofsraam");
    this->SetVersion(VERSION);
  }
};
/**
 * @}
 */
MODULE_HOOK(searcher)