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
     int lag = time(NULL)-(int)ts;
     timeout = false;
     pings = 0;
     if(protocoldebug)
        printf("%i sec lag (%i - %i)\n", lag, (int)ts, (int)time(NULL));
    }
    if(command == "PING")
    {
      timeout = false;
      send_cmd("PONG :%s\n", params[1].c_str());
    }
  }
  void OnConnectionError(const Flux::string &buffer)
  {
   throw CoreException(buffer.c_str()); 
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