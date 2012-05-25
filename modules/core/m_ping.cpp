/* Navn IRC bot -- Ping module
 * 
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */

#include "modules.h"

class PingTimeoutTimer;
class PingTimer : public Timer
{
public:
  PingTimeoutTimer *ptt;
  PingTimer():Timer(30, time(NULL), true), ptt(NULL) { }
  void Tick(time_t);
};

class PingTimeoutTimer : public Timer
{
  PingTimer *pt;
  time_t wait;
public:
  PingTimeoutTimer(time_t w, PingTimer *pt2):Timer(w, time(NULL), false), pt(pt2), wait(w)
  {
    if(this->pt && this->pt->ptt)
      return;
  }
  
  ~PingTimeoutTimer() { this->pt->ptt = NULL; }
  
  void Tick(time_t)
  {
    sock->ThrowException(printfify("Ping Timeout: %u seconds", this->wait));
  }
};

void PingTimer::Tick(time_t)
{
  send_cmd("PING :%i\n", time(NULL));
  this->ptt = new PingTimeoutTimer(Config->PingTimeoutTime, this);
}

class Ping_pong : public module
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
     Log(LOG_RAWIO) << lag << " sec lag (" << ts << " - " << time(NULL) << ')';
     
     if(pingtimer.ptt)
	delete pingtimer.ptt;
     pingtimer.ptt = NULL;
  }
  
  void OnPing(const Flux::vector &params)
  {
    if(pingtimer.ptt)
      delete pingtimer.ptt;
    
    pingtimer.ptt = NULL;
    send_cmd("PONG :%s\n", params[0].c_str());
  }
  
  void OnConnectionError(const Flux::string &buffer)
  {
    throw CoreException(buffer.c_str());
  }
  
  void OnNumeric(int i, const std::vector<Flux::string> &params)
  {
   if((i == 451))
     ircproto->introduce_client(Config->BotNick, Config->Ident, Config->Realname);
  }
};

MODULE_HOOK(Ping_pong)
