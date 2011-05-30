#ifndef FS
#define FS
#include "../includes.h"
#include "../flux_net_irc.hpp"
using namespace std;
using namespace flux_net_irc;
void Flux_Services(Socket &sock, irc_string *reply){

	if(reply->said("!part ")){ 
		if(unick == owner_nick){
			sock << privmsg(chan, "im out niggaz!");
			sock << part(reply->params(1), "I'm leaving this dump.");
			log("%s used Flux_S3rvices part %s", unick.c_str(), chan.c_str());
		}else{
			sock << notice(unick, access_denied);
		}
	}
	if(reply->said("!botadd")){
		if(unick == owner_nick){
			string bnick = reply->params(1);
			string buser = reply->params(2);
			string bhost = reply->params(3);
			string breal = reply->params(4);
			sock << privmsg("BotServ", "add %s %s %s %s", bnick.c_str(), buser.c_str(), bhost.c_str(), breal.c_str());
			log("%s used Flux_S3rvices to make bot \"%s!%s@%s :%s\" %s", unick.c_str(), bnick.c_str(), buser.c_str(), bhost.c_str(), breal.c_str(), chan.c_str());
		}else{
		sock << notice(unick, access_denied);
		}
	}/*
	if(reply->said("!umode")){
		if(unick == owner_nick){
			string dest = reply->params(1);
			cout << "final: "<<msg << endl;
			sock << samode(dest, "%s", msg.c_str());
		}else{
			sock << notice(unick, access_denied);
		}	
	}*/
}
#endif
