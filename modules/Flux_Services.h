#ifndef FS
#define FS
#include "../includes.h"
#include "../flux_net_irc.hpp"
using namespace std;
using namespace flux_net_irc;
int idiots = 0; //define the idiot counter outside of the main loop
int magiccount = 0;
void Flux_Services(Socket &sock, irc_string *reply){
	if(reply->said("!part ")){ 
		if(unick == owner_nick){
			sock << privmsg(chan, "im out niggaz!");
			sock << part(reply->params(1), "I'm leaving this dump.");
			log("%s used Flux_S3rvices part %s", unick.c_str(), chan.c_str());
		}else{
			sock << notice(unick, access_denied);
		}
	}
	if(reply->said("!botadd")){
		if(unick == owner_nick){
			string bnick = reply->params(1);
			string buser = reply->params(2);
			string bhost = reply->params(3);
			string breal = reply->params(4);
			sock << privmsg("BotServ", "add %s %s %s %s", bnick.c_str(), buser.c_str(), bhost.c_str(), breal.c_str());
			log("%s used Flux_S3rvices to make bot \"%s!%s@%s :%s\" %s", unick.c_str(), bnick.c_str(), buser.c_str(), bhost.c_str(), breal.c_str(), chan.c_str());
		}else{
		sock << notice(unick, access_denied);
		}
	}
	if(reply->said("slaps")){
	 sock << privmsg(chan, "\0036Oh \0034Hell \0039No...");
	 int num = randint(1,6); //make a random number from 1 to 6 (increase 6 to how ever many switch statements below) so messages are random
	 switch(num){
	   case 1:
	     sock << me(chan, "\00313,1Dodges and \0037Pulls out a \0036Energy \0034Sword \0033and stabs\0032 %s \00312in the eye \0034.........what now \0039B1TCH?", unick.c_str());
	     break; //break after each case so it doesnt keep trying to get the messages in the switch statement
	   case 2:
	     sock << me(chan, "\00313,1Dodges and \0037Shoots\0032 %s \00312in the \0034A\0038$\0039S \0033with a \0039AK47 \0034.......what now \0039B1TCH?", unick.c_str());
	     break;
	   case 3:
	     sock << me(chan, "\00313,1Dodges and \0037Pulls out a \0036Ballistic Knife \0033and shoots\0032 %s \00312in the \00313DICK/PUSSY \0034.........what now \0039B1TCH?", unick.c_str());
	     break;
	   case 4:
	     sock << me(chan, "\00313,1Dodges and \0037Pulls out a \0036Ray \0038Gun \0033and blows\0032 %s's \00312legs off \0034..........what now \0039B1TCH?", unick.c_str());
	     break;
	   case 5:
	     sock << me(chan, "\00313,1Dodges and \0037Pulls out his \0034D1CK \0033and slaps\0032 %s \00312in the \0039Face \0034.........what now \0039B1TCH?", unick.c_str());
	     break;
	   case 6:
	     sock << me(chan, "\0034,1bends dows and and \00312 grabs a M2W LAW and \0034 shoots back and \0038 SCORE it a win..................\0030 what now muthafucker!");
	     break;
	}
	 idiots++; //add +1 to the already big number of idiots in the list
	 sock << privmsg(chan, "\2\0034,1Total \0039morons \0033slapped \0038back \00310: %i", idiots); //tell the channel how many idiots where slapped.
	}
	if(reply->said("!magicbox")){
	 int num = randint(1,20);
	 string object;
	  switch(num){ //FIXME: This switch statement will become rather large.. perhapse rewriting it to read from a wordlist?
	    case 1:
	      object = "toothbrush (Object #"+stringify(num)+")";
	      break;
	    case 2:
	      object = "balloon (Object #"+stringify(num)+")";
	      break;	      
	    case 3:
	      object = "spacecraft (Object #"+stringify(num)+")";
	      break;	      
	    case 4:
	      object = "apple (Object #"+stringify(num)+")";
	      break;	      
	    case 5:
	      object = "basketball (Object #"+stringify(num)+")";
	      break;	      
	    case 6:
	      object = "$10,000 check (Object #"+stringify(num)+")";
	      break;
	    case 7:
	      object = "dinner table (Object #"+stringify(num)+")";
	      break;	      
	    case 8:
	      object = "couch (Object #"+stringify(num)+")";
	      break;	      
	    case 9:
	      object = "lawn chair (Object #"+stringify(num)+")";
	      break;	      
	    case 10:
	      object = "pink flamingo (Object #"+stringify(num)+")";
	      break;	      
	    case 11:
	      object = "KFC bucket (Object #"+stringify(num)+")";
	      break;	      
	    case 12:
	      object = "used condom (Object #"+stringify(num)+")";
	      break;	      
	    case 13:
	      object = "candy bar (Object #"+stringify(num)+")";
	      break;	      
	    case 14:
	      object = "door knob (Object #"+stringify(num)+")";
	      break;	      
	    case 15:
	      object = "free bottle of vodka (Object #"+stringify(num)+")";
	      break;	      
	    case 16:
	      object = "empty can (Object #"+stringify(num)+")";
	      break;
	    case 17:
	      object = "light bulb (Object #"+stringify(num)+")";
	      break;	      
	    case 18:
	      object = "arm & hammer box (Object #"+stringify(num)+")";
	      break;	      
	    case 19:
	      object = "laundry detergent (Object #"+stringify(num)+")";
	      break;	      
	    case 20:
	      object = "toilet paper roll (Object #"+stringify(num)+")";
	      break;	              
	  }
	  magiccount++;
	  sock << me(chan, "%s gives %s a %s", nick.c_str(), unick.c_str(), object.c_str());
	  sock << privmsg(chan, "MagicBox used %i times", magiccount);
	}
	/*
	if(reply->said("!umode")){
		if(unick == owner_nick){
			string dest = reply->params(1);
			cout << "final: "<<msg << endl;
			sock << samode(dest, "%s", msg.c_str());
		}else{
			sock << notice(unick, access_denied);
		}	
	}*/
}
#endif
