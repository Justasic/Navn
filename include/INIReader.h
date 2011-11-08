// Read an INI file into easy-to-access name/value pairs.

// inih and INIReader are released under the New BSD license (see LICENSE.txt).
// Go to the project home page for more info:
//
// http://code.google.com/p/inih/
// This INIReader has been modified by Justasic, it is NOT the one on google code!

#ifndef __INIREADER_H__
#define __INIREADER_H__
#include "flux.h"
#include "extern.h"
#include "textfile.h"

// Read an INI file into easy-to-access name/value pairs. (Note that I've gone
// for simplicity here rather than speed, but it should be pretty decent.)
class INIReader
{
public:
    // Construct INIReader and parse given filename. See ini.h for more info
    // about the parsing.
    INIReader(const Flux::string&);

    // Return the result of ini_parse(), i.e., 0 on success, line number of
    // first error on parse error, or -1 on file open error.
    int ParseError();

    // Get a string value from INI file, returning default_value if not found.
    Flux::string Get(const Flux::string&, const Flux::string&, const Flux::string&);

    // Get an integer (long) value from INI file, returning default_value if
    // not found.
    long GetInteger(const Flux::string&, const Flux::string&, long);

    bool GetBoolean(const Flux::string&, const Flux::string&, bool);
    ~INIReader();

private:
    int _error;
    Flux::map<Flux::string> _values;
    static Flux::string MakeKey(const Flux::string&, const Flux::string&);

    // Parse the INI file
    int Parse(const Flux::string &filename);
};
class BotConfig
{
public:
  BotConfig();
  virtual ~BotConfig();
  INIReader *Parser;
  Flux::string LogFile;
  Flux::string Binary_Dir;
  Flux::string Owner;
  Flux::string Realname;
  Flux::string Ident;
  Flux::string BotNick;
  Flux::string Channel;
  Flux::string Port;
  Flux::string Server;
  Flux::string LogChannel;
  Flux::string PidFile;
  Flux::string UserPass;
  Flux::string OperatorAccount;
  Flux::string OperatorPass;
  Flux::string ModuleDir;
  Flux::string Modules;
  Flux::string ServicesAccount;
  Flux::string ServicesPass;
  Flux::string ServicesSendString;
  Flux::string AutoIdentString;
  Flux::string ServicesService;
  bool IdentOnConn;
  size_t SockWait;
private:
  void Read();
};

#endif  // __INIREADER_H__
