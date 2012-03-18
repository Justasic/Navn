/* Navn IRC bot -- Module Functions
 * 
 * (C) 2011-2012 Flux-Net
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */

#include "module.h"
//This code sucks, you know it and I know it. 
//Move on and call me an idiot later.
Flux::insensitive_map<module*> Modules;
std::vector<module *> ModuleHandler::EventHandlers[I_END];

/** 
 * \fn module::module(Flux::string n)
 * \brief Module Constructor
 * \param name Name of the module
 */
module::module(const Flux::string &n) : name(n)
{
  SET_SEGV_LOCATION();
  
  this->handle = NULL;
  this->Priority = PRIORITY_DONTCARE;
  this->loadtime = time(NULL);
  this->filename = this->filepath = this->author = this->version = "";
  
  if(FindModule(this->name))
    throw ModuleException("Module already exists!");
  
  Modules[this->name] = this;
  if(InTerm())
    Log() << "Loaded module " << n;
}

/**
 * \fn module::~module()
 * \brief Module destructor
 */
module::~module()
{
  SET_SEGV_LOCATION();
  Log(LOG_DEBUG) << "Unloading module " << this->name;
  ModuleHandler::DetachAll(this);
  Modules.erase(this->name);
}

/**
 * \fn void module::SetAuthor(const Flux::string &person)
 * \brief Sets the module author, can only be set by the module its self
 * \param Author the name of the author(s) to be set
 */
void module::SetAuthor(const Flux::string &person) { this->author = person; }

/**
 * \fn void module::SetVersion(const Flux::string &ver)
 * \brief Sets the module version, can only be set by the module its self
 * \param Version the module version
 */
void module::SetVersion(const Flux::string &ver) { this->version = ver; }

/**
 * \fn void module::SetAuthor(const Flux::string &person)
 * \brief Sets the module priority, can only be set by the module its self
 * \param Priority sets the priority of the module
 */
void module::SetPriority(ModulePriority p) { this->Priority = p; }

/**
 * \fn Flux::string module::GetVersion()
 * \brief Gets the string of the modules version
 * \return The modules version
 */
Flux::string module::GetVersion() { return this->version; }

/**
 * \fn time_t module::GetLoadTime()
 * \brief Gets the time the module was loaded
 * \return load time of the module
 */
time_t module::GetLoadTime() { return this->loadtime; }

/**
 * \fn Flux::string module::GetAuthor()
 * \brief Gets the module author(s)
 * \return The modules author string
 */
Flux::string module::GetAuthor() { return this->author; }

/**
 * \fn ModulePriority module::GetPriority()
 * \brief Gets the module priority
 * \return Priority of the module
 */
ModulePriority module::GetPriority() { return this->Priority; }

/** 
 * \fn module *FindModule(const Flux::string &name)
 * \brief Find a module in the module list
 * \param name A string containing the module name you're looking for
 */
module *FindModule(const Flux::string &name)
{
  Flux::insensitive_map<module*>::iterator it = Modules.find(name);
  if(it != Modules.end())
    return it->second;
 return NULL;
}

/** 
 * \fn bool ModuleHandler::Attach(Implementation i, module *mod)
 * \brief Module hook for the FOREACH_MOD events
 * \param Implementation The Implementation of the call list you want your module to have
 * \param Module the module the Implementation is on
 */
bool ModuleHandler::Attach(Implementation i, module *mod)
{
  if(std::find(EventHandlers[i].begin(), EventHandlers[i].end(), mod) != EventHandlers[i].end())
    return false;
  EventHandlers[i].push_back(mod);
  return true;
}

/// \overload void ModuleHandler::Attach(Implementation *i, module *mod, size_t sz)
void ModuleHandler::Attach(Implementation *i, module *mod, size_t sz)
{
 for(size_t n = 0; n < sz; ++n)
   Attach(i[n], mod);
}

/**
 * \fn Flux::string DecodeModErr(ModErr err)
 * \brief Decodes module errors into a useful string to use in user-end functions
 * \param Error The error to decode
 */
Flux::string DecodeModErr(ModErr err)
{
 switch(err)
 {
   case MOD_ERR_OK:
     return "No error (MOD_ERR_OK)";
   case MOD_ERR_MEMORY:
     return "Out of memory (MOD_ERR_MEMORY)";
   case MOD_ERR_PARAMS:
     return "Insufficient parameters (MOD_ERR_PARAMS)";
   case MOD_ERR_EXISTS:
     return "Module Exists (MOD_ERR_EXISTS)";
   case MOD_ERR_NOEXIST:
     return "Module does not exist (MOD_ERR_NOEXIST)";
   case MOD_ERR_NOLOAD:
     return "Module cannot be loaded (MOD_ERR_NOLOAD)";
   case MOD_ERR_UNKNOWN:
     return "Unknown error (MOD_ERR_UNKNOWN)";
   case MOD_ERR_FILE_IO:
     return "File I/O Error (MOD_ERR_FILE_IO)";
   case MOD_ERR_EXCEPTION:
     return "Module Exception caught (MOD_ERR_EXCEPTION)";
   default:
     return "Unknown error code";
 }
}

/*  This code was found online at http://www.linuxjournal.com/article/3687#comment-26593 */
template<class TYPE> TYPE function_cast(void *symbol)
{
    union
    {
        void *symbol;
        TYPE function;
    } cast;
    cast.symbol = symbol;
    return cast.function;
}

/** 
 * \fn bool ModuleHandler::Detach(Implementation i, module *mod)
 * \brief Unhook for the module hook ModuleHandler::Attach()
 * \param Implementation The Implementation of the call list you want your module to detach
 * \param Module the module the Implementation is on
 */
bool ModuleHandler::Detach(Implementation i, module *mod){
  std::vector<module*>::iterator x = std::find(EventHandlers[i].begin(), EventHandlers[i].end(), mod);
  if(x == EventHandlers[i].end())
    return false;
  EventHandlers[i].erase(x);
  return true;
}

/**
 * \fn void ModuleHandler::DetachAll(module *m)
 * \brief Remove all event lists from the module
 * \param Module the module the Implementation is on
 */
void ModuleHandler::DetachAll(module *m)
{
 for(size_t n = I_BEGIN+1; n != I_END; ++n)
   Detach(static_cast<Implementation>(n), m);
}

/**
 * \fn ModErr ModuleHandler::LoadModule(const Flux::string &modname)
 * \brief Load a module into the bot
 * \param Module the module to load
 */
ModErr ModuleHandler::LoadModule(const Flux::string &modname)
{
  SET_SEGV_LOCATION();
  
  if(modname.empty())
    return MOD_ERR_PARAMS;
  if(FindModule(modname))
    return MOD_ERR_EXISTS;
  
  Log() << "Attempting to load module [" << modname << ']';
  
  Flux::string mdir = Config->Binary_Dir + "/runtime/"+ (modname.search(".so")?modname+".XXXXXX":modname+".so.XXXXXX"),
  input = Flux::string(Config->Binary_Dir + "/" + (Config->ModuleDir.empty()?modname:Config->ModuleDir+"/"+modname) + ".so").replace_all_cs("//","/");
  
  TextFile mod(input);
  Flux::string output = TextFile::TempFile(mdir);
  Log(LOG_RAWIO) << "Runtime module location: " << output;
  mod.Copy(output);
  
  if(mod.GetLastError() != FILE_IO_OK){
    Log(LOG_RAWIO) << "Runtime Copy Error: " << mod.DecodeLastError();
    return MOD_ERR_FILE_IO;
  }
  
  dlerror();
  
  void *handle = dlopen(output.c_str(), RTLD_NOW);
  const char *err = dlerror();
  if(!handle && err && *err)
  {
    Log() << '[' << modname << "] " << err;
    return MOD_ERR_NOLOAD;
  }
  
  dlerror();
  module *(*f)(const Flux::string&) = function_cast<module *(*)(const Flux::string&)>(dlsym(handle, "ModInit"));
  err = dlerror();
  
  if(!f && err && *err){
    Log() << "No module init function, moving on.";
    dlclose(handle);
    return MOD_ERR_NOLOAD;
  }
  if(!f)
    throw CoreException("Can't find module constructor, yet no moderr?");
  
  module *m;
  try
  {
    m = f(modname); 
  }
  catch (const ModuleException &e)
  {
    Log() << "Error while loading " << modname << ": " << e.GetReason();
    return MOD_ERR_EXCEPTION;
  }
  
  m->filepath = output;
  m->filename = (modname.search(".so")?modname:modname+".so");
  m->handle = handle;
  m->OnLoad();
  
  FOREACH_MOD(I_OnModuleLoad, OnModuleLoad(m));
  
  return MOD_ERR_OK;
}

/**
 * \fn bool ModuleHandler::DeleteModule(module *m)
 * \brief Delete the module from module lists and unload it from navn completely
 * \param Module the module to be removed
 */
bool ModuleHandler::DeleteModule(module *m)
{
  SET_SEGV_LOCATION();
  if (!m || !m->handle)
	  return false;
  
  void *handle = m->handle;
  Flux::string filepath = m->filepath;
  
  dlerror();
  void (*df)(module*) = function_cast<void (*)(module*)>(dlsym(m->handle, "ModunInit"));
  const char *err = dlerror();
  if (!df && err && *err)
  {
	  Log(LOG_DEBUG) << "No destroy function found for " << m->name << ", chancing delete...";
	  delete m; /* we just have to chance they haven't overwrote the delete operator then... */
  }
  else
	  df(m); /* Let the module delete it self, just in case */
	  
  if(handle)
    if(dlclose(handle))
      Log() << "[" << m->name << ".so] " << dlerror();
    
  if (!filepath.empty())
    Delete(filepath.c_str());
  
  return true;
}

/**
 * \fn bool ModuleHandler::Unload(module *m)
 * \brief Unloads the module safely and announces the module unload event
 * \param Module the module to be unloaded
 */
bool ModuleHandler::Unload(module *m)
{
  if(!m)
    return false;
  FOREACH_MOD(I_OnModuleUnload, OnModuleUnload(m));
  return DeleteModule(m);
}

/**
 * \fn void ModuleHandler::UnloadAll()
 * \brief Safely unloads ALL modules from navn
 */
void ModuleHandler::UnloadAll()
{
#ifdef _CXX11
  for(auto var : Modules)
    Unload(var.second);
#else
  for(Flux::insensitive_map<module*>::iterator it = Modules.begin(); it != Modules.end(); ++it)
    Unload(it->second);
#endif
}

/**
 * \fn Flux::string ModuleHandler::DecodePriority(ModulePriority p)
 * \brief Decodes the module priority to a string used in user-end functions
 * \param Priority The priority to decode
 */
Flux::string ModuleHandler::DecodePriority(ModulePriority p)
{
 switch(p)
 { 
   case PRIORITY_FIRST:
     return "FIRST";
   case PRIORITY_DONTCARE:
     return "DON'T CARE";
   case PRIORITY_LAST:
     return "LAST";
   default:
     return "";
 }
 return "";
}

/**
 * \fn void ModuleHandler::SanitizeRuntime()
 * \brief Deletes all files in the runtime directory.
 */
void ModuleHandler::SanitizeRuntime()
{
  Log(LOG_DEBUG) << "Cleaning up runtime directory.";
  Flux::string dirbuf = Config->Binary_Dir+"/runtime/";
  
  if(!TextFile::IsDirectory(dirbuf))
  {
    if(mkdir(dirbuf.c_str(), getuid()) != 0)
      throw CoreException(printfify("Error making new runtime directory: %s", strerror(errno)));
  }
  else
  {
    Flux::vector files = TextFile::DirectoryListing(dirbuf);
    for(Flux::vector::iterator it = files.begin(); it != files.end(); ++it)
      Delete(Flux::string(dirbuf+(*it)).c_str());
  }
}
/******************Configuration variables***********************/

/**Rehash void
 * \fn void ReadConfig()
 * \deprecated This will be removed soon.
 * This will re-read the config file values when told to do so
 */
void ReadConfig()
{
  sepstream sep(Config->Modules, ',');
  Flux::string tok;
  while(sep.GetToken(tok))
  {
    tok.trim();
    ModErr e = ModuleHandler::LoadModule(tok);
    if(e != MOD_ERR_OK)
      Log() << "ERROR loading module " << tok << ": " << DecodeModErr(e);
  }
}
/******************End Configuration variables********************/