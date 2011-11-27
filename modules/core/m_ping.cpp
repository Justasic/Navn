#include "flux_net_irc.hpp"

class PingTimer:public Timer
{
public:
  int pings;
  PingTimer():Timer(30, time(NULL), true), pings(0) { }
  void Tick(time_t){
    send_cmd("PING :%i\n", time(NULL));
    if(++pings >= 3)
      sock->ThrowException("Ping timeout: 120 seconds");
  }
};
class Ping_pong:public module
{
  PingTimer pingtimer;
public:
  Ping_pong(const Flux::string &Name):module(Name)
  {
    Implementation i[] = { I_OnNumeric, I_OnPong, I_OnPing };
    ModuleHandler::Attach(i, this, sizeof(i) / sizeof(Implementation));
    this->SetAuthor("Justasic");
    this->SetVersion(VERSION);
    this->SetPriority(PRIORITY_FIRST);
  }
  void OnPong(const std::vector<Flux::string> &params)
  {
     Flux::string ts = params[1];
     int lag = time(NULL)-(int)ts;
     pingtimer.pings = 0;
     if(protocoldebug)
        Log(LOG_RAWIO) << lag << " sec lag (" << ts << " - " << time(NULL) << ')';
  }
  void OnPing(const std::vector<Flux::string> &params)
  {
    pingtimer.pings = 0;
    send_cmd("PONG :%s\n", params[0].c_str());
  }
  void OnConnectionError(const Flux::string &buffer)
  {
   throw CoreException(buffer.c_str());
  }
  void OnNumeric(int i)
  {
   if((i == 451)){
     ircproto->user(Config->Ident, Config->Realname);
     ircproto->nick(Config->BotNick);
   }
  }
};
MODULE_HOOK(Ping_pong)
