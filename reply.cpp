#include "includes.h"
/**
  *\fn irc_string(Flux::string reply)
  *Constructor for \a irc_string class
  * This is where the magic happens. The constructor takes the Flux::string
  * and breaks it down into its component parts to find the 
  * \a usernick \a host \a user \a channel and \a message
  */
irc_string::irc_string(Flux::string reply){
  raw_string = reply;
  usernick = isolate(':','!',reply);
  host = isolate('@',' ',reply);
  user = isolate('!','@',reply);
  channel = '#'+isolate('#',' ',reply);
  Flux::string space = " ";
  size_t pos = reply.find(" :");
  pos += 2;
  for (unsigned i = pos; i < reply.size(); i++){
    if (reply.at(i) == ' '){
      message.append(space);
    }else{message = message+reply.at(i);}
  }
  if(message.size()>2){
    message.resize(message.size()-2);
  }
  
  Flux::string fmessage = message;
  char * cmessage = (char *)fmessage.c_str();
  char * pch;
  pch = strtok(cmessage," ");
  while (pch != NULL)
  {
    toks.push_back(pch);
    pch = strtok(NULL, " ");
  }
}
/**
  * \fn Flux::string params(int i)
  * \brief Returns individual words from the message of a reply
  * Because \a toks is private, this is its "get" function.
  * We made this so someone writing a module doesn't try to go out 
  * of bounds while accessing an array.
  * \param i An integer value.
  * \return A Flux::string with the single specified word.
  */
Flux::string irc_string::params(unsigned i){
  if (i >= toks.size()){
    return " ";
  }else{return toks[i];}
}

bool irc_string::isnumeric(int i){
  if(said(stringify(i))){
    if(2 >= toks.size()){
      return false;
    }
    Flux::string tok = toks[2];
    tok.trim();
      if (tok.is_number_only()){
	int a = atoi(tok.c_str());
	if((a = i)){
	  return true;
	}
      }
    }
  return false;
}
/**
  * \overload Flux::string params(int b, int e)
  * \brief Overload of params. Returns a range of words.
  * We overloaded \a params() so that you could get a range of words from the message
  *  as requeseted by Justasic.
  * \param b An integer value describing the place of the first word wanted.
  * \param e An integer value describing the place of the last word wanted.
  * \return A Flux::string withing the range of words specified by \a b and \a e
  */
Flux::string irc_string::params(unsigned b, unsigned e){
  Flux::string buf = "";
  if (b >= toks.size()){
    b = toks.size();
  }
  if (e >= toks.size()){
    e = toks.size() - 1;
  }
  for (unsigned i = b; i <= (e); i++){
    buf += toks[i];
    buf.append(" ");
  }
  return buf;
}
/**
  * \fn static Flux::string isolate(char begin, char end, Flux::string msg)
  * \brief Isolates a Flux::string between two characters
  * Finds the first character, then begins to add every consecutive character from there to a Flux::string
  *  until it reaches the end character.
  * \param begin The character saying where the cut should begin.
  * \param end The character saying where the cut should end.
  * \param msg The Flux::string you are wanting to isolate from.
  */
Flux::string irc_string::isolate(char begin, char end, Flux::string msg){
  Flux::string to_find;
  size_t pos = msg.find(begin);
  pos += 1;
  for (unsigned i = pos; i < msg.length(); i++){
    if (msg.at(i) == end){
      break;
    }else{to_find = to_find+msg.at(i);}
  }
  return to_find;
}
/**
  * \fn bool said(Flux::string findee)
  * \brief Check if something was said.
  * \param findee The Flux::string you want to check if was said.
  * \return True if \a findee was found, false otherwise.
  */
bool irc_string::said(Flux::string findee){
  unsigned i = raw_string.find(findee);
  if (i != Flux::string::npos){
    return true;
  }else{return false;}
}
/**
  * \overload static bool said(Flux::string source, Flux::string findee)
  * \brief Static overload of said() function.
  * \param source A Flux::string that is to be searched through.
  * \param findee The Flux::string that is to be found.
  * We overloaded the said function and made it static because we thought it would be 
  *  very useful to have outside of an \a irc_string object.
  * \return True if \a findee was found, false otherwise.
  */
bool irc_string::said(Flux::string source, Flux::string findee){
  unsigned i = source.find(findee);
  if (i != Flux::string::npos){
    return true;
  }else{return false;}
}

IsoHost::IsoHost(Flux::string fullhost){
  nick = irc_string::isolate(':','!',fullhost);
  raw = fullhost;
  host = irc_string::isolate('@',' ',fullhost);
  user = irc_string::isolate('!','@',fullhost);
}