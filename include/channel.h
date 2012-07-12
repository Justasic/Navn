/* Navn IRC bot -- Channel class header
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */
#pragma once
#ifndef CHANNEL_H
#define CHANNEL_H
/**
 * \include ircproto.h
 */
#include "ircproto.h"
/**
 * \file channel.h
 * \brief Contains the Channel class as well as all of it's contents as prototype declarations.
 */
/**
 * \class Channel
 * \brief A wrapper class for IRC Channels.
 * Wraps around IRC Channels and contains their respective properties
 * as well as IRC channel related actions as functions.
 */
/**
 * \var Flux::string name
 * \brief Name of the IRC Channel.
 */
/**
 * \var Flux::string topic
 * \brief Topic of the IRC Channel.
 */
/**
 * \var Flux::string topic_setter
 * \brief Name of the user who last set the topic of the IRC Channel.
 */
/**
 * \var time_t topic_time
 * \brief The time the topic of the IRC Channel was last set.
 */
/**
 * \var time_t creation_time
 * \brief Time the IRC Channel was last created.
 */
typedef std::map<User*, Channel*> UList;
class CoreExport Channel : public Base
{
public:
  Channel(const Flux::string&);
  virtual ~Channel();
  UList UserList;
  User *finduser(const Flux::string&);
  Flux::string name;
  Flux::string modes;
  Flux::string topic;
  Flux::string topic_setter;
  time_t topic_time;
  time_t creation_time;
  void AddUser(User*);
  void DelUser(User*);
  void SendJoin();
  void SendPart(const Flux::string& = "");
  void SendPart(const char*, ...);
  void SendWho();
  void kick(User*, const Flux::string&);
  void kick(User*, const char*, ...);
  void kick(const Flux::string&, const Flux::string&);
  void kick(const Flux::string&, const char*, ...);
  void SetMode(const Flux::string&);
  void SetMode(User *u, const Flux::string &mode);
  void RemoveMode(const Flux::string&);
  void RemoveMode(User *u, const Flux::string &mode);
  void ChangeTopic(const Flux::string&);
  void ChangeTopic(const char *fmt, ...);
  virtual void SendMessage(const Flux::string&);
  void SendMessage(const char*, ...);
  virtual void SendAction(const Flux::string&);
  void SendAction(const char*, ...);
  virtual void SendNotice(const Flux::string&);
  void SendNotice(const char*, ...);
};
#endif

