/* Riista IRC bot -- main game file
 * 
 * (C) 2011-2012 Flux-Net
 * Contact us at Dev@Flux-Net.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */
#include "monster.h"

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

class m_riista : public module
{
  monster *riista;
public:
  m_riista(const Flux::string &Name):module(Name), riista(new monster("Riista"))
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