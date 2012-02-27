/* Riista IRC bot -- main game file
 * 
 * (C) 2011-2012 Flux-Net
 * Contact us at Dev@Flux-Net.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */

#ifndef RIISTA_H
#define RIISTA_H
#include "includes.h"
#include "flux_net_irc.hpp"
//#include "monster.h"
//#include "player.h"
using namespace std;
using namespace flux_net_irc;

//const Flux::string password = make_pass();
const Flux::string start_game = "!start_game "+password;

std::map<Flux::string, Player*> players;
std::vector<Flux::string> playersV;

bool in_player_list(Flux::string n){
  bool t; 
  if(find(playersV.begin(), playersV.end(), n)!=playersV.end()){
    t = true;
  }
  else{
    t = false;
  }
  return t;  
}

void delete_from_playersV(Flux::string n){
  for (unsigned i = 0; i < playersV.size(); i++){
    if (playersV[i] == n){
      playersV.erase(playersV.begin()+i);
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////

// class monster {
// public :
//   int maxHp;
//   int hp;
//   int STR;
//   int END;
//   int DEF;
//   int AGI;
//   int INT;
//   int LUK;
//   
//   int atkMin;
//   int atkMax;
//   
//   int evasion;
//   int critical;
//   
//   int paralyzed_count;
//   bool paralyzed;
//   
//   Flux::string name;
//   Flux::string last_attacker;
//   Flux::string last_attacker2;
//   Flux::string last_attacker3;
//   
//   //constructor
//   monster(Flux::string n){
//     name = n;
//     STR = 15;
//     END = 10;
//     DEF = 7;
//     AGI = 4;
//     INT = 7;
//     LUK = 5;
//     
//     paralyzed = false;
//     paralyzed_count = 0;
//   }
//   
//   void set_stats() {
//     maxHp = END * 100;
//     hp = maxHp;
//     
//     atkMin = STR * 4;
//     atkMax = STR * 6;
//     
//     evasion = AGI * 2.5;
//     critical = LUK;
//   }
//   
//   bool alive(){
//     if (hp <= 0){
//       return false;
//     }else{return true;}
//   }
//   
//   Flux::string get_health(){
//     stringstream health;
//     health << name << "'s HP: " << hp << "/" << maxHp;
//     Flux::string output = health.str();
//     return output;
//   }class Player {
//   public:
//     int maxHp;
//     int hp;
//     int maxMp;
//     int mp;
//     int STR;
//     int END;
//     int DEF;
//     int AGI;
//     int INT;
//     int LUK;
//     
//     int atkMin;
//     int atkMax;
//     int magMin;
//     int magMax;
//     int evasion;
//     int critical;
//     
//     
//     Flux::string name;
//     Flux::string job;
//     Flux::string stance;
//     
//     //constructor
//     Player(Flux::string user) {
//       name = user;
//       stance = "normal";
//       job = "n00b";
//       STR = 3;
//       END = 3;
//       DEF = 3;
//       AGI = 3;
//       INT = 3;
//       LUK = 3;
//     }
//     Player(Flux::string user, Flux::string job_name) {
//       name = user;
//       if(job_name == "n00b"){
// 	job = "n00b";
// 	stance = "normal";
// 	STR = 3;
// 	END = 3;
// 	DEF = 3;
// 	AGI = 3;
// 	INT = 3;
// 	LUK = 3;
//       }
//       if(job_name == "mage"){
// 	job = "Mage";
// 	stance = "normal";
// 	STR = 4;
// 	END = 5;
// 	DEF = 6;
// 	AGI = 6;
// 	INT = 9;
// 	LUK = 6;
//       }
//       if(job_name == "rogue"){
// 	job = "Rogue";
// 	stance = "normal";
// 	STR = 5;
// 	END = 6;
// 	DEF = 5;
// 	AGI = 8;
// 	INT = 6;
// 	LUK = 7;
//       }
//       if(job_name == "fighter"){
// 	job = "Fighter";
// 	stance = "normal";
// 	STR = 8;
// 	END = 7;
// 	DEF = 7;
// 	AGI = 5;
// 	INT = 4;
// 	LUK = 5;
//       }
//     }
//     
//     void set_stats() {
//       maxHp = END * 100;
//       hp = maxHp;
//       maxMp = INT * 20;
//       mp = maxMp;
//       
//       atkMin = STR * 4;
//       atkMax = STR * 6;
//       magMin = INT * 4;
//       magMax = INT * 6;
//       evasion = AGI * 2.5;
//       critical = LUK;
//     }
//     
//     void set_stance_stats() {
//       atkMin = STR * 4;
//       atkMax = STR * 6;
//       magMin = INT * 4;
//       magMax = INT * 6;
//       evasion = AGI * 3;
//       critical = LUK;
//     }
//     
//     void reset_stats(){
//       if(this->job == "n00b"){
// 	STR = 3;
// 	END = 3;
// 	DEF = 3;
// 	AGI = 3;
// 	INT = 3;
// 	LUK = 3;
//       }
//       if(this->job == "Mage"){
// 	STR = 4;
// 	END = 5;
// 	DEF = 6;
// 	AGI = 6;
// 	INT = 9;
// 	LUK = 6;
//       }
//       if(this->job == "Rogue"){
// 	STR = 5;
// 	END = 6;
// 	DEF = 5;
// 	AGI = 8;
// 	INT = 6;
// 	LUK = 7;
//       }
//       if(this->job == "Fighter"){
// 	STR = 8;
// 	END = 7;
// 	DEF = 7;
// 	AGI = 5;
// 	INT = 4;
// 	LUK = 5;
//       }
//     }
//     
//     Flux::string get_health(){
//       stringstream health;
//       health << this->name << "'s HP: " << this->hp << "/" << this->maxHp << "\t" << "MP: " << this->mp << "/" << this->maxMp << ". Job: " << this->job << ". Stance: " << this->stance;
//       Flux::string output = health.str();
//       return output;
//     }
//     
//     bool alive(){
//       if (this->hp <= 0){
// 	return false;
//       }else{return true;}
//     }
//     
//     Flux::string heal() {
//       if (this->mp >= 8) {
// 	stringstream output;
// 	int heals = (.1*this->INT)*this->maxHp;
// 	this->hp += heals;
// 	this->mp -= 8;
// 	if (this->hp > this->maxHp) this->hp = this->maxHp;
// 	output << this->name << " casts Heal and recovers " << heals << " HP!";
// 	return output.str();
//       } else {
// 	return "Not enough MP";
//       }
//     }
//     
//     Flux::string concentrate(Flux::string uname){
//       this->mp += 10;
//       return uname + " concentrated and gained 10 MP!";
//     }
//     
//     Flux::string set_stance(Flux::string stance_name){
//       reset_stats();
//       if (stance_name == "defending"){
// 	this->DEF *= 4;
// 	this->STR *= 0.5;
// 	this->stance = "defending";
// 	set_stance_stats();
// 	return this->name+" is now defending.";
//       }
//       else if (stance_name == "aggressive"){
// 	this->DEF *= 0.25;
// 	this->STR *= 2;
// 	this->stance = "aggressive";
// 	set_stance_stats();
// 	return this->name+" is now in an aggressive stance.";
//       }
//       else if (stance_name == "running"){
// 	this->AGI *= 2;
// 	this->STR *= 1.3;
// 	this->DEF *= 0.7;
// 	this->stance = "running";
// 	set_stance_stats();
// 	return this->name+" is now running.";
//       }
//       else if (stance_name == "normal"){
// 	reset_stats();
// 	this->stance = "normal";
// 	set_stance_stats();
// 	return this->name+" has reset their stance.";
//       }
//       else{
// 	return "Unrecognized stance.";
//       }
//     }
//     
//   };
  
  Flux::string hurt(Flux::string uname, Flux::string attack_name){  
      last_attacker3 = last_attacker2;
      last_attacker2 = last_attacker;
      last_attacker = uname;
      stringstream output;
      if (attack_name == "slash")
      {
	int hit = randint(1,100);
	int crit = randint(1,100);
	if (hit <= this->evasion)
	{
	  output << this->name << " dodged your attack!";
	}
	else
	{
	  int dmg = randint(players[uname]->atkMin,players[uname]->atkMax) - randint(this->DEF,this->DEF*2);
	  if (dmg < 1) dmg = 1;
	  if (crit < players[uname]->critical) dmg *= 2;
	  this->hp -= dmg;
	  output << uname << " slashed " << this->name << " and did " << dmg << " damage!";
	}
	return output.str();
      }
      else if(attack_name == "cross_cut")
      {
	if(players[uname]->job == "Fighter" or players[uname]->job == "Rogue")
	{
	  if(players[uname]->mp > 4)
	  {
	    int hit1 = randint(1,100);
	    int crit1 = randint(1,100);
	    int hit2 = randint(1,100);
	    int crit2 = randint(1,100);
	    int dmg = randint(players[uname]->atkMin,players[uname]->atkMax) - randint(this->DEF,this->DEF*2);
	    dmg *= .75;
	    int dmg2 = randint(players[uname]->atkMin,players[uname]->atkMax) - randint(this->DEF,this->DEF*2);
	    dmg2 *= .75;
	    players[uname]->mp -= 5;
	    if (dmg < 1) dmg = 1;
	    if (dmg2 < 1) dmg2 = 1;
	    if (hit1 <= this->evasion) dmg = 0;
	    if (crit1 < players[uname]->critical) dmg *= 2;
	    if (hit2 <= this->evasion) dmg2 = 0;
	    if (crit2 < players[uname]->critical) dmg *= 2;
	    this->hp -= (dmg+dmg2);
	    output << uname << " slashed " << this->name << " twice and did " << dmg << " damage and " << dmg2 << " damage.";
	    return output.str();
	  }else{ return "Not enough MP."; }
	}else{ return "You need to be a Fighter or Rogue to use this attack."; }
      }
      else if(attack_name == "lucky_shot"){
	if (players[uname]->job == "Fighter" or players[uname]->job == "Rogue"){
	  if(players[uname]->mp > 4){
	    int dmg = randint(players[uname]->atkMin-randint(players[uname]->LUK,players[uname]->LUK*3),players[uname]->atkMax+randint(players[uname]->LUK,players[uname]->LUK*4)) - randint(this->DEF,this->DEF*2);
	    if (dmg < 1) dmg = 1;
	    this->hp -= dmg;
	    players[uname]->mp -= 4;
	    output << uname << " tried his luck against " << this->name << " and did " << dmg << " damage!";
	    return output.str();
	  }else{return "Not enough MP.";}
	}else{return "You need to be a Fighter or Rogue to use this attack.";}
      }
      else if (attack_name == "forceus"){
	if(players[uname]->mp > 4){
	int dmg = randint(players[uname]->magMin,players[uname]->magMax) - this->DEF;
	if (dmg < 1) dmg = 1;
	this->hp -= dmg;
	players[uname]->mp -= 8;
	output << uname << " cast Forceus on " << this->name << " and did " << dmg << " damage!";
	return output.str();
	}else{return "Not enough MP.";}
      }
  }
  
  Flux::string retaliate(){
    stringstream output;
    if (!paralyzed){
      srand(time(NULL));
      int p1;
      int p2;
      Flux::string victim;
      do{
      p1 = playersV.size();
      p2 = rand()%p1;
      victim = playersV[p2];
      }while(!players[victim]->alive());
      int hit = randint(1,100);
      if (hit <= players[victim]->evasion) {
	output << players[victim]->name << " dodged "<< this->name << "'s attack!";
      }
      else {
	int crit = randint(1,100);
	int dmg = randint(this->atkMin,this->atkMax) - randint(players[victim]->DEF,players[victim]->DEF*2);
	if (crit < this->critical) dmg *= 2;
	players[victim]->hp -= dmg;
	output << this->name << " deals " << dmg <<  " damage to " << players[victim]->name;
	if (!players[victim]->alive()){
	  output << ". " << players[victim]->name << " has died!";
	}
      }
    }else{
      output << this->name << " is paralyzed and can not move!";
      this->paralyzed_count += 1;
      if(paralyzed_count > 3){
	this->paralyzed = false;
	this->paralyzed_count = 0;
      }
    }
    return output.str();
  }
  
  Flux::string retaliate(Flux::string uname){
    stringstream output;
    if (!paralyzed){
      int hit = randint(1,100);
      if (hit <= players[uname]->evasion) {
	output << players[uname]->name << " dodged " << this->name << "'s attack!";
      } 
      else {
	int crit = randint(1,100);
	int dmg = randint(this->atkMin,this->atkMax) - randint(players[uname]->DEF,players[uname]->DEF*2);
	if (crit < this->critical) dmg *= 2;
	players[uname]->hp -= dmg;
	output << this->name << " deals " << dmg <<  " damage to " << players[uname]->name;
	if (!players[uname]->alive()){
	  output << ". " << players[uname]->name << " has died!";
	}
      }
    }else{
      output << this->name << " is paralyzed and can not move!";
      this->paralyzed_count += 1;
      if(paralyzed_count > 3){
	this->paralyzed = false;
	this->paralyzed_count = 0;
      }
    }
    return output.str();
  }
  
};

//////////////////////////////////////////////////////////////////////////////////////

class m_riista : public module
{
  monster *riista;
public:
  m_riista(const Flux::string &Name):module(Name), riista("Riista")
  {
    this->SetAuthor("Lordofsraam");
    ModuleHandler::Attach(I_OnPrivmsg, this);
  }

  void OnPrivmsgChannel(User *u, Channel *c, const std::vector<Flux::string> &params)
  {
    Flux::string message = CondenseString(params);

    riista->set_stats();
    if(message.search_ci(start_game))
    {
      c->SendMessage("%s has arrived! Let the battle begin!", riista->name.c_str());
      c->ChangeTopic(riista->get_health());
      game_started = true;
    }

    //join_game command
      if (message.search_ci("!join_game") && game_started) {
	if (!in_player_list(u->nick)){
	  playersV.push_back(u->nick);
	  c->SendMessage("%s has joined the fight!", u->nick.c_str());
	  //job selection
	  if (message.search_ci("fighter")) {
	    players[u->nick] = new Player(u->nick,"fighter");
	    players[u->nick]->set_stats();
	    c->SendMessage("%s is a Fighter.", u->nick.c_str());
	  }
	  else if (message.search_ci("mage")) {
	    players[u->nick] = new Player(u->nick,"mage");
	    players[u->nick]->set_stats();
	    c->SendMessage("%s is a Mage.", u->nick.c_str());
	  }
	  else if (message.search_ci("rogue")) {
	    players[u->nick] = new Player(u->nick,"rogue");
	    players[u->nick]->set_stats();
	    c->SendMessage("%s is a Rogue.", u->nick.c_str());
	  }
	  //if player didn't specify a valid job
	  else {
	    players[u->nick] = new Player(u->nick);
	    players[u->nick]->set_stats();
	    c->SendMessage("%s is clearly a n00b.", u->nick.c_str());
	  }
	  c->SendMessage(players[u->nick]->get_health());
	  c->SendMessage("Current Players: ");
	  c->SendMessage(print_vector(playersV));
	  c->SendMessage("");
	  //if more than one player has joined
	  if (playersV.size() > 1) {
	    c->SendMessage("%s is strengthened!", riista->name.c_str());
	    riista->END += 5;
	    riista->set_stats();
	    c->ChangeTopic(riista->get_health());
	  }
	}
	if (reply->said("!testing")){
	  srand(time(NULL));
	  int p1 = playersV.size();
	  int p2 = rand()%p1;
	  Flux::string victim = playersV[p2];
	  cout << p1 << endl;
	  cout << p2 << endl;
	  cout << victim << endl;
	}

	if (message.search_ci("!attack") && game_started){
	  if (in_player_list(u->nick) && players[u->nick]->alive()) {
	    if (message.search_ci("slash")){
	      c->SendMessage(riista->hurt(u->nick, "slash"));
	      c->ChangeTopic(riista->get_health());
	    }
	    else if (message.search_ci("cross_cut")){
	      c->SendMessage(riista->hurt(u->nick, "cross_cut"));
	      c->ChangeTopic(riista->get_health());
	    }
	    else if (message.search_ci("lucky_shot")){
	      c->SendMessage(riista->hurt(u->nick, "lucky_shot"));
	      c->ChangeTopic(riista->get_health());
	    }
	    //if player didn't specify a valid action
	    else {
	      c->SendMessage("Wait, what? ");
	    }
	    //riista's turn
	    if(riista->last_attacker == riista->last_attacker2 && riista->last_attacker2 == riista->last_attacker3){
	      c->SendMessage(riista->retaliate(riista->last_attacker));
	    }else{c->SendMessage(riista->retaliate());}
	  } else {
	    c->SendMessage("You are not in the game");
	  }
	}

	//make sure someone who has died can't come back
	else if (in_player_list(u->nick) && !players[u->nick]->alive()) {
	  c->SendMessage("You are dead...No zombies allowed...");
	}
	//in case someone tries to join twice under the same nick
	else {
	  c->SendMessage("You are already in the game!");
	}
      } else if (message.search_ci("!join_game") && !game_started)
      {
	c->SendMessage("Please start the game before joining.");

	if (message.search_ci("!magic") && game_started){
	  if (in_player_list(u->nick) && players[u->nick]->alive()) {
	    if (message.search_ci("heal")){
	      bool already_cast = false;
	      for(unsigned i = 0; i < playersV.size(); i++){
		if (message.search_ci(playersV[i]) && playersV[i] != unick && players[playersV[i]]->alive()){
		  if (players[unick]->mp >= 8) {
		    std::stringstream output;
		    int heals = (.05*players[playersV[i]]->INT)*players[playersV[i]]->maxHp;
		    players[playersV[i]]->hp += heals;
		    players[u->nick]->mp -= 8;
		    if (players[playersV[i]]->hp > players[playersV[i]]->maxHp) players[playersV[i]]->hp = players[playersV[i]]->maxHp;
		    output << players[u->nick]->name << " casts Heal on " << playersV[i] << " and heals them by " << heals << " HP!";
		    c->SendMessage(output.str());
		  }else{
		    c->SendMessage("Not enough MP or target is dead.");
		  }
		  already_cast = true;
		  break;
		}
	      }
	      if(!already_cast){
		c->SendMessage(players[u->nick]->heal());
		c->SendMessage(players[u->nick]->get_health());
	      }
	    }
	    else if(reply->said("paralyze")){
	      if (players[u->nick]->job == "Mage" or players[u->nick]->job == "Rogue"){
		if (players[u->nick]->mp > 20){
		  if(!riista->paralyzed){
		    c->SendMessage("%s is attempting to paralyze %s", u->nick.c_str(), riista->name.c_str());
		    players[u->nick]->mp -= 20;
		    srand(time(NULL));
		    int ri = rand()%100;
		    if (ri >= (100-((players[u->nick]->INT * 8)-(riista->AGI * 2)))){
		      riista->paralyzed = true;
		      c->SendMessage("The paralysis succeeded! %s is paralyzed!", riista->name.c_str());
		    }else{c->SendMessage("The paralysis failed!");}
		  }
		}else{c->SendMessage("Not enough MP!");}
	      }else
		c->SendMessage("You need to be a mage or a rogue to use this spell %s", u->nick.c_str());}
	  }
	  else if(message.search_ci("concentrate")){
	    if (players[u->nick]->job == "Mage"){
	      c->SendMessage(players[u->nick]->concentrate(u->nick));
	      if(players[u->nick]->mp > players[u->nick]->maxMp) players[u->nick]->mp = players[u->nick]->maxMp;
	      c->SendMessage(riista->retaliate(u->nick));
	    }else{c->SendMessage("You need to be a mage or a rogue to use this spell %s ", u->nick.c_str());}
      }
      else if(message.search_ci("forceus")){
	if (players[u->nick]->job == "Mage"){
	  c->SendMessage(riista->hurt(u->nick, "forceus"));
	  c->ChangeTopic(riista->get_health());
	}else{c->SendMessage("You need to be a mage or a rogue to use this spell %s", u->nick.c_str());}
      }
      //if player didn't specify a valid action
      else {
	c->SendMessage("Wait, what? ");
      }
      //riista's turn
      if(riista->last_attacker == riista->last_attacker2 && riista->last_attacker2 == riista->last_attacker3){
	c->SendMessage(riista->retaliate(riista->last_attacker));
      }else{
	c->SendMessage(riista->retaliate());
	c->SendMessage(players[u->nick]->get_health());
      }
    } else {
      c->SendMessage("You are not in the game");
    }
    if (message.search_ci("!stance") && game_started){
      if (in_player_list(u->nick) && players[u->nick]->alive()) {
	if (message.search_ci("defending")){
	  c->SendMessage(players[u->nick]->set_stance("defending"));
      }
      else if(message.search_ci("aggressive")){
	c->SendMessage(players[u->nick]->set_stance("aggressive"));
      }
      else if(message.search_ci("running")){
	c->SendMessage(players[u->nick]->set_stance("running"));
      }
      else if(message.search_ci("normal")){
	c->SendMessage(players[u->nick]->set_stance("normal"));
      }
      }else{c->SendMessage("You are dead.");}
      }
      
      if (reply->said("!my_stats")){
	  if (in_player_list(u->nick)){
	    u->SendMessage(players[u->nick]->get_health());
	  }else{
	    u->SendMessage("You are not in the game");
	  }
      }
      
      
      if (game_started && riista->alive() && playersV.size() > 0){
	int ri = randint(1,100);
	if (ri <= 10){
	  c->SendMessage("Has suddenly become enraged!");
	  c->SendMessage(riista->retaliate());
	}
      }


      if (!riista->alive()){
	/*continue;
      }else{*/
	c->SendMessage(riista->name+" is dead!");
	c->SendMessage(riista->last_attacker+" killed "+riista->name+"!");
	sock << quit("SCREW you guys! I'M going Home!");
	delete riista;
	exit(EXIT_FAILURE);
      }
  }//magic commands end here

  }
};

#endif