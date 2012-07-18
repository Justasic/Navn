/* Navn IRC bot -- Netsplit detector
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */
#include "modules.h"

#if 0

class NetsplitModule : public Module
{
  std::map<time_t, std::map<User*, message> > SplitUsers;
public:
  NetsplitModule(const Flux::string &Name):Module(Name)
  {
    this->SetAuthor("Justasic");
    this->SetVersion(VERSION);
    Implementation i[] = { I_OnQuit, I_OnGarbageCollect };
    ModuleHandler::Attach(i, this, sizeof(i) / sizeof(Implementation));
  }

  void OnQuit(User *u, const Flux::string &message)
  {
    std::map<User*, const Flux::string> quituser;
    quituser[u] = message;
    SplitUsers[time(NULL)] = quituser;
    Flux::map<User*> LostUsers;

    for(std::map<time_t, std::map<User*, message> >::iterator it = SplitUsers.begin(), it_end = SplitUsers.end(); it != it_end; ++it)
    {
      time_t splittime = it->first;
      if(splittime == time(NULL))
	return;
      else
      {
	for(std::map<User*, const Flux::string>::iterator it2 = it->second.begin(), it2_end = it->second.end(); it2 != it2_end; ++it2)
	{
	  User *u = it2->first;
	  const Flux::string msg = it2->second;

	  LostUsers[msg] = u;
	}

	Log(LOG_TERMINAL) << "Netsplt, lost " << it->second.size() << " users.";
      }
    }
  }

  void OnGarbageCollect()
  {
    Log(LOG_DEBUG) << "Clearing split " << SplitUsers.size() << " users";
    SplitUsers.clear();
  }
};

MODULE_HOOK(NetsplitModule)
#endif