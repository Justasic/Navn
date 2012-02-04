#ifndef SEARCHER_H
#define SEARCHER_H
#include "../includes.h"
#include "../flux_net_irc.hpp"
using namespace std;
using namespace flux_net_irc;

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
 * \fn void searcher(Socket &sock, irc_string *reply, Flux::string rply)
 * \brief Returns search links for different sites.
 * Searches for the search command, then uses the \a search function from \a flux_net_irc to return search links.
 */

void searcher(Socket &sock, irc_string *reply, Flux::string rply){

  if(reply->said("!google") && in_channel){ // if !google is said in the channel
		sock << privmsg(chan, search(rply, "!google").c_str());
		log("Channel Google Search from %s \"%s\"", reply->usernick.c_str(), search(rply, "!google").c_str());
      }
      if(reply->said("!youtube") && in_channel){ // if !youtube is said in the channel
		sock << privmsg(chan, search(rply, "!youtube").c_str());
		log("Channel youtube Search from %s \"%s\"", reply->usernick.c_str(), search(rply, "!youtube").c_str());
      }
      if(reply->said("!tpb") && in_channel){ // if !tpb xor !thepiratebay is said in the channel
		sock << privmsg(chan, search(rply, "!tpb").c_str());
		log("Channel tpb Search from %s \"%s\"", reply->usernick.c_str(), search(rply, "!tpb").c_str());
      }
      if(reply->said("!define") && in_channel){
		sock << privmsg(chan, search(rply, "!define").c_str());
		log("Channel define Search from %s \"%s\"", reply->usernick.c_str(), search(rply, "!define").c_str());
      }
      if(reply->said("!urban") && in_channel){ 
		sock << privmsg(chan, search(rply, "!urban").c_str());
		log("Channel urban Search from %s \"%s\"", reply->usernick.c_str(), search(rply, "!urban").c_str());
      }
      if(reply->said("!movie") && in_channel){ 
		sock << privmsg(chan, search(rply, "!movie").c_str());
		log("Channel movie Search from %s \"%s\"", reply->usernick.c_str(), search(rply, "!movie").c_str());
      }
      if(reply->said("!wiki") && in_channel){ 
		sock << privmsg(chan, search(rply, "!wiki").c_str());
		log("Channel wiki Search from %s \"%s\"", reply->usernick.c_str(), search(rply, "!wiki").c_str());
      }
      if(reply->said("!music") && in_channel){ 
		sock << privmsg(chan, search(rply, "!music").c_str());
		log("Channel music Search from %s \"%s\"", reply->usernick.c_str(), search(rply, "!music").c_str());
      }
  
}
/**
 * @}
 */
#endif