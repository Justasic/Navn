/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#ifndef FS
#define FS
#include "includes.h"
#include "flux_net_irc.hpp"

/**
 * \file Flux_Services.h Header file holding the \a Flux_S3rvices functions.
 * \author DeathBlade. Polished by Justasic.
 */

/**
 * \defgroup flux_servicesM Flux_S3rvices Module
 * This is the module for the Flux_S3rvices function.
 * For a better description see the function description.
 * \section commands Commands associated with this module.
 * \subsection part !part
 * Say \a !part and the bot will part the channel the command was said in
 * \subsection botadd !botadd
 * Say \a !botadd with the correct parameters and if the bot is oper, the bot will
 * create a new anope channel bot for the network
 * \subsection magicbox !magicbox
 * Say \a !magicbox in a channel and the bot will give you a random object from the list in the switch statement
 * @{
 */

/**
 * \fn class Flux_Services(bool a):module("Flux_Services", a, PRIORITY_LAST){ this->SetDesc("DeathBlade's Flux_S3rvices bot"); }
 * \brief Flux_S3rvices module created by DeathBlade
 */
  int idiots, magiccount = 0; //define the counters outside of the main loop
class Flux_Services:public module
{
public:
  Flux_Services(bool a):module("Flux_Services", a, PRIORITY_LAST){ this->SetDesc("DeathBlade's Flux_S3rvices bot"); }
ModuleReturn run(CommandSource &source, std::vector<Flux::string> &params){
  Flux::string cmd = params.empty()?"":params[0];
  User *u = source.u;
  Channel *c = source.c;
	if(cmd.equals_ci("!part")){ 
		if(u->IsOwner()){
		  Flux::string pchannel = params.size() == 2?params[1]:"";
		  if(pchannel.empty()){
		   c->SendMessage("im out niggaz!");
		   c->SendPart("I'm leaving this dump."); 
		   log(LOG_NORMAL, "%s used Flux_S3rvices part %s", u->nick.c_str(), c->name.c_str());
		   return MOD_STOP;
		  }
		  if(!IsValidChannel(pchannel)){
		    source.Reply("Channel \2%s\2 is not a valid channel", pchannel.c_str());
		    return MOD_STOP;
		  }
		  Channel *ch = findchannel(pchannel);
		  if(!c){
		    source.Reply("I am not in channel \2%s\2", pchannel.c_str());
		    return MOD_STOP;
		  }
		  ch->SendMessage("im out niggaz!");
		  ch->SendPart("I'm leaving this dump.");
		  log(LOG_NORMAL, "%s used Flux_S3rvices part %s", u->nick.c_str(), pchannel.c_str());
		}else{
			source.Reply(ACCESS_DENIED);
		}
	}
	if(cmd.equals_ci("!botadd")){
		if(u->IsOwner()){
		  if(params.size() < 5){
		    source.Reply("Syntax: \2!botadd \37nick\37 \37user\37 \37host\37 \37real\37");
		   return MOD_STOP; 
		  }
			Flux::string bnick = params[1];
			Flux::string buser = params[2];
			Flux::string bhost = params[3];
			Flux::string breal = params[4];
			Send->privmsg("BotServ", "bot add %s %s %s %s", bnick.c_str(), buser.c_str(), bhost.c_str(), breal.c_str());
			log(LOG_NORMAL, "%s used Flux_S3rvices to make bot \"%s!%s@%s :%s\" %s", u->nick.c_str(), bnick.c_str(), buser.c_str(), bhost.c_str(), breal.c_str(), c->name.c_str());
		}else{
		source.Reply(ACCESS_DENIED);
		}
	}
	if(irc_string::said(source.message, ci::string("slaps"))){
	 c->SendMessage("\0036Oh \0034Hell \0039No...");
	 int num = randint(1,6); //make a random number from 1 to 6 (increase 6 to how ever many switch statements below) so messages are random
	 switch(num){
	   case 1:
	     c->SendAction("\00313,1Dodges and \0037Pulls out a \0036Energy \0034Sword \0033and stabs\0032 %s \00312in the eye \0034.........what now \0039B1TCH?", u->nick.c_str());
	     break; //break after each case so it doesnt keep trying to get the messages in the switch statement
	   case 2:
	     c->SendAction("\00313,1Dodges and \0037Shoots\0032 %s \00312in the \0034A\0038$\0039S \0033with a \0039AK47 \0034.......what now \0039B1TCH?", u->nick.c_str());
	     break;
	   case 3:
	     c->SendAction("\00313,1Dodges and \0037Pulls out a \0036Ballistic Knife \0033and shoots\0032 %s \00312in the \00313DICK/PUSSY \0034.........what now \0039B1TCH?", u->nick.c_str());
	     break;
	   case 4:
	     c->SendAction("\00313,1Dodges and \0037Pulls out a \0036Ray \0038Gun \0033and blows\0032 %s's \00312legs off \0034..........what now \0039B1TCH?", u->nick.c_str());
	     break;
	   case 5:
	     c->SendAction("\00313,1Dodges and \0037Pulls out his \0034D1CK \0033and slaps\0032 %s \00312in the \0039Face \0034.........what now \0039B1TCH?", u->nick.c_str());
	     break;
	   case 6:
	     c->SendAction("\0034,1bends dows and and \00312 grabs a M2W LAW and \0034 shoots back and \0038 SCORE it a win..................\0030 what now muthafucker!");
	     break;
	}
	 idiots++; //add +1 to the already big number of idiots in the list
	 c->SendMessage("\2\0034,1Total \0039morons \0033slapped \0038back \00310: %i", idiots); //tell the channel how many idiots where slapped.
	}
	if(cmd.equals_ci("!magicbox")){
	 int num = randint(1,20);
	 Flux::string object;
	  switch(num){ //FIXME: This switch statement will become rather large.. perhapse rewriting it to read from a wordlist?
	    case 1:
	      object = "toothbrush (Object #"+Flux::stringify(num)+")";
	      break;
	    case 2:
	      object = "balloon (Object #"+Flux::stringify(num)+")";
	      break;	      
	    case 3:
	      object = "spacecraft (Object #"+Flux::stringify(num)+")";
	      break;	      
	    case 4:
	      object = "apple (Object #"+Flux::stringify(num)+")";
	      break;	      
	    case 5:
	      object = "basketball (Object #"+Flux::stringify(num)+")";
	      break;	      
	    case 6:
	      object = "$10,000 check (Object #"+Flux::stringify(num)+")";
	      break;
	    case 7:
	      object = "dinner table (Object #"+Flux::stringify(num)+")";
	      break;	      
	    case 8:
	      object = "couch (Object #"+Flux::stringify(num)+")";
	      break;	      
	    case 9:
	      object = "lawn chair (Object #"+Flux::stringify(num)+")";
	      break;	      
	    case 10:
	      object = "pink flamingo (Object #"+Flux::stringify(num)+")";
	      break;	      
	    case 11:
	      object = "KFC bucket (Object #"+Flux::stringify(num)+")";
	      break;	      
	    case 12:
	      object = "used condom (Object #"+Flux::stringify(num)+")";
	      break;	      
	    case 13:
	      object = "candy bar (Object #"+Flux::stringify(num)+")";
	      break;	      
	    case 14:
	      object = "door knob (Object #"+Flux::stringify(num)+")";
	      break;	      
	    case 15:
	      object = "free bottle of vodka (Object #"+Flux::stringify(num)+")";
	      break;	      
	    case 16:
	      object = "empty can (Object #"+Flux::stringify(num)+")";
	      break;
	    case 17:
	      object = "light bulb (Object #"+Flux::stringify(num)+")";
	      break;	      
	    case 18:
	      object = "arm & hammer box (Object #"+Flux::stringify(num)+")";
	      break;	      
	    case 19:
	      object = "laundry detergent (Object #"+Flux::stringify(num)+")";
	      break;	      
	    case 20:
	      object = "toilet paper roll (Object #"+Flux::stringify(num)+")";
	      break;	              
	  }
	  magiccount++;
	  c->SendAction("%s gives %s a %s", nick.c_str(), u->nick.c_str(), object.c_str());
	  c->SendMessage("MagicBox used %i times", magiccount);
	}
	/*
	if(cmd == "!umode"){
		if(u->IsOwner()){
			Flux::string dest = reply->params(1);
			cout << "final: "<<msg << endl;
			Send->raw(samode(dest, "%s", msg.c_str()).c_str());
		}else{
			Send->notice(u->nick, access_denied);
		}	
	}*/
	return MOD_RUN;
}
};
/**
 * @}
 */
#endif
