/* Navn IRC bot -- Exceptions header
 * 
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */
/**
 *\file  SocketException.h
 *\brief Contains the Exception throw classes.
 */
#pragma once
#ifndef SOCKET_EXCEPTION_H
#define SOCKET_EXCEPTION_H
#include "flux.h"
/** SocketExceptions are to be thrown with anything Socket related
 * Throws Socket Exception
 * @param throw SocketException(message)
 */
class SocketException
{
 public:
  SocketException(Flux::string s):m_s(s) {};
  ~SocketException(){};

  Flux::string description() { return m_s; }
 private:
  Flux::string m_s;
};

/**CoreExceptions are to be thrown with everything else
 * Throws Core Exception
 * @param throw CoreException(message)
 */
/**************************************************
 * Code was taken from the InspIRCd 2.0 Team      *
 * and has been modified to the needs of this bot *
 **************************************************/

class CoreException : public std::exception
{
 public:
	/** Holds the error message to be displayed
	 */
	const Flux::string err;
	/** Source of the exception
	 */
	const Flux::string source;
	/** Default constructor, just uses the error mesage 'Core threw an exception'.
	 */
	CoreException() : err("Core threw an exception"), source("The core") { }
	/** This constructor can be used to specify an error message before throwing.
	 */
	CoreException(const Flux::string &message) : err(message), source("The core") { }
	/** This constructor can be used to specify an error message before throwing,
	 * and to specify the source of the exception.
	 */
	CoreException(const Flux::string &message, const Flux::string &src) : err(message), source(src) { }
	/** This destructor solves world hunger, cancels the world debt, and causes the world to end.
	 * Actually no, it does nothing. Never mind.
	 * @throws Nothing!
	 */
	virtual ~CoreException() throw() { };
	/** Returns the reason for the exception.
	 * The module should probably put something informative here as the user will see this upon failure.
	 */
	virtual const char* GetReason() const
	{
		return err.c_str();
	}

	virtual const char* GetSource()
	{
		return source.c_str();
	}
};

class LogException : public CoreException
{
 public:
		LogException(const Flux::string &message) : CoreException(message, "A Log") { }
		virtual ~LogException() throw() { }
};
class ModuleException : public CoreException
{
 public:
		ModuleException(const Flux::string &message) : CoreException(message, "A Module") { }
		virtual ~ModuleException() throw() { }
};

class ConfigException : public CoreException
{
 public:
 		ConfigException() : CoreException("Config threw an exception", "Config Parser") { }
		ConfigException(const Flux::string &msg) : CoreException(msg, "A Config") { }
		virtual ~ConfigException() throw() { }
};
#endif
