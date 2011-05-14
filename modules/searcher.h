#ifndef SEARCHER_H
#define SEARCHER_H
#include "../includes.h"
#include "../flux_net_irc.hpp"
using namespace std;
using namespace flux_net_irc;
void searcher(Socket &sock, irc_string *reply, string rply){

  if(reply->said("!google") && in_channel){ // if !google is said in the channel
    sock << privmsg(chan, search(rply, "!google"));
    log("Channel Google Search from %s \"%s\"", reply->usernick.c_str(), search(rply, "!google").c_str());
  }
  if(reply->said(pmsggoogle)){ //if the message was sent to the bot as a private message.
    sock << notice(reply->usernick, search(rply, "google"));
    log("Private message Google Search from %s \"%s\"", reply->usernick.c_str(), search(rply, "google").c_str());
  }
  if(reply->said("!youtube") && in_channel){ // if !youtube is said in the channel
    sock << privmsg(chan, search(rply, "!youtube"));
    log("Channel youtube Search from %s \"%s\"", reply->usernick.c_str(), search(rply, "!youtube").c_str());
  }
  if(reply->said(pmsgyoutube)){ //if the message was sent to the bot as a private message.
    sock << notice(reply->usernick, search(rply, "youtube"));
    log("Private message Google Search from %s \"%s\"", reply->usernick.c_str(), search(rply, "youtube").c_str());
  }
  if(reply->said("!tpb") && in_channel){ // if !tpb xor !thepiratebay is said in the channel
    sock << privmsg(chan, search(rply, "!tpb"));
    log("Channel tpb Search from %s \"%s\"", reply->usernick.c_str(), search(rply, "!tpb").c_str());
  }
  if(reply->said("!define") && in_channel){
    sock << privmsg(chan, search(rply, "!define"));
    log("Channel define Search from %s \"%s\"", reply->usernick.c_str(), search(rply, "!define").c_str());
  }
  if(reply->said("!urban") && in_channel){ 
    sock << privmsg(chan, search(rply, "!urban"));
    log("Channel urban Search from %s \"%s\"", reply->usernick.c_str(), search(rply, "!urban").c_str());
  }
  if(reply->said("!movie") && in_channel){ 
    sock << privmsg(chan, search(rply, "!movie"));
    log("Channel movie Search from %s \"%s\"", reply->usernick.c_str(), search(rply, "!movie").c_str());
  }
  if(reply->said("!image") && in_channel){
    sock << chanmsg(search(rply, "!image"));
    log("Channel image Search from %s \"%s\"", reply->usernick.c_str(), search(rply, "!image").c_str());
  }
  
}
#endif