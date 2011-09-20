/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#include <command.h>
/**
 *\file  command.cpp 
 *\brief Contains the command class.
 */
Commands::Commands(){
}
/**
 * \fn void command::raw(const char *fmt, ...)
 * \brief Sends data straight to the socket engine
 */
void Commands::raw(const char *fmt, ...){
  char buffer[4096] = "";
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  send_cmd(buffer);
  va_end(args);
}
/**
 * \fn void command::kick(Flux::string Channel, Flux::string User, const char *fmt, ...)
 * \brief Handles kick requests
 * \param channel Channel to be kicked from.
 * \param user User to be kicked.
 * \param reason Reason for the kick.
 */
void Commands::kick(const Flux::string &Channel, const Flux::string &User, const char *fmt, ...){
  char buffer[4096] = "";
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  this->kick(Channel, User, Flux::string(buffer));
  va_end(args);
}
/**
 * \fn void command::topic(Flux::string channel, const char *fmt, ...)
 * \brief Sets channel topic.
 */
void Commands::topic(const Flux::string &channel, const char *fmt, ...){
  char buffer[4096] = "";
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  this->topic(channel, Flux::string(buffer));
  va_end(args);
}
/**
 * \fn void Commands::quit(const char *fmt, ...)
 * \brief Handles quitting of irc
 * \param message Quit message
 */
void Commands::quit(const char *fmt, ...){
  char buffer[4096] = "";
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  this->quit(Flux::string(buffer));
  va_end(args);
}
/** 
 * \fn void command::part(Flux::string channel, Flux::string reason)
 * \brief Sends part with message
 * \param channel Channel to part from.
 * \param reason Reason for parting.
 */
void Commands::part(const Flux::string &channel, const char *fmt, ...){
  char buffer[4096] = "";
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  this->part(channel, Flux::string(buffer));
  va_end(args);
}
/**
 * \overload void command::kick(Flux::string channel, Flux::string user, Flux::string reason)
 */
void Commands::kick(const Flux::string &chan, const Flux::string &userstr, const Flux::string &msg){
  this->raw("KICK %s %s :%s\n", chan.c_str(), userstr.c_str(), msg.c_str());
}
/**
 * \overload void Commands::quit(Flux::string message)
 */
void Commands::quit(const Flux::string &message){
 this->raw("QUIT :%s\n", message.c_str());
}
/**
 * \overload void Commands::part(Flux::string channel, Flux::string msg)
 */
void Commands::part(const Flux::string &channel, const Flux::string &msg){
  this->raw("PART %s :%s\n", channel.c_str(), msg.c_str());
}
/**
 * \overload void command::topic(Flux::string channel, Flux::string msg)
 */
void Commands::topic(const Flux::string &chan, const Flux::string &msg){
  this->raw("TOPIC %s :%s\n", chan.c_str(), msg.c_str());
}
/** 
 * \fn void command::nick(Flux::string nick)
 * \brief Sets the bots nickname in IRC.
 * \param nickname A Flux::string with the new nickname.
 */
void Commands::nick(const Flux::string &bnick){
  this->raw("NICK %s\n", bnick.c_str());
}
/**
 * \fn void command::oper(Flux::string oper, Flux::string password)
 * \brief Sends IRC command /oper
 */
void Commands::oper(const Flux::string &username, const Flux::string &password){
  this->raw("OPER %s %s\n", username.c_str(), password.c_str());
}
/** 
 * \fn void command::join(Flux::string chan)
 * \brief Makes the bot join a channel
 * \param stringy_chan A Flux::string with the channel you want to join.
 */
void Commands::join(const Flux::string &dchan){
  this->raw("JOIN %s\n", dchan.c_str());
}
/** 
 * \overload void command::part(Flux::string channel)
 * \brief Parts channel w/o reason.
 * \param channel Channel to part from.
 */
void Commands::part(const Flux::string &fchan){
  this->raw("PART %s\n", fchan.c_str()); 
}
/** 
 * \fn void Commands::who(Flux::string chan)
 * \brief Sends a /who to the channel
 * \param chan A Flux::string with the channel you want to /who.
 */
void Commands::who(const Flux::string &chan){
 this->raw("WHO %s\n", chan.c_str());
}
/** 
 * \fn void Commands::names(Flux::string &chan)
 * \brief Sends /names to the channel
 * \param chan A Flux::string with the channel you want to /names.
 */
void Commands::names(const Flux::string &chan){
 this->raw("NAMES %s\n", chan.c_str());
}
/** 
 * \fn void command::whois(Flux::string Nick)
 * \brief Sends a IRC Whois to Server.
 * \param Nick Nick to query
 */
void Commands::whois(const Flux::string &nickname){
  this->raw("WHOIS %s\n", nickname.c_str());
}
/** 
 * \fn void command::mode(Flux::string nickname, Flux::string mode, Flux::string user)
 * \brief Sends a mode to be set in IRC
 * \param nickname Nickname of who we are setting a more to.
 * \param mode The mode to set.
 */
void Commands::mode(const Flux::string &chan, const Flux::string &usermode, const Flux::string &usernick){
  this->raw("MODE %s %s %s\n", chan.c_str(), usermode.c_str(), usernick.c_str());
}
/** 
 * \fn void Commands::user(Flux::string ident, Flux::string realname)
 * \brief Sends the user gecos to the server
 * \param ident The ident at the beginning of the IRC host.
 * \param realname The real name gecos used in irc.
 */
void Commands::user(const Flux::string &ident, const Flux::string &realname){
 this->raw("USER %s * * :%s\n", ident.c_str(), realname.c_str());
}
/**
 *\overload void command::mode(Flux:;string dest, Flux::string mode)
 * \brief Sends a mode to the server
 * @param dest where to set the mode
 * @param mode mode to set
 */
void Commands::mode(const Flux::string &dest, const Flux::string &chanmode){
  this->raw("MODE %s %s\n", dest.c_str(), chanmode.c_str()); 
}
/***************************************************************************************/
Oper::Oper(){
}
void Oper::raw(const char *fmt, ...){
  char buffer[4096] = "";
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  send_cmd(buffer+'\n');
  va_end(args);
}
void Oper::samode(const Flux::string &target, const Flux::string &mode){
  this->raw("SAMODE %s %s\n", target.c_str(), mode.c_str());
}
void Oper::samode(const Flux::string &target, const Flux::string &mode, const Flux::string &params){
  this->raw("SAMODE %s %s %s", target.c_str(), mode.c_str(), params.c_str());
}
void Oper::sajoin(const Flux::string &target, const Flux::string &channel){
  this->raw("SAJOIN %s %s", target.c_str(), channel.c_str());
}
void Oper::sapart(const Flux::string &target, const Flux::string &channel){
  this->raw("SAPART %s %s", target.c_str(), channel.c_str());
}
void Oper::sanick(const Flux::string &target, const Flux::string &nickname){
  this->raw("SANICK %s %s", target.c_str(), nickname.c_str());
}
void Oper::sakick(const Flux::string &channel, const Flux::string &target, const Flux::string &reason){
  this->raw("SAKICK %s %s %s", channel.c_str(), target.c_str(), reason.c_str());
}
void Oper::satopic(const Flux::string &target, const Flux::string &topic){
  this->raw("SATOPIC %s %s", target.c_str(), topic.c_str());
}
void Oper::satopic(const Flux::string &target, const char *fmt, ...){
  char buffer[4096] = "";
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  this->satopic(target, Flux::string(buffer));
  va_end(args); 
}
void Oper::sahost(const Flux::string &target, const Flux::string &host){
  this->raw("CHGHOST %s %s", target.c_str(), host.c_str());
}
void Oper::saident(const Flux::string &target, const Flux::string &ident){
  this->raw("CHGIDENT %s %s", target.c_str(), ident.c_str());
}
void Oper::kill(const Flux::string &target, const Flux::string &reason){
  this->raw("KILL %s %s", target.c_str(), reason.c_str());
}
void Oper::saname(const Flux::string &target, const Flux::string &name){
  this->raw("CHGNAME %s %s", target.c_str(), name.c_str());
}
void Oper::saname(const Flux::string &target, const char *fmt, ...){
  char buffer[4096] = "";
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  this->saname(target, Flux::string(buffer));
  va_end(args); 
}
void Oper::wallops(const Flux::string &message){
  this->raw("WALLOPS %s", message.c_str());
}
void Oper::wallops(const char *fmt, ...){
  char buffer[4096] = "";
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  this->wallops(Flux::string(buffer));
  va_end(args); 
}
void Oper::globops(const Flux::string &message){
  this->raw("GLOBOPS %s", message.c_str());
}
void Oper::globops(const char *fmt, ...){
  char buffer[4096] = "";
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  this->globops(Flux::string(buffer));
  va_end(args); 
}
void Oper::zline(const Flux::string &ipmask, const Flux::string &time, const Flux::string &reason){
  this->raw("ZLINE %s %s %s", ipmask.c_str(), time.c_str(), reason.c_str());
}
void Oper::qline(const Flux::string &target, const Flux::string &time, const Flux::string &reason){
  this->raw("QLINE %s %s %s", target.c_str(), time.c_str(), reason.c_str());
}
void Oper::kline(const Flux::string &target, const Flux::string &time, const Flux::string &reason){
  this->raw("KLINE %s %s %s", target.c_str(), time.c_str(), reason.c_str());
}
void Oper::gline(const Flux::string &target, const Flux::string &time, const Flux::string &reason){
  this->raw("GLINE %s %s %s", target.c_str(), time.c_str(), reason.c_str());
}
/*******************************************************************************************/
void CommandSource::Reply(const char *fmt, ...){
  va_list args;
  char buf[4096];
  if(fmt){
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    this->Reply(Flux::string(buf));
    va_end(args);
  }
}
void CommandSource::Reply(const Flux::string &msg){
 sepstream sep(msg, '\n');
 Flux::string tok;
 while(sep.GetToken(tok))
 {
   this->u->SendMessage(tok);
 }
}
/*******************************************************************************************/
/* why is this in here with the rest of the commands that send to the server? i dont fucking know lol */
Command::Command(const Flux::string &sname, size_t min_params, size_t max_params): MaxParams(max_params), MinParams(min_params), name(sname)
{
}
Command::~Command()
{
}
void Command::SetDesc(const Flux::string &d){
 this->desc = d; 
}
void Command::SetSyntax(const Flux::string &s){
 this->syntax.push_back(s); 
}
void Command::SendSyntax(CommandSource &source){
 if(!this->syntax.empty()){
  source.Reply("Syntax: \2%s %s\2", source.command.c_str(), this->syntax[0].c_str());
  for(unsigned i=1, j = this->syntax.size(); i < j; ++i)
    source.Reply("        \002%s %s\002", source.command.c_str(), this->syntax[i].c_str());
 }
}
void Command::SendSyntax(CommandSource &source, const Flux::string &syn){
  source.Reply("Syntax: \2%s %s\2", source.command.c_str(), syn.c_str());
  source.Reply("\002/msg %s HELP %s\002 for more information.", nick.c_str(), source.command.c_str());
}
const Flux::string &Command::GetDesc() const{
 return this->desc; 
}
bool Command::OnHelp(CommandSource &source, const Flux::string &subcommand) { return false; }
void Command::OnList(User *u) { }
void Command::Run(CommandSource&, const std::vector<Flux::string>&) { }
void Command::OnSyntaxError(CommandSource &source, const Flux::string &subcommand)
{
 this->SendSyntax(source);
 source.Reply("\002/msg %s HELP %s\002 for more information.", nick.c_str(), source.command.c_str());
}