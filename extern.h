#ifndef EXTERN_H
#define EXTERN_H
#include "flux.h"
/* This file is NOT for functions, it is for defining stuff like classes */

/* Classes */
class Channel;
class User;
class SocketIO;
class Commands;
class Command;
class Oper;
class module;
struct CommandSource;

/* * type definitions */
Channel *findchannel(const Flux::string&);
User *finduser(const Flux::string &nick);
//Command *FindCommand(const Flux::string &name);

/* extern's */
extern SocketIO *sock;
extern SendMessage *Send;
extern Flux::string nick;
extern bool IsValidChannel(const Flux::string&);
extern Flux::string logfile;
extern bool protocoldebug, IsOper;
extern std::vector<Flux::string> StringVector(const Flux::string&, char);

/* void's */
void ListChans(CommandSource &source);
void send_cmd(const char *fmt, ...);
void log(const char *fmt, ...);
void ProcessModules(CommandSource&, std::vector<Flux::string>&);

#endif