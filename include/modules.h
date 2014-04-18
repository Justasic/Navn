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

#include "module.h"
#include "xmlfile.h"
#include "INIReader.h"

// Module Global Classes/Functions
class irc_string;
class CommandLineArguments;
E DEPRECATED(Flux::string removeCommand(Flux::string command, Flux::string s));
E DEPRECATED(Flux::string urlify(const Flux::string &received));
E void WritePID();
E void startup(int, char **, char*[]);
