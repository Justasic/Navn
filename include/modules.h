/* Navn IRC bot -- Module include
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */
#pragma once
#ifndef MODULES_H
#define MODULES_H

#include "module.h"
#include "xmlfile.h"
#include "INIReader.h"
/**
 * \include INIReader.h
 * \include extern.h
 * \include flux.h
 */

/**
 * \file modules.h
 * List the constants used throughout the script.
 * If you wanna edit the server, channel, nick, etc that the bot
 * connects to, do so here, NOT in the main code.
 */
#ifdef HAVE_SETJMP_H
jmp_buf sigbuf;
#endif

// Global variables
char **my_av, **my_envp;
bool nofork, dev, protocoldebug, IsOper, quitting, started, nocolor, istempnick = false;
Flux::string binary_path, bot_bin, binary_dir, quitmsg, server_name, LastBuf;
const Flux::string password = make_pass();
char segv_location[255];
time_t starttime = 0;

// Global Pointers
IRCProto *ircproto;
SocketIO *sock;
BotConfig *Config;
module *LastRunModule;

// Module Global Classes/Functions
class irc_string;
class CommandLineArguments;
E Flux::string getprogdir(const Flux::string&);
E DEPRECATED(Flux::string removeCommand(Flux::string command, Flux::string s));
E DEPRECATED(Flux::string urlify(const Flux::string &received));
E Flux::string execute(const char*);
E void restart(const Flux::string&);
E void Rehash();
E void WritePID();
E void startup(int, char**, char*[]);

#endif // MODULES_H
