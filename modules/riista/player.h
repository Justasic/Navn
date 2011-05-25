#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <sstream>
#include <signal.h>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

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
  for (int i = 0; i < playersV.size(); i++){
    if (playersV[i] == n){
      playersV.erase(playersV.begin()+i);
    }
  }
}