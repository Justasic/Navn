/* Navn IRC bot -- Incoming IRC Protocol parser
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */

#include "user.h"
#include "module.h"

/**
 * \file  process.cpp
 * \brief Contains IRC buffer processing functions
 * This file has functions which process the raw buffer from the
 * socket engine and then continues to process module events
 * and commands which depend on this processing system.
 * This system also implements the "Parameters system" which
 * allows for strings to be split by spaces and placed into a
 * vector which makes for easy processing
 */
// EVENT_HOOK(command, "PING", I_OnPing, OnPing);
#define EVENT_HOOK(w, x, y, z) \
if(true) \
{ \
  if(w.equals_ci(x)) \
  {\
    FOREACH_MOD(y, z); \
  } \
} \
else \
  static_cast<void>(0);

/**
 * \fn void ProcessJoin(CommandSource &source, const Flux::string &chan)
 * \brief Processes the /who numeric (352), this should only be used in Process() unless its for something special
 * \param source CommandSource struct used to find all the information needed to make new users
 * \param chan The channel we're processing
 */
void ProcessJoin(CommandSource &source, const Flux::string &chan)
{
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
    if(!u)
    {
      if(!Host.empty() || !Nickname.empty() || !Ident.empty())
	u = new User(Nickname, Ident, Host, realname, Server);
    }
    Channel *c = findchannel(channel);
    if(!c)
    {
     if(!channel.empty() && IsValidChannel(channel))
       c = new Channel(channel);
     else
       Log(LOG_DEBUG) << "Invalid channel " << channel << " in processing channels from numeric 352!";
    }
    if(u)
      u->AddChan(c);
    if(c)
      c->AddUser(u);
}
/*********************************************************************************/
/**
 * \fn void ProcessCommand(CommandSource &Source, std::vector<Flux::string> &params2, const Flux::string &receiver, const Flux::string &command)
 * \brief Processes the Command class commands, this should only be used in Process() unless its for something special
 * \param CommandSource CommandSource struct used to find all the information needed to make new users
 * \param std::vector vector of the command string sent to the bot
 * \param Flux::string receiver
 * \param Flux::string Command sent to the bot in raw form
 */
void ProcessCommand(CommandSource &Source, std::vector<Flux::string> &params2,
		    const Flux::string &receiver, const Flux::string &command)
{
  SET_SEGV_LOCATION();
  User *u = Source.u;
  Channel *c = Source.c;

  if(!command.is_pos_number_only()) { FOREACH_MOD(I_OnCommand, OnCommand(command, params2)); }

 if(!FindCommand(params2[0], C_PRIVATE) && command.equals_ci("PRIVMSG"))
  {
    if(!protocoldebug)
      Log(LOG_DEVEL) << '<' << u->nick << '-' << receiver << "> " << Source.params[1];

    if(!IsValidChannel(receiver))
    {
      Source.Reply("Unknown command \2%s\2", Flux::Sanitize(params2[0]).c_str());
      FOREACH_MOD(I_OnPrivmsg, OnPrivmsg(u, params2));
    }
    else
    {
      Command *ccom = FindCommand(params2[0], C_CHANNEL);
      if(ccom)
      {
	Source.command = ccom->name;
	params2.erase(params2.begin());

	while(ccom->MaxParams > 0 && params2.size() > ccom->MaxParams)
	{
	  params2[ccom->MaxParams - 1] += " " + params2[ccom->MaxParams];
	  params2.erase(params2.begin() + ccom->MaxParams);
	}

	if(params2.size() < ccom->MinParams)
	{
	  ccom->OnSyntaxError(Source, !params2.empty() ? params2[params2.size() - 1] : "");
	  return;
	}
#ifdef HAVE_SETJMP_H
	// Yes, i understand this code is VERY VERY bad and can cause VERY BAD
	// stack corruption, but it's still cool to see that it unloads modules
	// if it segfaults and i would like to keep a system like this in place.
	if(setjmp(sigbuf) == 0)
	{
#endif
	  LastRunModule = ccom->mod;
	  ccom->Run(Source, params2);
#ifdef HAVE_SETJMP_H
	}
	else
	{
	  Log() << "Command " << ccom->name << " failed to execute. Stack Restored.";
	  Source.Reply("An internal error has occurred, please contact one of the bots administrators: %s", CondenseString(Config->Owners).c_str());

	  for(unsigned i = 0; i < Config->Owners.size(); ++i)
	  {
	    User *ou = finduser(Config->Owners[i]);
	    if(ou)
	      ou->SendMessage("Module \2%s\2 has crashed! User \2%s\2 was unable to use command \2%s\2", LastRunModule->name.c_str(), Source.u->nick.c_str(), ccom->name.c_str());
	  }
	}
#endif
	LastRunModule = NULL;
      }
      else
      {
	//This will one day be a actual function for channel only messages..
	FOREACH_MOD(I_OnPrivmsgChannel, OnPrivmsgChannel(u, c, params2));
      }
    }
  }
  else
  {
    Command *com = FindCommand(params2[0], C_PRIVATE);
    if(com && !IsValidChannel(receiver) && command == "PRIVMSG")
    {
      Source.command = com->name;
      params2.erase(params2.begin()); //Remove the command from the params

      while(com->MaxParams > 0 && params2.size() > com->MaxParams)
      { // Trim the command separation, then paste the rest unmodified as the last param.
	 params2[com->MaxParams - 1] += " " + params2[com->MaxParams];
	 params2.erase(params2.begin() + com->MaxParams);
      }

      if(params2.size() < com->MinParams)
      { // Insufficent params
	com->OnSyntaxError(Source, !params2.empty() ? params2[params2.size() - 1] : "");
	return;
      }
#ifdef HAVE_SETJMP_H // Module Segmentation fault recovery.
	if(setjmp(sigbuf) == 0)
	{
#endif
	  LastRunModule = com->mod;
	  com->Run(Source, params2);
#ifdef HAVE_SETJMP_H
	}
	else
	{ //Module segfaulted.
	  Log() << "Command " << com->name << " failed to execute. Stack Restored.";
	  Source.Reply("An internal error has occurred, please contact one of the bots administrators: %s", CondenseString(Config->Owners).c_str());

	  for(unsigned i = 0; i < Config->Owners.size(); ++i)
	  {
	    User *ou = finduser(Config->Owners[i]);
	    if(ou)
	      ou->SendMessage("Module \2%s\2 has crashed! User \2%s\2 was unable to use command \2%s\2", LastRunModule->name.c_str(), Source.u->nick.c_str(), com->name.c_str());
	  }
	}
#endif
	LastRunModule = NULL;
    }
    else
    {
      if(!protocoldebug)
	Log(LOG_DEBUG) << Flux::Sanitize(Source.raw); //This receives ALL server commands sent to the bot..
    }
  }
}

/*********************************************************************************/

/**
 * \fn void process(const Flux::string &buffer)
 * \brief Main Processing function.
 * This function takes the raw socket buffer and processes it down to
 * the basic IRC protocol so Navn can interpret it and process its
 * commands and events. This void function is rather large and
 * has a lot of complex functions in it, it may be modified to become
 * simpler to understand in later releases
 * \param buffer The raw socket buffer
 */

// my god, someone PLEASE for the LOVE OF GOD clean this function up!!!
void process(const Flux::string &buffer)
{
  EventResult e;
  FOREACH_RESULT(I_OnPreReceiveMessage, OnPreReceiveMessage(buffer), e);
  if(e != EVENT_CONTINUE)
    return;

  SET_SEGV_LOCATION();
  Flux::string buf = buffer;
  buf = buf.replace_all_cs("  ", " ");

  if(buf.empty())
    return;

  Flux::string source;
  if(buf[0] == ':')
  {
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

  Flux::vector params;

  while(bufferseparator.GetToken(bufferseparator_token))
  {
    if(bufferseparator_token[0] == ':')
    {
      if(!bufferseparator.StreamEnd())
	params.push_back(bufferseparator_token.substr(1)+" "+bufferseparator.GetRemaining());
      else
	params.push_back(bufferseparator_token.substr(1));
      break;
    }
    else
      params.push_back(bufferseparator_token);
  }

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
  const Flux::string receiver = params.size() > 0 ? params[0] : "";
  Flux::string message = params.size() > 1? params[1] : "";
  const Flux::string nickname = source.isolate(':', '!'),
  uident = source.isolate('!', '@'), uhost = source.isolate('@', ' ');
  
  User *u = finduser(nickname);
  Channel *c = findchannel(receiver);
  Flux::vector params2 = ParamitizeString(message, ' ');
  /***********************************************/
  
  if(message[0] == '\1' && message[message.length() -1] == '\1' && !params2[0].equals_cs("\001ACTION"))
  {
    FOREACH_MOD(I_OnCTCP, OnCTCP(nickname, params2));
    return; // Don't allow the rest of the system to process ctcps as it will be caught by the command handler.
  }
  // the nickname hook MUST go before the creation of a new user, or we'll end up with phantom users in the map!
  if(command.equals_ci("NICK") && u)
  {
    FOREACH_MOD(I_OnNickChange, OnNickChange(u, params[0]));
    u->SetNewNick(params[0]);
  }

  if(!u && !finduser(nickname) && (!nickname.empty() || !uident.empty() || !uhost.empty()))
  {
    if(!nickname.search('.'))
      u = new User(nickname, uident, uhost);
  }

  if(command.equals_ci("QUIT"))
  {
    FOREACH_MOD(I_OnQuit, OnQuit(u, params[0]));
    QuitUser(u);
  }

  if(command.equals_ci("PART"))
  {
    FOREACH_MOD(I_OnPart, OnPart(u, c, params[0]));
    if(IsValidChannel(receiver) && c && u && u->nick == Config->BotNick)
     delete c; //This should remove the channel from all users if the bot is parting..
    else
    {
     if(u)
       u->DelChan(c);

     if(c)
       c->DelUser(u);

     if(u && c && !u->findchannel(c->name))
     {
       Log(LOG_TERMINAL) << "Deleted " << u->nick << '|' << c->name << '|' << u->findchannel(c->name);
       delete u;
      }
    }
  }

  if(command.is_pos_number_only())
  {
    FOREACH_MOD(I_OnNumeric, OnNumeric((int)command, params));
  }

  // Various events we need to pay attention to.
  EVENT_HOOK(command, "PING", I_OnPing, OnPing(params));
  EVENT_HOOK(command, "PONG", I_OnPong, OnPong(params));
  EVENT_HOOK(command, "KICK", I_OnKick, OnKick(u, finduser(params[1]), findchannel(params[0]), params[2]));
  EVENT_HOOK(command, "ERROR", I_OnConnectionError, OnConnectionError(buffer));
  EVENT_HOOK(command, "INVITE", I_OnInvite, OnInvite(u, params[1]));

  if(command.equals_ci("NOTICE") && !source.find('.'))
  {
    if(!IsValidChannel(receiver))
    {
      FOREACH_MOD(I_OnNotice, OnNotice(u, params2));
    }
    else
    {
      FOREACH_MOD(I_OnNoticeChannel, OnNoticeChannel(u, c, params2));
    }
  }

  if(command.equals_ci("MODE"))
  {
    if(IsValidChannel(params[0]) && params.size() == 2)
    {
      FOREACH_MOD(I_OnChannelMode, OnChannelMode(u, c, params[1]));
    }
    else if(IsValidChannel(params[0]) && params.size() == 3)
    {
      FOREACH_MOD(I_OnChannelOp, OnChannelOp(u, c, params[1], params[2]));
    }
    else if(params[0] == Config->BotNick)
    {
      FOREACH_MOD(I_OnUserMode, OnUserMode(u, params[0], params[1]));
    }
  }

  if(command.equals_ci("JOIN"))
  {
    if(!u && (!nickname.empty() || !uident.empty() || !uhost.empty()))
      u = new User(nickname, uident, uhost);
    else if(!c && IsValidChannel(receiver))
      c = new Channel(receiver);
    else if(!u->findchannel(c->name))
      u->AddChan(c);
    else if(!c->finduser(u->nick))
      c->AddUser(u);
    else if(u->nick != Config->BotNick)
    {
      FOREACH_MOD(I_OnJoin, OnJoin(u, c));
    }
  }

  /**************************************/
  // Make sure we have valid information being passed to the core commands and modules
  if(!c && !nickname.search('.') && IsValidChannel(receiver) && protocoldebug)
    Log(LOG_WARN) << "No channel being passed to CommandSource???";

  if(!u && !nickname.search('.') && protocoldebug)
    Log(LOG_WARN) << "No User being passed to CommandSource???";
  
  CommandSource Source;
  Source.u = u; //User class
  Source.c = c; //Channel class
  Source.params = params;
  Source.raw = buffer;
  /**************************************/
  
  if(command == "352")
    ProcessJoin(Source, c->name);

  if(source.empty() || message.empty() || params2.empty())
    return;

  if(!c && !u) // Don't bother executing commands if there's nothing to do
    return;

  ProcessCommand(Source, params2, receiver, command);
}
