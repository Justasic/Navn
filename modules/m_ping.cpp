#include "flux_net_irc.hpp"

int pings = 0;
bool timeout = false;
class PingTimer:public Timer
{
public:
  PingTimer():Timer(30, time(NULL), true) {}
  void Tick(time_t){
    Send->raw("PING :%i\n", time(NULL));
    if(!timeout)
      timeout = true;
    if(timeout){
      ++pings;
      if(pings >= 3)
	restart("Ping-Timeout: 121 seconds");
    }
  }
};
class Ping_pong:public module
{
  PingTimer pingtimer;
public:
  Ping_pong():module("Ping", PRIORITY_FIRST){
    Implementation i[] = { I_OnNumeric, I_OnCommand };
    ModuleHandler::Attach(i, this, sizeof(i) / sizeof(Implementation));
    this->SetAuthor("Justasic");
    this->SetVersion(VERSION);
  }
  
  void OnCommand(const Flux::string &command, const std::vector<Flux::string> &params){
    if(command == "PONG")
    {
     Flux::string ts = params[0];
     int timestamp = atoi(ts.c_str());
     int lag = time(NULL)-timestamp;
     timeout = false;
     pings = 0;
     if(protocoldebug)
        printf("%i sec lag (%i - %i)\n", lag, timestamp, (int)time(NULL));
    }
  }
  
  void OnNumeric(int i)
  {
   if((i == 451)){
     Send->command->user(Config->Ident, Config->Realname);
     Send->command->nick(Config->BotNick);
   }
  }
};
MODULE_HOOK(Ping_pong)