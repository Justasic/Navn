#ifndef SEARCHER_H
#define SEARCHER_H
#include "../includes.h"
#include "../flux_net_irc.hpp"


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
  for (unsigned i=0; i < raw_searchstring.length(); i++){
    switch(raw_searchstring.at(i)){
      case ' ':
	searchstring = searchstring+"%20";
	break;
      case '+':
	searchstring = searchstring+"%2B";
	break;
      case '$':
	searchstring = searchstring+"%24";
	break;
      case '&':
	searchstring = searchstring+"%26";
	break;
      case ',':
	searchstring = searchstring+"%2C";
	break;
      case '/':
	searchstring = searchstring+"%2F";
	break;
      case ':':
	searchstring = searchstring+"%3A";
	break;
      case ';':
	searchstring = searchstring+"%3B";
	break;
      case '=':
	searchstring = searchstring+"%3D";
	break;
      case '?':
	searchstring = searchstring+"%3F";
	break;
      case '@':
	searchstring = searchstring+"%40";
	break;
      case '#':
	searchstring = searchstring+"%23";
	break;
      case '>':
	searchstring = searchstring+"%3E";
	break;
      case '<':
	searchstring = searchstring+"%3C";
	break;
      case '%':
	searchstring = searchstring+"%25";
	break;
      case '{':
	searchstring = searchstring+"%7B";
	break;
      case '}':
	searchstring = searchstring+"%7D";
	break;
      case '|':
	searchstring = searchstring+"%7C";
	break;
      case '\\':
	searchstring = searchstring+"%5C";
	break;
      case '^':
	searchstring = searchstring+"%5E";
	break;
      case '~':
	searchstring = searchstring+"%7E";
	break;
      case '[':
	searchstring = searchstring+"%5B";
	break;
      case ']':
	searchstring = searchstring+"%5D";
	break;
      case '`':
	searchstring = searchstring+"%60";
	break;
      case '*':
	searchstring = searchstring+"%2A";
	break;
      case '(':
	searchstring = searchstring+"%28";
	break;
      case ')':
	searchstring = searchstring+"%29";
	break;
      case '"':
	searchstring = searchstring+"%22";
	break;
      case '\'':
	searchstring = searchstring+"%27";
	break;
      case '.':
	searchstring = searchstring+"%2E";
	break;
      default:
	searchstring = searchstring+raw_searchstring.at(i);
    }
  }
  if (searchstring.empty()){
    return "Empty search Flux::string.";
  }else{
    if(command == "!google"){
      return "http://www.google.com/search?q="+searchstring;
    }else if(command == "!youtube"){
      return "http://www.youtube.com/results?search_query="+searchstring;
    }else if(command == "!tpb"){
      return "http://thepiratebay.org/search/"+searchstring;
    }else if(command == "!define"){
      return "http://dictionary.reference.com/browse/"+searchstring;
    }else if(command == "!urban"){
      return "http://www.urbandictionary.com/define.php?term="+searchstring;
    }else if(command == "!movie"){
      return "www.letmewatchthis.ch/index.php?search_keywords="+searchstring;
    }else{
      return "http://www.google.com/search?q="+searchstring;
    }
  }
}
/**
 * \fn class searcher(bool a):module("Searcher Handler", a, PRIORITY_DONTCARE){ this->SetDesc("Search Module used to search for stuff"); }
 * \brief Returns search links for different sites.
 * Searches for the search command, then uses the \a search function from \a flux_net_irc to return search links.
 */
class searcher:module{
public:
  searcher(bool a):module("Searcher Handler", a, PRIORITY_DONTCARE){ this->SetDesc("Search Module used to search for stuff"); }
ModuleReturn run(SendMessage *Send, Flux::string rply, irc_string *reply){

  if(reply->said("!google") && in_channel){ // if !google is said in the channel
		Send->privmsg(chan, search(rply, "!google"));
		log("Channel Google Search from %s \"%s\"", reply->usernick.c_str(), search(rply, "!google").c_str());
      }
      if(reply->said("!youtube") && in_channel){ // if !youtube is said in the channel
		Send->privmsg(chan, search(rply, "!youtube"));
		log("Channel youtube Search from %s \"%s\"", reply->usernick.c_str(), search(rply, "!youtube").c_str());
      }
      if(reply->said("!tpb") && in_channel){ // if !tpb xor !thepiratebay is said in the channel
		Send->privmsg(chan, search(rply, "!tpb"));
		log("Channel tpb Search from %s \"%s\"", reply->usernick.c_str(), search(rply, "!tpb").c_str());
      }
      if(reply->said("!define") && in_channel){
		Send->privmsg(chan, search(rply, "!define"));
		log("Channel define Search from %s \"%s\"", reply->usernick.c_str(), search(rply, "!define").c_str());
      }
      if(reply->said("!urban") && in_channel){ 
		Send->privmsg(chan, search(rply, "!urban"));
		log("Channel urban Search from %s \"%s\"", reply->usernick.c_str(), search(rply, "!urban").c_str());
      }
      if(reply->said("!movie") && in_channel){ 
		Send->privmsg(chan, search(rply, "!movie"));
		log("Channel movie Search from %s \"%s\"", reply->usernick.c_str(), search(rply, "!movie").c_str());
      }
      if(reply->said("!wiki") && in_channel){ 
		Send->privmsg(chan, search(rply, "!wiki"));
		log("Channel wiki Search from %s \"%s\"", reply->usernick.c_str(), search(rply, "!wiki").c_str());
      }
      if(reply->said("!music") && in_channel){ 
		Send->privmsg(chan, search(rply, "!music"));
		log("Channel music Search from %s \"%s\"", reply->usernick.c_str(), search(rply, "!music").c_str());
      }
  return MOD_RUN;
}
};
/**
 * @}
 */
#endif