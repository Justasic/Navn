#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <sstream>
#include <signal.h>
#include "defs.h"
#include "player.h"

using namespace std;

int randint(int x, int y) {
  srand(time(NULL));
  return rand()%(y-x+1)+x;
}

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