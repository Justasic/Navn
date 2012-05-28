/* Navn IRC bot -- Logging header
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */
#pragma once
#ifndef LOG_H
#define LOG_H
#include "command.h"
#include "user.h"

class CoreExport Log
{
protected:
  std::fstream log;
public:
  LogType type;
  Flux::string filename;
  User *u;
  Command *c;
  std::stringstream buffer, logstream;
  static Flux::string TimeStamp();
  explicit Log(LogType type = LOG_NORMAL);
  explicit Log(User*);
  Log(LogType, User*);
  Log(User*, Command*);
  Log(LogType, User*, Command*);
  ~Log();

  template<typename T> Log &operator<<(T val)
  {
   this->buffer << val;
   return *this;
  }
};
#endif
