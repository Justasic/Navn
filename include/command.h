/* Navn IRC bot -- Command class header
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */
#pragma once
#ifndef command_h
#define command_h

///\include includes.h
#include "includes.h"
///\include user.h
#include "user.h"
///\include module.h
/**
 *\file  command.h
 *\brief Contains the classes Command, and Oper.
 */

/**
 * \class Oper
 * \brief A wrapper class for IRC Oper commands.
 * Contains IRC operator commands.
 */
class CoreExport Oper
{
public:
  Oper();
  void samode(const Flux::string&, const Flux::string&);
  void samode(const Flux::string&, const Flux::string&, const Flux::string&);
  void sajoin(const Flux::string&, const Flux::string&);
  void sapart(const Flux::string&, const Flux::string&);
  void sanick(const Flux::string&, const Flux::string&);
  void sakick(const Flux::string&, const Flux::string&, const Flux::string&);
  void satopic(const Flux::string&, const Flux::string&);
  void satopic(const Flux::string&, const char *fmt, ...);
  void sahost(const Flux::string&, const Flux::string&);
  void saident(const Flux::string&, const Flux::string&);
  void kill(const Flux::string&, const Flux::string&);
  void saname(const Flux::string&, const Flux::string&);
  void saname(const Flux::string&, const char *fmt, ...);
  void wallops(const Flux::string&);
  void wallops(const char *fmt, ...);
  void globops(const char *fmt, ...);
  void globops(const Flux::string&);
  void zline(const Flux::string&, const Flux::string&, const Flux::string&);
  void qline(const Flux::string&, const Flux::string&, const Flux::string&);
  void kline(const Flux::string&, const Flux::string&, const Flux::string&);
  void gline(const Flux::string&, const Flux::string&, const Flux::string&);
};

/**
 * \class IsoHost
 * \brief Wrapper for an irc host
 * This was written by Justasic to break up the parts of a messages host for easier use.
 */
class CoreExport IsoHost:Flux::string
{
public:
  IsoHost(const Flux::string&);
  Flux::string raw;
  Flux::string nick;
  Flux::string host;
  Flux::string ident;
};

/**
 * \struct CommandSource "user.h" USER_H
 * \brief A wrapper class for the source of the Command
 * Contains information about where the command came from, as well as a Reply() function to quickly send a PrivMessage to the Command sender.
 */
struct CoreExport CommandSource
{
  User *u;
  Channel *c;
  Flux::string command;
  Flux::string raw;
  std::vector<Flux::string> params;

  void Reply(const char *fmt, ...);
  void Reply(const Flux::string&);
};

/**
 * \class Command
 * \brief A wrapper class for Module commands
 * Contains methods and properties for handling/getting information from module commands.
 */
class CoreExport Command : public Base
{
  Flux::string desc;
  std::vector<Flux::string> syntax;
  CommandType type;
public:
  size_t MaxParams;
  size_t MinParams;
  Flux::string name;
  module *mod;
  Command(module*, const Flux::string &sname, CommandType, size_t min_params=0, size_t max_params=0);
  virtual ~Command();
protected:
  void SetDesc(const Flux::string&);
  void SetSyntax(const Flux::string&);
  void SendSyntax(CommandSource&);
  void SendSyntax(CommandSource&, const Flux::string&);
public:
  const Flux::string &GetDesc() const;
  const CommandType GetType() const;
  virtual void Run(CommandSource&, const std::vector<Flux::string> &params);
  virtual bool OnHelp(CommandSource&, const Flux::string&);
  virtual void OnList(User *u);
  virtual void OnSyntaxError(CommandSource&, const Flux::string&);
};
#endif
