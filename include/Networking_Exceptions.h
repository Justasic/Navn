/*
 * Simple C++ Socket wrapper
 * The following people authored this code:
 *
 * Justin Crawford (Justasic@gmail.com)
 * Piero Barbagelata
 *
 * This project is not licensed and falls under
 * the Creative Commons Public Domain.
 * You are free to use and/or distribute this
 * source code for any project or organization
 * without authors consent.
 *
 */
#ifndef Networking_Exceptions_H
#define Networking_Exceptions_H

#include <iostream>
#include <exception>
// #include "extern.h"
#include "flux.h"

#define ExceptionLocation printfify("%s line %d", __PRETTY_FUNCTION__, __LINE__)
//extern CoreExport Flux::string printfify(const char*, ...);


/**************************************************
 * Code was taken from the InspIRCd 2.0 Team      *
 * and has been modified to the needs of this bot *
 **************************************************/

class CoreException : public std::exception
{
protected:
    /** Holds the type of exception we're throwing
     */
    Flux::string type;
public:
    /** Holds the error message to be displayed
     */
    const Flux::string err;
    /** Source of the exception
     */
    const Flux::string source;
    /** Default constructor, just uses the error mesage 'Core threw an exception'.
     */
    CoreException() : type("Core"), err("Core threw an exception"), source("The core") { }
    /** This constructor can be used to specify an error message before throwing.
     */
    CoreException(const Flux::string &message) : type("Core"), err(message), source("The core") { }
    /** This constructor can be used to specify an error message before throwing,
     * and to specify the source of the exception.
     */
    CoreException(const Flux::string &message, const Flux::string &src) : type("Core"), err(message), source(src) { }
    /** This destructor solves world hunger, cancels the world debt, and causes the world to end.
     * Actually no, it does nothing. Never mind.
     * @throws Nothing!
     */
    virtual ~CoreException() throw() { };
    /** Returns the reason for the exception.
     * The Module should probably put something informative here as the user will see this upon failure.
     */
    virtual const char* GetReason() const
    {
	return err.c_str();
    }

    virtual const char* GetSource() const
    {
	return source.c_str();
    }

    virtual void Print() const
    {
	std::cerr << this->type << "Exception caught:" << std::endl << "Source: " << this->GetSource() << std::endl
	<< "Message: " << this->GetReason() << std::endl;
    }
};

class LogException : public CoreException
{
public:
    LogException(const Flux::string &message) : CoreException(message, "A Log") { this->type = "Log"; }
    virtual ~LogException() throw() { }
};

class ModuleException : public CoreException
{
public:
    ModuleException(const Flux::string &message) : CoreException(message, "A Module") { this->type = "Module"; }
    virtual ~ModuleException() throw() { }
};

class ConfigException : public CoreException
{
public:
    ConfigException() : CoreException("Config threw an exception", "Config Parser") { this->type = "Config"; }
    ConfigException(const Flux::string &msg) : CoreException(msg, "Config Parser") { this->type = "Config"; }
    virtual ~ConfigException() throw() { }
};

class SocketException : public CoreException
{
public:
    SocketException(const Flux::string &msg) : CoreException(msg, "Socket Engine") { this->type = "Socket"; }
    SocketException(const Flux::string &msg, const Flux::string &src) : CoreException(msg, src) { this->type = "Socket"; }
    virtual ~SocketException() throw() { }
};

/*
class NetworkingException : public std::exception
{
private:
    int numeric;
    std::string message;
public:
    NetworkingException(){}

    NetworkingException(int value, const std::string &msg) : numeric(value), message(msg) { }

    NetworkingException(const std::string &msg) : message(msg) { }

    virtual ~NetworkingException() throw() {}

    int Numeric() throw()
    {
	return numeric;
    }

    std::string Message() throw()
    {
	return message;
    }

    virtual const char* what() const throw()
    {
	return message.c_str();
    }
};*/

#endif