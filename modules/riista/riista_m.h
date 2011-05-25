#ifndef RIISTA_H
#define RIISTA_H
#include "../../includes.h"
#include "../../flux_net_irc.hpp"
using namespace std;
using namespace flux_net_irc;
void riista_m(Socket &sock, irc_string *reply){

  //Game code here:
      
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
	    for(int i = 0; i < playersV.size(); i++){
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
      
      
      if (riista->alive()){
	continue;
      }else{
	sock << chanmsg(riista->name+" is dead!");
	sock << chanmsg(riista->last_attacker+" killed "+riista->name+"!");
	sock << quit("SCREW you guys! I'M going Home!");
	delete riista;
	exit(EXIT_FAILURE);
      }

}
#endif