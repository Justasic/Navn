#include "flux_net_irc.hpp"

class PingTimer:public Timer
{
public:
  int pings;
  bool timeout;
  PingTimer():Timer(30, time(NULL), true), pings(0), timeout(false) { }
  void Tick(time_t){
    send_cmd("PING :%i\n", time(NULL));
    if(!timeout)
      timeout = true;
    if(timeout)
    {
      if(++pings >= 3)
	sock->ThrowException("Ping timeout: 120 seconds");
    }
  }
};
class Ping_pong:public module
{
  PingTimer pingtimer;
public:
  Ping_pong(const Flux::string &Name):module(Name)
  {
    Implementation i[] = { I_OnNumeric, I_OnCommand, I_OnPing };
    ModuleHandler::Attach(i, this, sizeof(i) / sizeof(Implementation));
    this->SetAuthor("Justasic");
    this->SetVersion(VERSION);
    this->SetPriority(PRIORITY_FIRST);
  }
  void OnCommand(const Flux::string &command, const std::vector<Flux::string> &params)
  {
    if(command.equals_cs("PONG"))
    {
     Flux::string ts = params[0];
     int lag = time(NULL)-(int)ts;
     pingtimer.timeout = false;
     pingtimer.pings = 0;
     if(protocoldebug)
        Log(LOG_RAWIO) << lag << " sec lag (" << ts << " - " << time(NULL) << ')';
    }
  }
  void OnPing(const Flux::string&, const std::vector<Flux::string> &params)
  {
    pingtimer.timeout = false;
    send_cmd("PONG :%s\n", params[0].c_str());
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
