/* Navn IRC bot -- Module Functions
 * 
 * (C) 2011-2012 Flux-Net
 * Contact us at Dev@Flux-Net.net
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
 * \param activated Wether the module is activated or not
 * \param priority The module priority
 */

/* Flux::string author, version;
 * time_t loadtime;
 * ModulePriority Priority;
 * void *handle;
 * Flux::string name, filename, filepath;
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
  if(!nofork)
    Log() << "Loaded module " << n;
}
module::~module(){
  SET_SEGV_LOCATION();
  Log(LOG_DEBUG) << "Unloading module " << this->name;
  ModuleHandler::DetachAll(this);
  Modules.erase(this->name);
}
void module::SetAuthor(const Flux::string &person) { this->author = person; }
void module::SetVersion(const Flux::string &ver) { this->version = ver; }
void module::SetPriority(ModulePriority p) { this->Priority = p; }
Flux::string module::GetVersion() { return this->version; }
time_t module::GetLoadTime() { return this->loadtime; }
Flux::string module::GetAuthor() { return this->author; }
ModulePriority module::GetPriority() { return this->Priority; }

/*******************************************************************/
/** 
 * \fn module *FindModule(const Flux::string &name)
 * \brief Find a module in the module list
 * \param name A string containing the module name you're looking for
 */
module *FindModule(const Flux::string &name){
  Flux::insensitive_map<module*>::iterator it = Modules.find(name);
  if(it != Modules.end())
    return it->second;
 return NULL;
}
/*******************************************************************/

/** 
 * \fn bool ModuleHandler::Attach(Implementation i, module *mod)
 * \brief Module hook for the FOREACH_MOD macro
 * \param Implementation The Implementation of the call list you want your module to have
 * \param Module the module the Implementation is on
 */
bool ModuleHandler::Attach(Implementation i, module *mod){
  if(std::find(EventHandlers[i].begin(), EventHandlers[i].end(), mod) != EventHandlers[i].end())
    return false;
  EventHandlers[i].push_back(mod);
  return true;
}
void ModuleHandler::Attach(Implementation *i, module *mod, size_t sz)
{
 for(size_t n = 0; n < sz; ++n)
   Attach(i[n], mod);
}
Flux::string DecodeModErr(ModErr err){
 switch(err){
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
void ModuleHandler::DetachAll(module *m)
{
 for(size_t n = I_BEGIN+1; n != I_END; ++n)
   Detach(static_cast<Implementation>(n), m);
}
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

bool ModuleHandler::Unload(module *m){
  if(!m)
    return false;
  FOREACH_MOD(I_OnModuleUnload, OnModuleUnload(m));
  return DeleteModule(m);
}
void ModuleHandler::UnloadAll(){
#ifdef _CXX11
  for(auto var : Modules)
    Unload(var.second);
#else
  for(Flux::insensitive_map<module*>::iterator it = Modules.begin(); it != Modules.end(); ++it)
    Unload(it->second);
#endif
}

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
void ModuleHandler::SanitizeRuntime()
{
  Log(LOG_DEBUG) << "Cleaning up runtime directory.";
  Flux::string dirbuf = Config->Binary_Dir+"/runtime/";
  
  if(!TextFile::IsDirectory(dirbuf))
    if(mkdir(dirbuf.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
      Log() << "Error making new runtime directory: " << strerror(errno);
    
  DIR *dirp = opendir(dirbuf.c_str());
  if (!dirp)
  {
	  Log(LOG_DEBUG) << "Cannot open directory (" << dirbuf << ')';
	  return;
  }
  
  for(dirent *dp; (dp = readdir(dirp));)
  {
	  if (!dp->d_ino)
		  continue;
	  if (Flux::string(dp->d_name).equals_cs(".") || Flux::string(dp->d_name).equals_cs(".."))
		  continue;
	  Flux::string filebuf = dirbuf + "/" + dp->d_name;
	  Delete(filebuf.c_str());
  }
  
  closedir(dirp);
}
/******************Configuration variables***********************/
/**Rehash void
 * \fn void ReadConfig()
 * This will re-read the config file values when told to do so
 */
void ReadConfig(){
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