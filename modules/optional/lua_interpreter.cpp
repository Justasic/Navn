/* Navn IRC bot -- DNS lookup tools for IRC
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "modules.h"

// Our exception class so we can throw errors instead of doing some weird handling
class LUAException : public CoreException
{
public:
  LUAException () : CoreException("LUA Script interpreter threw exception", "LUA Module") { }
  LUAException (const Flux::string &msg) : CoreException(msg, "LUA Module") { }
  virtual ~LUAException () throw() { }
};

class LUA_Interpreter : public module
{
public:
  LUA_Interpreter(const Flux::string &Name):module(Name)
  {
//     Implementation i[] = { I_OnNumeric, I_OnPong, I_OnPing };
//     ModuleHandler::Attach(i, this, sizeof(i) / sizeof(Implementation));
    this->SetAuthor("Justasic");
    this->SetVersion(VERSION);
    this->SetPriority(PRIORITY_FIRST);
  }

  void OnLoad()
  {
    Flux::string LUADir = Config->Binary_Dir+Config->luadir;
    if(!TextFile::IsDirectory(LUADir))
      throw ModuleException("Cannot find lua scripts directory for loading! ("+LUADir+")");
    
    try
    {
      Lua_State *L;

      L = luaL_newstate();
      luaL_openlibs(L);

      Flux::vector files = TextFile::DirectoryListing(LUADir);

      for(Flux::vector::iterator it = files.begin(); it != files.end(); ++it)
      {
	Flux::string file = *it, fext;
	fext = file.substr();
      }

      

    } catch (const LUAException &ex)
    {
      Log(LOG_SCRIPT) << "Failed to load lua script: " << ex.GetReason();
    }
  }
};

MODULE_HOOK(LUA_Interpreter)