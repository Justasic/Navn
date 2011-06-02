#ifndef PING_PONG_H
#define PING_PONG_H
#include "../includes.h"
#include "../flux_net_irc.hpp"
using namespace std;
using namespace flux_net_irc;
#define pingwait 30
time_t endwait = time(NULL) + pingwait;
bool ppto = false;
string servername;
void ping_pong(Socket &sock, irc_string *reply, string re){
  if (reply->said("PING :")){
    sock << "PONG :"+re.substr(6,-1);
    endwait = time(NULL) + pingwait;
    servername = re.substr(6,-1);
  }

  if (time(NULL) > endwait){
   string pingtimestamp = stringify(time(NULL));
   sock << strip("PING :"+ strip(servername));
   endwait = time(NULL) + pingwait;
   /*if(reply->said(" PONG ")){
	ppto = false;
   }else{
	if(ppto && time(NULL) > endwait){
		cout << "Ping timeout? restarting" << endl;
		DoQuit();
		restart("Ping-Timeout");
	}
	ppto = true;
   }*/
  }
  if(reply->said("!endwait")){
	cout << "Endwait: " << endwait << endl;
	cout << "Time(): " << time(NULL) << endl;
	cout << "Remaining: " << endwait - time(NULL) << endl;
  }
}
#endif
