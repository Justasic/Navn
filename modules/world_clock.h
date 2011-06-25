#ifndef WORLD_CLOCK_H
#define WORLD_CLOCK_H
#include "../includes.h"
#include "../flux_net_irc.hpp"
#define MST (-6)
#define UTC (0)
#define CCT (+8)
#define EST (-4)
#define PST (-7)
#define AUS (+10)
using namespace std;
using namespace flux_net_irc;
void world_clock(Socket &sock, irc_string *reply){
//If it looks for !time in a server rply (aka if anyone in the channel
//says it) and if it finds it, it tells the time.
  if (reply->said("!time") && in_channel){
    /*WORLD CLOCK
    Since this is all C++ and not IRC or socket stuff, I won't bother
    trying to explain it. (Mainly because if youre trying something
    like this you should already have a decent grasp on C++, and partly
    because I'm lazy)
    */
    time_t rawtime;
    tm * ptm;
    time ( &rawtime );
    ptm = gmtime ( &rawtime );
    int minutes = (ptm->tm_min);
    int seconds = (ptm->tm_sec);
    sock << privmsg(chan, "Current time around the World:");
    sock << privmsg(chan, "GMT == %2d:%02d:%02d", (ptm->tm_hour+UTC)%24, minutes, seconds);
    sock << privmsg(chan, "New York (USA) == %2d:%02d:%02d", (ptm->tm_hour+EST)%24, minutes, seconds);
    sock << privmsg(chan, "California (USA) == %2d:%02d:%02d", (ptm->tm_hour+PST)%24, minutes, seconds);
    sock << privmsg(chan, "Beijing (China) == %2d:%02d:%02d", (ptm->tm_hour+CCT)%24, minutes, seconds);
    sock << privmsg(chan, "Sydney (Australia) == %2d:%02d:%02d", (ptm->tm_hour+AUS)%24, minutes, seconds);	
    char buf[100];
    ptm = localtime(&rawtime);
    strftime(buf,100,"Navn's Time: %Z %c",ptm);
    sock << privmsg(chan, buf);
    log("%s requested !time command in %s", unick.c_str(), chan.c_str());
  }

}
#endif
