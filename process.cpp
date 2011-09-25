#include "user.h"
#include "module.h"

/** 
 * \fn void ProcessJoin(CommandSource &source, const Flux::string &chan)
 * \brief Processes the /who numeric (352), this should only be used in Process() unless its for something special
 * \param source CommandSource struct used to find all the information needed to make new users
 * \param chan The channel we're processing
 */
void ProcessJoin(CommandSource &source, const Flux::string &chan){
    std::vector<Flux::string> &params = source.params;
    if(params.size() < 7)
      return;
    Flux::string channel = params[1];
    Flux::string Ident = params[2];
    Flux::string Host = params[3];
    Flux::string Server = params[4];
    Flux::string Nickname = params[5];
    Flux::string opstatus = params[6];
    Flux::string realname = params[7].erase(0,2);
    /*******************************************************/
    User *u = finduser(Nickname);
    if(!u){
      if(!Host.empty() || !Nickname.empty() || !Ident.empty())
	u = new User(Nickname, Ident, Host, realname, Server);
    }
    Channel *c = findchannel(channel);
    if(!c){
     if(!channel.empty())
       c = new Channel(channel);
    }
}
/*********************************************************************************/

/** 
 * \fn void process(const Flux::string &buffer)
 * \brief Main Processing function
 * \param buffer The raw socket buffer
 */
void process(const Flux::string &buffer){
  Flux::string buf = buffer;
  buf = buf.replace_all_cs("  ", " ");
  if(buf.empty())
    return;
  Flux::string source;
  if(buf[0] == ':'){
   size_t space = buf.find_first_of(" ");
   if(space == Flux::string::npos)
     return;
   source = buf.substr(1, space - 1);
   buf = buf.substr(space + 1);
   if(source.empty() || buf.empty())
     return;
  }
  sepstream bufferseparator(buf, ' ');
  Flux::string bufferseparator_token;
  Flux::string command = buf;
  if(bufferseparator.GetToken(bufferseparator_token))
    command = bufferseparator_token;
  std::vector<Flux::string> params;
  
  while(bufferseparator.GetToken(bufferseparator_token))
  {
    if(bufferseparator_token[0] == ':'){
      if(!bufferseparator.StreamEnd())
	params.push_back(bufferseparator_token.substr(1)+" "+bufferseparator.GetRemaining());
      else
	params.push_back(bufferseparator_token.substr(1));
      break;
    }
    else
      params.push_back(bufferseparator_token);
  }
  log(LOG_RAWIO, "Received: %s\n", Flux::Sanitize(buffer).c_str());
  if(protocoldebug)
  {
   log(LOG_TERMINAL, "Source: %s\n", source.empty() ? "No Source" : source.c_str());
   log(LOG_TERMINAL, "%s: %s\n", command.is_number_only() ? "Numeric" : "Command", command.c_str());
   if(params.empty())
     log(LOG_TERMINAL, "No Params\n");
   else
     for(unsigned i =0; i < params.size(); ++i)
       log(LOG_TERMINAL, "Params %i: %s\n", i, Flux::Sanitize(params[i]).c_str());
  }
  /**************************************/
  const Flux::string &receiver = params.size() > 0 ? params[0] : "";
  Flux::string message = params.size() > 1? params[1] : "";
  
  /* make local variables instead of global ones */
  Flux::string nickname = isolate(':','!',source),
  uident = isolate('!','@',source),
  uhost = isolate('@',' ',source), cmd;
  
  User *u = finduser(nickname);
  Channel *c = findchannel(receiver);
  
  if(command.is_pos_number_only())
   FOREACH_MOD(I_OnNumeric, OnNumeric(atoi(command.c_str())));
  else
    FOREACH_MOD(I_OnCommand, OnCommand(command, StringVector(message, ' ')));
  
  if(!u && (!nickname.empty() || !uident.empty() || !uhost.empty()) /*&& !nickname.find('.')*/)
    u = new User(nickname, uident, uhost);
  if(command == "PRIVMSG")
  {
    if(!protocoldebug && !receiver.empty() && u && !params[1].empty())
      log(LOG_TERMINAL, "<%s-%s> %s\n", u->nick.c_str(), receiver.c_str(), params[1].c_str());
  }else{
    if(!protocoldebug)
      log(LOG_DEBUG, "%s\n", Flux::Sanitize(buffer).c_str());
  }
  if(command == "QUIT" && u)
    delete u;
  if(command == "PART"){
    if(IsValidChannel(receiver) && c && u && u->nick == Config->BotNick)
     delete c;
    else
     delete u;
  }
  /*if(command == "NICK"){
   if(u && u->nick == Config->BotNick){
       nick = params[0];
       delete u; //we shouldnt be a user in the 1st place (:
  }else if(u->IsOwner())
    owner_nick = params[0];
  else
    delete u; //we delete the user because the above if statement makes a new one for the nick change
   }*/
  if(command == "JOIN"){
    if(!u && (!nickname.empty() || !uident.empty() || !uhost.empty()))
      u = new User(nickname, uident, uhost);
    if(!c && IsValidChannel(receiver))
      c = new Channel(receiver);
    if(u->nick == Config->BotNick)
      c->SendWho();
  }
  if(command == "004" && source.find('.'))
    server_name = source;
  if(command == "PING") //this is here and not in ping_pong.h because some servers require it before connections
      send_cmd("PONG :%s\n", params[0].c_str());
  if(command.equals_ci("ERROR"))
    throw CoreException(buffer.c_str());
  /**************************************/
  CommandSource Source;
  Source.u = u; //User class
  Source.c = c; //Channel class
  Source.command = command;
  Source.message = message;
  Source.params = params;
  Source.raw = buffer;
  Source.raw_source = nickname;
  if(command == "352"){
   ProcessJoin(Source, c->name);
  }
  std::vector<Flux::string> params2 = StringVector(message, ' ');
  if(source.empty() || message.empty() || params2.empty())
    return;
  if(!FindCommand(params2[0]) && source != server_name && command == "PRIVMSG")
  {
    if(!IsValidChannel(receiver))
      Source.Reply("Unknown command \2%s\2", params2[0].c_str());
    else
      static_cast<void>(0); //This will one day be a actual function for channel only messages..
  }
  else{
    Command *com = FindCommand(params2[0]);
    if(com && !IsValidChannel(receiver))
      com->Run(Source, params2);
    else
      static_cast<void>(0);
  }
  command.clear();
}