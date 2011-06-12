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
    sock << "PONG "+re.substr(6,-1);
    servername = re.substr(6,-1);
  }
  if(reply->said("ERROR :Closing link:")){
   throw SocketException(raw);
  }
  /* This is still a very experemental Ping timer to give ping timeouts when something happens
  if (time(NULL) > endwait){
   string pingtimestamp = stringify(time(NULL));
   sock << strip("PING "+ strip(servername));
   endwait = time(NULL) + pingwait;
   if(reply->said(" PONG ")){
    //ping-timeout function still needs to be programmed..
    cout << "woop woop we're still connected :P" << endl;
   }
  }
  if(reply->said("!endwait")){
	cout << "Endwait: " << endwait << endl;
	cout << "Time(): " << time(NULL) << endl;
	cout << "Remaining: " << endwait - time(NULL) << endl;
  }
*/
}
#endif
