#ifndef REPLY_H
#define REPLY_H
#include "includes.h"
#include <sstream>
template<typename T> inline Flux::string stringify(const T &x){
	std::ostringstream stream;
	if(!(stream << x))
		throw;
	return stream.str();
}
class irc_string:Flux::string{
  /** \class irc_string
 * NOTE this MUST be included in the main file.
 * This class wraps around a Flux::string (usually revieved from the irc server)
 * and attempts to decode all the info recieved from the server
 * so as to make it easier to read different kinds of information
 * (ei. who said something, their host, username, etc.)
 * \param toks Private vector that will contain the words in \a message.
 * \param raw_Flux::string Will hold the exact Flux::string that was recieved from the server.
 * \param usernick Will hold the usernick of the person who sent the message.
 * \param host Will hold the host of the person who sent the message.
 * \param user Will hold the user of the person who sent the message.
 * \param channel Will hold the channel of where the message was said.
 * \param message Will hold the message (stripped of irc protocol stuff)
 */
  private:
    std::vector<Flux::string> toks;
  public:
    irc_string(Flux::string);
    Flux::string raw_string;
    Flux::string usernick; 
    Flux::string host; 
    Flux::string user; 
    Flux::string channel; 
    Flux::string message; 
    Flux::string params(unsigned);
    bool isnumeric(int);
    Flux::string params(unsigned, unsigned);
    static Flux::string isolate(char, char, Flux::string);
    bool said(Flux::string findee);
    static bool said(Flux::string, Flux::string);
};
/**
 * \class IsoHost
 * \brief Wrapper for an irc host
 * This was written by Justasic to break up the parts of a messages host for easier use.
 */
class IsoHost:Flux::string{
public:
    Flux::string raw;
    Flux::string nick;
    Flux::string host;
    Flux::string user;
    IsoHost(Flux::string);
    /**
     * \fn IsoHost(Flux::string fullhost)
     * \param fullhost A Flux::string containing the full host of an irc message
     */
};
#endif