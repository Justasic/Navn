/* Navn IRC bot -- Player vs. Player game
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */

#include "modules.h"
#include "Player.h"

namespace BattleField
{
  std::map<Flux::string, Player> Players;

  bool PlayerInField(Flux::string PlayerName) { if (BattleField::Players.find(PlayerName) != BattleField::Players.end()) return true; else return false; }

  void AddPlayer(Flux::string PlayerName)
  {
    Player p(PlayerName);
    Players[PlayerName] = p;
  }

  int NumberOfPlayers() { return Players.size(); }
}

class Join : public Command
{
public:
  Join(Module *m):Command(m, "joinPvP", C_PRIVATE)
  {
    this->SetDesc("Say "+Config->FantasyPrefix+"joinPvP to join the PvP game.");
  }
  void Run(CommandSource &Source, const std::vector<Flux::string> &params)
  {
    if (!BattleField::PlayerInField(Source.u->nick))
    {
      BattleField::AddPlayer(Source.u->nick);
      Source.Reply("Welcome to the PvP game. Watch your back.");
    }
    else { Source.Reply("You are already in the game."); }
  }
};

class Info : public Command
{
public:
  Info(Module *m):Command(m, "PvPInfo", C_CHANNEL)
  {
    this->SetDesc("Say "+Config->FantasyPrefix+"PvPInfo to get information about the game.");
  }
  void Run(CommandSource &Source, const std::vector<Flux::string> &params)
  {
    if ((params[1] == "me") ^ (params[1] == "myself") ^ (params[1] == Source.u->nick))
    {
      if (BattleField::PlayerInField(Source.u->nick))
      {
	Source.Reply("Your health: %i ; Your energy: %i" , BattleField::Players[Source.u->nick].Health(), BattleField::Players[Source.u->nick].Energy());
      }
      else { Source.Reply("Sorry, but you aren't in the game."); }
    }
    else { Source.Reply("Number of players: %i", BattleField::NumberOfPlayers()); }
  }
};

class Stab : public Command
{
public:
  Stab(Module *m):Command(m, "stab", C_CHANNEL)
  {
    this->SetDesc("Say "+Config->FantasyPrefix+"stab <victim> to stab someone! (Uses 5 energy).");
  }
  void Run(CommandSource &Source, const std::vector<Flux::string> &params)
  {
    if (BattleField::PlayerInField(params[1])) BattleField::Players[params[1]].HurtBy(5);
    Source.Reply("You just stabbed " + BattleField::Players[params[1]].Name() + "!");
  }
};

class PvPModule : public Module
{
  Join join;
  Stab stab;
  Info info;
public:
  PvPModule(const Flux::string &Name):Module(Name), join(this), stab(this), info(this)
  {
    this->SetVersion("1.0.3");
    this->SetAuthor("Lordofsraam");
    this->SetPriority(PRIORITY_LAST);
    Implementation i[] = { I_OnPrivmsg, I_OnNotice };
    ModuleHandler::Attach(i, this, sizeof(i)/sizeof(Implementation));
  }
  void OnPrivmsg(User *u, const std::vector<Flux::string> &params)
  {
    Flux::string msg;
    for(unsigned i=0; i < params.size(); ++i)
      msg += params[i] +' ';
  }
  void OnNotice(User *u, const std::vector<Flux::string> &params)
  {
    Flux::string msg;
    for(unsigned i=0; i < params.size(); ++i)
      msg += params[i] +' ';
  }
};

MODULE_HOOK(PvPModule)
