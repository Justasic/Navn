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
  LUAException (const Flux::string &msg, const Flux::string &src) : CoreException(msg, src) { }
  virtual ~LUAException () throw() { }
};

class LUA_Interpreter : public module
{
  std::vector<Lua_State*> Lua_States;
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
	fext = file.substr((file.rfind('.') != Flux::string::npos));
	Log(LOG_TERMINAL) << "EXT: " << fext;
	if(fext.equals_ci("LUA"))
	{
	  if(luaL_loadfile(L, file.c_str()))
	    throw LUAException("Cannot load "+file, lua_tostring(L, -1));
	  if(lua_pcall(L, 0, 0, 0))
	    throw LUAException("Failed to run lua_pcall()", lua_tostring(L, -1));

	  lua_getglobal(L, "LUAHook");

	  Log(LOG_SCRIPT) << "Pre-Call of " << file;
	  if(lua_pcall(L, 0, 0, 0))
	    throw LUAException("Failed to run lua_pcall()", lua_tostring(L, -1));
	}
      }

      lua_close(L);
    }
    catch (const LUAException &ex)
    {
      Log(LOG_SCRIPT) << "Failed to load lua script: " << ex.GetReason() << " | " << ex.GetSource();
    }
  }
};

MODULE_HOOK(LUA_Interpreter)