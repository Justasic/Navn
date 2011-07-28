#ifndef RIISTA_H
#define RIISTA_H
#include "../../includes.h"
#include "../../flux_net_irc.hpp"
//#include "monster.h"
//#include "player.h"
using namespace std;
using namespace flux_net_irc;

//const string password = make_pass();
const string start_game = "!start_game "+password;

class Player {
public:
  int maxHp;
  int hp;
  int maxMp;
  int mp;
  int STR;
  int END;
  int DEF;
  int AGI;
  int INT;
  int LUK;
  
  int atkMin;
  int atkMax;
  int magMin;
  int magMax;
  int evasion;
  int critical;
  
  
  string name;
  string job;
  string stance;  
  
  //constructor
  Player(string user) {
     name = user;
     stance = "normal";
     job = "n00b";
     STR = 3;
     END = 3;
     DEF = 3;
     AGI = 3;
     INT = 3;
     LUK = 3;     
  }
  Player(string user, string job_name) {
     name = user;
     if(job_name == "n00b"){
      job = "n00b";
      stance = "normal";
      STR = 3;
      END = 3;
      DEF = 3;
      AGI = 3;
      INT = 3;
      LUK = 3;
     }
     if(job_name == "mage"){
      job = "Mage";
      stance = "normal";
      STR = 4;
      END = 5;
      DEF = 6;
      AGI = 6;
      INT = 9;
      LUK = 6;
     }
     if(job_name == "rogue"){
      job = "Rogue";
      stance = "normal";
      STR = 5;
      END = 6;
      DEF = 5;
      AGI = 8;
      INT = 6;
      LUK = 7;
     }
     if(job_name == "fighter"){
      job = "Fighter";
      stance = "normal";
      STR = 8;
      END = 7;
      DEF = 7;
      AGI = 5;
      INT = 4;
      LUK = 5;
     }    
  }
  
  void set_stats() {
    maxHp = END * 100;
    hp = maxHp;
    maxMp = INT * 20;
    mp = maxMp;
    
    atkMin = STR * 4;
    atkMax = STR * 6;
    magMin = INT * 4;
    magMax = INT * 6;
    evasion = AGI * 2.5;
    critical = LUK;
  }
  
  void set_stance_stats() {
    atkMin = STR * 4;
    atkMax = STR * 6;
    magMin = INT * 4;
    magMax = INT * 6;
    evasion = AGI * 3;
    critical = LUK;
  }
  
  void reset_stats(){
    if(this->job == "n00b"){
      STR = 3;
      END = 3;
      DEF = 3;
      AGI = 3;
      INT = 3;
      LUK = 3;
     }
     if(this->job == "Mage"){
      STR = 4;
      END = 5;
      DEF = 6;
      AGI = 6;
      INT = 9;
      LUK = 6;
     }
     if(this->job == "Rogue"){
      STR = 5;
      END = 6;
      DEF = 5;
      AGI = 8;
      INT = 6;
      LUK = 7;
     }
     if(this->job == "Fighter"){
      STR = 8;
      END = 7;
      DEF = 7;
      AGI = 5;
      INT = 4;
      LUK = 5;
     }    
  }
  
  string get_health(){
    stringstream health;
    health << this->name << "'s HP: " << this->hp << "/" << this->maxHp << "\t" << "MP: " << this->mp << "/" << this->maxMp << ". Job: " << this->job << ". Stance: " << this->stance;
    string output = health.str();
    return output;
  }
  
  bool alive(){
    if (this->hp <= 0){
      return false;
    }else{return true;}
  }
  
  string heal() {
    if (this->mp >= 8) {
    stringstream output;
    int heals = (.1*this->INT)*this->maxHp;
    this->hp += heals;
    this->mp -= 8;
    if (this->hp > this->maxHp) this->hp = this->maxHp;
    output << this->name << " casts Heal and recovers " << heals << " HP!";
    return output.str();
  } else {
    return "Not enough MP";
    }
  }
  
  string concentrate(string uname){
    this->mp += 10;
    return uname + " concentrated and gained 10 MP!";
  }
  
  string set_stance(string stance_name){
    reset_stats();
    if (stance_name == "defending"){
      this->DEF *= 4;
      this->STR *= 0.5;
      this->stance = "defending";
      set_stance_stats();
      return this->name+" is now defending.";
    }
    else if (stance_name == "aggressive"){
      this->DEF *= 0.25;
      this->STR *= 2;
      this->stance = "aggressive";
      set_stance_stats();
      return this->name+" is now in an aggressive stance.";
    }
    else if (stance_name == "running"){
      this->AGI *= 2;
      this->STR *= 1.3;
      this->DEF *= 0.7;
      this->stance = "running";
      set_stance_stats();
      return this->name+" is now running.";
    }
    else if (stance_name == "normal"){
      reset_stats();
      this->stance = "normal";
      set_stance_stats();
      return this->name+" has reset their stance.";
    }
    else{
      return "Unrecognized stance.";
    }
  }
  
};

/////////////////////////////////////////////////////////////////////////////////////////////
map<string, Player*> players;
vector<string> playersV;

bool in_player_list(string n){
  bool t; 
  if(find(playersV.begin(), playersV.end(), n)!=playersV.end()){
    t = true;
  }
  else{
    t = false;
  }
  return t;  
}

void delete_from_playersV(string n){
  for (unsigned i = 0; i < playersV.size(); i++){
    if (playersV[i] == n){
      playersV.erase(playersV.begin()+i);
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////

class monster {
public :
  int maxHp;
  int hp;
  int STR;
  int END;
  int DEF;
  int AGI;
  int INT;
  int LUK;
  
  int atkMin;
  int atkMax;
  
  int evasion;
  int critical;
  
  int paralyzed_count;
  bool paralyzed;
  
  string name;
  string last_attacker;
  string last_attacker2;
  string last_attacker3;
  
  //constructor
  monster(string n){
    name = n;
    STR = 15;
    END = 10;
    DEF = 7;
    AGI = 4;
    INT = 7;
    LUK = 5;
    
    paralyzed = false;
    paralyzed_count = 0;
  }
  
  void set_stats() {
    maxHp = END * 100;
    hp = maxHp;
    
    atkMin = STR * 4;
    atkMax = STR * 6;
    
    evasion = AGI * 2.5;
    critical = LUK;
  }
  
  bool alive(){
    if (hp <= 0){
      return false;
    }else{return true;}
  }
  
  string get_health(){
    stringstream health;
    health << name << "'s HP: " << hp << "/" << maxHp;
    string output = health.str();
    return output;
  }
  
  string hurt(string uname, string attack_name){  
      last_attacker3 = last_attacker2;
      last_attacker2 = last_attacker;
      last_attacker = uname;
      stringstream output;
      if (attack_name == "slash"){
	int hit = randint(1,100);
	int crit = randint(1,100);
	if (hit <= this->evasion) {
	  output << this->name << " dodged your attack!";
	}
	else {
	  int dmg = randint(players[uname]->atkMin,players[uname]->atkMax) - randint(this->DEF,this->DEF*2);
	  if (dmg < 1) dmg = 1;
	  if (crit < players[uname]->critical) dmg *= 2;
	  this->hp -= dmg;
	  output << uname << " slashed " << this->name << " and did " << dmg << " damage!";
	}
	return output.str();
      }
      else if(attack_name == "cross_cut"){
	if(players[uname]->job == "Fighter" or players[uname]->job == "Rogue"){
	  if(players[uname]->mp > 4){
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
	  }else{return "Not enough MP.";}
	}else{return "You need to be a Fighter or Rogue to use this attack.";}
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
  
  string retaliate(){
    stringstream output;
    if (!paralyzed){
      srand(time(NULL));
      int p1;
      int p2;
      string victim;
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
  
  string retaliate(string uname){
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



monster* riista = new monster("Riista");
//riista->set_stats();

void riista_m(Socket &sock, irc_string *reply){

  //Game code here:
      riista->set_stats();
      //start_game command
      if (reply->said(start_game)){
	sock << chanmsg(riista->name+" has arrived! Let the battle begin!");
	sock << topic(channel,riista->get_health());
	game_started = true;
      }
      
      //join_game command
      if (reply->said("!join_game") && game_started) {
	if (!in_player_list(unick)){
	  playersV.push_back(unick);
	  sock << chanmsg(unick+" has joined the fight!");
	  //job selection
	  if (reply->said("fighter") xor reply->said("Fighter")) {
	    players[unick] = new Player(unick,"fighter");
	    players[unick]->set_stats();
	    sock << chanmsg(unick+" is a Fighter.");
	  } 
	  else if (reply->said("mage") xor reply->said("Mage")) {
	    players[unick] = new Player(unick,"mage");
	    players[unick]->set_stats();
	    sock << chanmsg(unick+" is a Mage.");
	  }
	  else if (reply->said("rogue") xor reply->said("Rogue")) {
	    players[unick] = new Player(unick,"rogue");
	    players[unick]->set_stats();
	    sock << chanmsg(unick+" is a Rogue.");
	  }
	  //if player didn't specify a valid job
	  else {
	    players[unick] = new Player(unick);
	    players[unick]->set_stats();
	    sock << chanmsg(unick+" is clearly a n00b.");
	  }
	  sock << chanmsg(players[unick]->get_health());
	  sock << chanmsg("Current Players: ");
	  sock << chanmsg(print_vector(playersV));
	  sock << chanmsg("");
	  //if more than one player has joined
	  if (playersV.size() > 1) {
	    sock << chanmsg(riista->name+" is strengthened!");
	    riista->END += 5;
	    riista->set_stats();
	    sock << topic(channel,riista->get_health());
	  }
	}
	//make sure someone who has died can't come back
	else if (in_player_list(unick) && !players[unick]->alive()) {
	  sock << chanmsg("You are dead...No zombies allowed...");
	}
	//in case someone tries to join twice under the same nick
	else {
	  sock << chanmsg("You are already in the game!");
	}
      }
      
      //if somone tries to join before the game is started
      if (reply->said("!join_game") && !game_started) {
	sock << chanmsg("Please start the game before joining.");
      }
      
      if (reply->said("!testing")){
	srand(time(NULL));
	int p1 = playersV.size();
	int p2 = rand()%p1;
	string victim = playersV[p2];
	cout << p1 << endl;
	cout << p2 << endl;
	cout << victim << endl;
      }
      
      //attack command
      if (reply->said("!attack") && game_started){
	if (in_player_list(unick) && players[unick]->alive()) {
	  if (reply->said("slash")){
	    sock << chanmsg(riista->hurt(unick, "slash"));
	    sock << topic(channel,riista->get_health());
	  }
	  else if (reply->said("cross_cut")){
	    sock << chanmsg(riista->hurt(unick, "cross_cut"));
	    sock << topic(channel,riista->get_health());
	  }
	  else if (reply->said("lucky_shot")){
	    sock << chanmsg(riista->hurt(unick, "lucky_shot"));
	    sock << topic(channel,riista->get_health());
	  }
	  //if player didn't specify a valid action
	  else {
	    sock << chanmsg("Wait, what? ");
	  }
	  //riista's turn
	  if(riista->last_attacker == riista->last_attacker2 && riista->last_attacker2 == riista->last_attacker3){
	    sock << chanmsg(riista->retaliate(riista->last_attacker));
	  }else{sock << chanmsg(riista->retaliate());}
	} else {
	  sock << chanmsg("You are not in the game");
	}
      }
      
      //magic command
      if (reply->said("!magic") && game_started){
	if (in_player_list(unick) && players[unick]->alive()) {
	  if (reply->said("heal")){
	    bool already_cast = false;
	    for(unsigned i = 0; i < playersV.size(); i++){
	      if (reply->said(playersV[i]) && playersV[i] != unick && players[playersV[i]]->alive()){		
		if (players[unick]->mp >= 8) {
		  stringstream output;
		  int heals = (.05*players[playersV[i]]->INT)*players[playersV[i]]->maxHp;
		  players[playersV[i]]->hp += heals;
		  players[unick]->mp -= 8;
		  if (players[playersV[i]]->hp > players[playersV[i]]->maxHp) players[playersV[i]]->hp = players[playersV[i]]->maxHp;
		  output << players[unick]->name << " casts Heal on " << playersV[i] << " and heals them by " << heals << " HP!";
		  sock << chanmsg(output.str());
		}else{
		  sock << chanmsg("Not enough MP or target is dead.");
		}
		already_cast = true;
		break;
	      }
	    }
	    if(!already_cast){
	      sock << chanmsg(players[unick]->heal());
	      sock << chanmsg(players[unick]->get_health());
	    }	    
	  }
	  else if(reply->said("paralyze")){
	    if (players[unick]->job == "Mage" or players[unick]->job == "Rogue"){
	      if (players[unick]->mp > 20){
		if(!riista->paralyzed){
		  sock << chanmsg(unick+" is attempting to paralyze "+riista->name);
		  players[unick]->mp -= 20;
		  srand(time(NULL));
		  int ri = rand()%100;
		  if (ri >= (100-((players[unick]->INT * 8)-(riista->AGI * 2)))){
		    riista->paralyzed = true;
		    sock << chanmsg("The paralysis succeeded! "+riista->name+" is paralyzed!");
		  }else{sock << chanmsg("The paralysis failed!");}
		}
	      }else{sock << chanmsg("Not enough MP!");}
	    }else{sock << chanmsg("You need to be a mage or a rogue to use this spell "+unick);}
	  }
	  else if(reply->said("concentrate")){
	    if (players[unick]->job == "Mage"){
	      sock << chanmsg(players[unick]->concentrate(unick));
	      if(players[unick]->mp > players[unick]->maxMp) players[unick]->mp = players[unick]->maxMp;
	      sock << chanmsg(riista->retaliate(unick));
	    }else{sock << chanmsg("You need to be a mage or a rogue to use this spell "+unick);}
	  }
	  else if(reply->said("forceus")){
	    if (players[unick]->job == "Mage"){
	      sock << chanmsg(riista->hurt(unick, "forceus"));
	      sock << topic(channel,riista->get_health());
	    }else{sock << chanmsg("You need to be a mage or a rogue to use this spell "+unick);}
	  }
	  //if player didn't specify a valid action
	  else {
	    sock << chanmsg("Wait, what? ");
	  }
	  //riista's turn
	  if(riista->last_attacker == riista->last_attacker2 && riista->last_attacker2 == riista->last_attacker3){
	    sock << chanmsg(riista->retaliate(riista->last_attacker));
	  }else{
	    sock << chanmsg(riista->retaliate());
	    sock << chanmsg(players[unick]->get_health());
	  }
	} else {
	  sock << chanmsg("You are not in the game");
	}
      }//magic commands end here
      
      if (reply->said("!stance") && game_started){
	if (in_player_list(unick) && players[unick]->alive()) {
	  if (reply->said("defending") or reply->said("Defending")){
	    sock << chanmsg(players[unick]->set_stance("defending"));
	  }
	  else if(reply->said("aggressive") or reply->said("Aggressive")){
	    sock << chanmsg(players[unick]->set_stance("aggressive"));
	  }
	  else if(reply->said("running") or reply->said("Running")){
	    sock << chanmsg(players[unick]->set_stance("running"));
	  }
	  else if(reply->said("normal") or reply->said("Normal")){
	    sock << chanmsg(players[unick]->set_stance("normal"));
	  }
	}else{sock << chanmsg("You are dead.");}
      }
      
      if (reply->said("!my_stats")){
	if (in_player_list(unick)){
	  sock << notice(unick, players[unick]->get_health());
	}else{
	  sock << notice(unick, "You are not in the game");
	}
      }
      
      
      if (game_started && riista->alive() && playersV.size() > 0){
	int ri = randint(1,100);
	if (ri <= 10){
	  sock << chanmsg("Has suddenly become enraged!");
	  sock << chanmsg(riista->retaliate());
	}
      }
      
      
      if (!riista->alive()){
	/*continue;
      }else{*/
	sock << chanmsg(riista->name+" is dead!");
	sock << chanmsg(riista->last_attacker+" killed "+riista->name+"!");
	sock << quit("SCREW you guys! I'M going Home!");
	delete riista;
	exit(EXIT_FAILURE);
      }

}

#endif