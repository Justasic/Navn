#include "flux_net_irc.hpp"

int pings = 0;
bool timeout = false;
class PingTimer:public Timer
{
public:
  PingTimer():Timer(30, time(NULL), true) {}
  void Tick(time_t){
    Send->raw("PING :%i\n", time(NULL));
    /*if(!timeout)
      timeout = true;
    if(timeout){
      ++pings;
      if(pings >= 3)
	restart("Ping-Timeout: 121 seconds");
    } */
  }
};
class Ping_pong:public module
{
  PingTimer pingtimer;
public:
  Ping_pong(bool a):module("Ping", a, PRIORITY_FIRST){ 
    this->SetDesc("Handles ping pong events");
    /*Implementation i[] = { I_OnPrivmsg };*/
    ModuleHandler::Attach(I_OnCommand, this);
  }
  void OnCommand(CommandSource &source, std::vector<Flux::string> &params){
    printf("Command!!!!!!!!\n");
    Flux::string cmd = params.empty()?"":params[0];
    
    if(source.command == "PONG")
    {
     Flux::string ts = params[0];
     int timestamp = atoi(ts.c_str());
     int lag = time(NULL)-timestamp;
     timeout = false;
     pings = 0;
     if(protocoldebug)
        printf("%i sec lag (%i - %i)\n", lag, timestamp, (int)time(NULL));
    }
    if(source.command == "451"){
     Send->command->user(Config->Ident, Config->Realname);
     Send->command->nick(Config->BotNick); 
    }
  }
};
MODULE_HOOK(Ping_pong)