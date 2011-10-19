#include "user.h"
#include "module.h"

/** 
 * \fn void ProcessJoin(CommandSource &source, const Flux::string &chan)
 * \brief Processes the /who numeric (352), this should only be used in Process() unless its for something special
 * \param source CommandSource struct used to find all the information needed to make new users
 * \param chan The channel we're processing
 */
void ProcessJoin(CommandSource &source, const Flux::string &chan){
    SET_SEGV_LOCATION();
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
    u->AddChan(c);
}
/*********************************************************************************/
void ProcessCommand(CommandSource &Source, std::vector<Flux::string> &params2,
		    const Flux::string &receiver, const Flux::string &command)
{
  SET_SEGV_LOCATION();
  User *u = Source.u;
  Channel *c = Source.c;
  if(!command.is_pos_number_only()) { FOREACH_MOD(I_OnCommand, OnCommand(command, params2)); }
 if(!FindCommand(params2[0]) && command == "PRIVMSG")
  {
    if(!protocoldebug)
      Log(LOG_TERMINAL) << '<' << u->nick << '-' << receiver << "> " << Source.params[1];
    if(!IsValidChannel(receiver)){
      Source.Reply("Unknown command \2%s\2", Flux::Sanitize(params2[0]).c_str());
      FOREACH_MOD(I_OnPrivmsg, OnPrivmsg(u, params2));
    }
    else{
      Command *ccom = FindChanCommand(params2[0]);
      if(ccom){
	Source.command = ccom->name;
	params2.erase(params2.begin());
	while(ccom->MaxParams > 0 && params2.size() > ccom->MaxParams){
	 params2[ccom->MaxParams - 1] += " " + params2[ccom->MaxParams];
	 params2.erase(params2.begin() + ccom->MaxParams);
	}
	if(params2.size() < ccom->MinParams) { ccom->OnSyntaxError(Source, !params2.empty() ? params2[params2.size() - 1] : ""); return; }
/*#ifdef HAVE_SETJMP_H
	if(setjmp(sigbuf) == 0){
#endif
	ccom->Run(Source, params2);
#ifdef HAVE_SETJMP_H
	}else
	  Log() << "Command " << ccom->name << " Failed to run.";
#endif*/
      TestRun(ccom->Run(Source, params2));
      }else{
	FOREACH_MOD(I_OnPrivmsg, OnPrivmsg(u, c, params2)); //This will one day be a actual function for channel only messages..
      }
    }
  }
  else{
    Command *com = FindCommand(params2[0]);
    if(com && !IsValidChannel(receiver) && command == "PRIVMSG"){
      Source.command = com->name;
      params2.erase(params2.begin());
      while(com->MaxParams > 0 && params2.size() > com->MaxParams){
	 params2[com->MaxParams - 1] += " " + params2[com->MaxParams];
	 params2.erase(params2.begin() + com->MaxParams);
      }
      if(params2.size() < com->MinParams) { com->OnSyntaxError(Source, !params2.empty() ? params2[params2.size() - 1] : ""); return; }
/*#ifdef HAVE_SETJMP_H
	if(setjmp(sigbuf) == 0){
#endif
	com->Run(Source, params2);
#ifdef HAVE_SETJMP_H
	}else
	  Log() << "Command " << ccom->name << " Failed to run.";
#endif*/
    TestRun(com->Run(Source, params2));
    }else{
      if(!protocoldebug)
	Log(LOG_DEBUG) << Flux::Sanitize(Source.raw); //This receives ALL server commands sent to the bot..
    }
  } 
}

/*********************************************************************************/

/** 
 * \fn void process(const Flux::string &buffer)
 * \brief Main Processing function
 * \param buffer The raw socket buffer
 */
void process(const Flux::string &buffer){
  SET_SEGV_LOCATION();
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
  Log(LOG_RAWIO) << "Received: " << Flux::Sanitize(buffer);
  if(protocoldebug)
  {
    Log(LOG_TERMINAL) << "Source: " << (source.empty()?"No Source":source);
    Log(LOG_TERMINAL) << (command.is_number_only()?"Numeric":"Command") << ": " << command;
   if(params.empty())
     Log(LOG_TERMINAL) << "No Params";
   else
     for(unsigned i =0; i < params.size(); ++i)
       Log(LOG_TERMINAL) << "Params " << i << ": " << Flux::Sanitize(params[i]);
  }
  /***********************************************/
  /* make local variables instead of global ones */
  const Flux::string &receiver = params.size() > 0 ? params[0] : "";
  Flux::string message = params.size() > 1? params[1] : "";

  Flux::string nickname = isolate(':','!',source),
  uident = isolate('!','@',source),
  uhost = isolate('@',' ',source), cmd;
  
  User *u = finduser(nickname);
  Channel *c = findchannel(receiver);
  std::vector<Flux::string> params2 = StringVector(message, ' ');
  /***********************************************/
  if(command == "004" && source.search('.')) { server_name = source; }
  if(message[0] == '\1' && message[message.length() -1] == '\1'){
    FOREACH_MOD(I_OnCTCP, OnCTCP(nickname, StringVector(message, ' ')));
    return; //Dont allow the rest of the system to process ctcp's as it will be caught by the command handler.
  }
  if(command.equals_cs("NICK") && u) { FOREACH_MOD(I_OnNickChange, OnNickChange(u, params[0])); u->SetNewNick(params[0]); }
  if(!u && !finduser(nickname) && (!nickname.empty() || !uident.empty() || !uhost.empty())){
    if(!nickname.search('.'))
      u = new User(nickname, uident, uhost);
  }
  if(command == "QUIT" && u){
    FOREACH_MOD(I_OnQuit, OnQuit(u, params[0]));
    delete u;
  }
  if(command == "PART"){
    FOREACH_MOD(I_OnPart, OnPart(u, c, params[0]));
    if(IsValidChannel(receiver) && c && u && u->nick == Config->BotNick)
     delete c;
    else{
     if(!u->FindChan(c))
       delete u;
     else
       static_cast<void>(0);
       //u->DelChan(c);
    }
  }
  if(command.is_pos_number_only()) { FOREACH_MOD(I_OnNumeric, OnNumeric((int)command)); }
  if(command.equals_cs("KICK")){ FOREACH_MOD(I_OnKick, OnKick(u, finduser(params[1]), findchannel(params[0]), params[2])); }
  if(command.equals_ci("ERROR")) { FOREACH_MOD(I_OnConnectionError, OnConnectionError(buffer)); }
  if(command.equals_cs("INVITE")) { FOREACH_MOD(I_OnInvite, OnInvite(u, params[1])); }
  if(command.equals_cs("NOTICE") && !source.find('.')){
    if(!IsValidChannel(receiver)) { FOREACH_MOD(I_OnNotice, OnNotice(u, params2)); } 
    else { FOREACH_MOD(I_OnNotice, OnNotice(u, c, params2)); }
  }
  if(command.equals_cs("MODE")) {
    if(IsValidChannel(params[0]) && params.size() == 2) { FOREACH_MOD(I_OnChannelMode, OnChannelMode(u, c, params[1])); }
    else if(IsValidChannel(params[0]) && params.size() == 3) { FOREACH_MOD(I_OnChannelOp, OnChannelOp(u, c, params[1], params[2])); }
    else if(params[0] == Config->BotNick) { FOREACH_MOD(I_OnUserMode, OnUserMode(u, params[0], params[1])); }
  }
  if(command == "JOIN"){
    if(!u && (!nickname.empty() || !uident.empty() || !uhost.empty()))
      u = new User(nickname, uident, uhost);
    else if(!c && IsValidChannel(receiver))
      c = new Channel(receiver);
    else if(u->nick == Config->BotNick)
      c->SendWho();
    else if(u->nick != Config->BotNick){
      FOREACH_MOD(I_OnJoin, OnJoin(u, c));
    }
  }
  /**************************************/
  CommandSource Source;
  Source.u = u; //User class
  Source.c = c; //Channel class
  Source.message = message;
  Source.params = params;
  Source.raw = buffer;
  Source.raw_source = nickname;
  /**************************************/
  if(command == "352"){ ProcessJoin(Source, c->name); }
  if(source.empty() || message.empty() || params2.empty())
    return;
  ProcessCommand(Source, params2, receiver, command);
  command.clear();
}