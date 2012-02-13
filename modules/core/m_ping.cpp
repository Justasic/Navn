#include "flux_net_irc.hpp"

class PingTimeoutTimer;
class PingTimer : public Timer
{
public:
  PingTimeoutTimer *ptt;
  PingTimer():Timer(30, time(NULL), true) { }
  void Tick(time_t);
};

class PingTimeoutTimer : public Timer
{
  PingTimer *pt;
  time_t wait;
public:
  PingTimeoutTimer(time_t w, PingTimer *pt2):Timer(w, time(NULL), false), wait(w) { this->pt = pt2; }
  ~PingTimeoutTimer() { this->pt->ptt = NULL; }
  void Tick(time_t)
  {
    sock->ThrowException(printfify("Ping Timeout: %i seconds", static_cast<int>(this->wait)));
  }
};

void PingTimer::Tick(time_t)
{
  send_cmd("PING :%i\n", time(NULL));
  this->ptt = new PingTimeoutTimer(Config->PingTimeoutTime, this);
}

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
     int lag = time(NULL)-static_cast<int>(ts);
     if(pingtimer.ptt)
	delete pingtimer.ptt;
     if(protocoldebug)
        Log(LOG_RAWIO) << lag << " sec lag (" << ts << " - " << time(NULL) << ')';
  }
  void OnPing(const Flux::vector &params)
  {
    if(pingtimer.ptt)
      delete pingtimer.ptt;
    send_cmd("PONG :%s\n", params[0].c_str());
  }
  void OnConnectionError(const Flux::string &buffer)
  {
    throw CoreException(buffer.c_str());
  }
  void OnNumeric(int i, const std::vector<Flux::string> &params)
  {
   if((i == 451)){
     ircproto->user(Config->Ident, Config->Realname);
     ircproto->nick(Config->BotNick);
   }
  }
};
MODULE_HOOK(Ping_pong)
