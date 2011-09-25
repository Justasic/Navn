/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
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
Flux::string search(Flux::string s, Flux::string command){
  Flux::string raw_searchstring;
  Flux::string searchstring;
  size_t pos = s.find(command);
  raw_searchstring = s.substr(pos+(command.size())+1);
  searchstring = urlify(raw_searchstring);
  if (searchstring.empty())
    return "Empty searchstring.";
  else{
    if(command == "!google")
      return "http://www.google.com/search?q="+searchstring;
    else if(command == "!youtube")
      return "http://www.youtube.com/results?search_query="+searchstring;
    else if(command == "!tpb")
      return "http://thepiratebay.org/search/"+searchstring;
    else if(command == "!define")
      return "http://dictionary.reference.com/browse/"+searchstring;
    else if(command == "!urban")
      return "http://www.urbandictionary.com/define.php?term="+searchstring;
    else if(command == "!movie")
      return "www.letmewatchthis.ch/index.php?search_keywords="+searchstring;
    else
      return "http://www.google.com/search?q="+searchstring;
  }
}
/**
 * \fn class searcher(bool a):module("Searcher Handler", a, PRIORITY_DONTCARE){ this->SetDesc("Search Module used to search for stuff"); }
 * \brief Returns search links for different sites.
 * Searches for the search command, then uses the \a search function from \a flux_net_irc to return search links.
 */
class searcher:public module
{
public:
  searcher():module("Searcher Handler", PRIORITY_DONTCARE){
    ModuleHandler::Attach(I_OnPrivmsg, this);
    this->SetAuthor("Lordofsraam");
  }
  void OnPrivmsg(User *u, Channel *c, const std::vector<Flux::string> &params){
  Flux::string cmd = params.empty()?"":params[0];
  Flux::string msg;
    for(unsigned i=0; i < params.size(); ++i){
      msg += params[i];
      msg.AddSpace();
    }

    if(cmd.equals_ci("!google")){
	      // if !google is said in the channel
	      if(params.size() < 2){
		u->SendMessage("Syntax: \2!google \37message\15");
		return;
	      }
	      c->SendMessage(search(msg, "!google"));
	      log(LOG_NORMAL, "Channel Google Search from %s \"%s\"", u->nick.c_str(), search(msg, "!google").c_str());
    }
    if(cmd.equals_ci("!youtube")){ // if !youtube is said in the channel
	      if(params.size() < 2){
		u->SendMessage("Syntax: \2!youtube \37message\15");
		return;
	      }
	      c->SendMessage(search(msg, "!youtube"));
	      log(LOG_NORMAL, "Channel youtube Search from %s \"%s\"", u->nick.c_str(), search(msg, "!youtube").c_str());
    }
    if(cmd.equals_ci("!tpb")){ // if !tpb xor !thepiratebay is said in the channel
	      if(params.size() < 2){
		u->SendMessage("Syntax: \2!tpb \37message\15");
		return;
	      }
	      c->SendMessage(search(msg, "!tpb"));
	      log(LOG_NORMAL, "Channel tpb Search from %s \"%s\"", u->nick.c_str(), search(msg, "!tpb").c_str());
    }
    if(cmd.equals_ci("!define")){
	      if(params.size() < 2){
		u->SendMessage("Syntax: \2!define \37message\15");
		return;
	      }
	      c->SendMessage(search(msg, "!define"));
	      log(LOG_NORMAL, "Channel define Search from %s \"%s\"", u->nick.c_str(), search(msg, "!define").c_str());
    }
    if(cmd.equals_ci("!urban")){ 
	      if(params.size() < 2){
		u->SendMessage("Syntax: \2!urban \37message\15");
		return;
	      }
	      c->SendMessage(search(msg, "!urban"));
	      log(LOG_NORMAL, "Channel urban Search from %s \"%s\"", u->nick.c_str(), search(msg, "!urban").c_str());
    }
    if(cmd.equals_ci("!movie")){ 
	      if(params.size() < 2){
		u->SendMessage("Syntax: \2!movie \37message\15");
		return;
	      }
	      c->SendMessage(search(msg, "!movie"));
	      log(LOG_NORMAL, "Channel movie Search from %s \"%s\"", u->nick.c_str(), search(msg, "!movie").c_str());
    }
    if(cmd.equals_ci("!wiki")){ 
	      if(params.size() < 2){
		u->SendMessage("Syntax: \2!wiki \37message\15");
		return;
	      }
	      c->SendMessage(search(msg, "!wiki"));
	      log(LOG_NORMAL, "Channel wiki Search from %s \"%s\"", u->nick.c_str(), search(msg, "!wiki").c_str());
    }
    if(cmd.equals_ci("!music")){ 
	      if(params.size() < 2){
		u->SendMessage("Syntax: \2!music \37message\15");
		return;
	      }
	      c->SendMessage(search(msg, "!music"));
	      log(LOG_NORMAL, "Channel music Search from %s \"%s\"", u->nick.c_str(), search(msg, "!music").c_str());
    }
  }
};
/**
 * @}
 */
MODULE_HOOK(searcher)