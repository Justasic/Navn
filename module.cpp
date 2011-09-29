#include "module.h"
/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
//This code sucks, you know it and I know it. 
//Move on and call me an idiot later.
Flux::insensitive_map<module*> Modules;
std::vector<module *> ModuleHandler::EventHandlers[I_END];
CommandMap Commandsmap;
CommandMap ChanCommandMap;
/** 
 * \fn module::module(Flux::string n, bool a, ModulePriority p)
 * \brief Module Constructor
 * \param name Name of the module
 * \param activated Wether the module is activated or not
 * \param priority The module priority
 */
module::module(const Flux::string &n, ModulePriority p):name(n){
  priority = p;
  loadtime = time(NULL);
  if(FindModule(this->name))
    throw ModuleException("Module already exists!");
  
  Modules[this->name] = this;
  
  log(LOG_NORMAL, "Loaded module %s", this->name.c_str());
}
module::~module(){ 
  ModuleHandler::DetachAll(this);
  Modules.erase(this->name);
}
void module::SetAuthor(const Flux::string &person) { this->author = person; }
void module::SetVersion(const Flux::string &ver) { this->version = ver; }
Flux::string module::GetVersion() { return this->version; }
time_t module::GetLoadTime() { return this->loadtime; }
Flux::string module::GetAuthor() { return this->author; }
/* commands stuff */
/** 
 * \fn int module::AddCommand(Command *c)
 * \brief Adds commands from modules into the bot
 * \param command The command to add
 */
int module::AddCommand(Command *c){
 if(!c)
   return 1;
 std::pair<CommandMap::iterator, bool> it = Commandsmap.insert(std::make_pair(c->name, c));
 if(it.second != true){
   log(LOG_NORMAL, "Command %s already loaded!", c->name.c_str());
   return 2;
 }
 c->mod = this;
 return 0;
}
/** 
 * \fn int module::DelCommand(Command *c)
 * \brief delete commands from modules in the bot
 * \param command The command to add
 */
int module::DelCommand(Command *c){
  if(!c)
    return 1;
  if(!Commandsmap.erase(c->name))
    return 2;
  c->mod = NULL;
  return 0;
}
/** 
 * \fn int module::AddCommand(Command *c)
 * \brief Adds commands from modules into the bot
 * \param command The command to add
 */
int module::AddChanCommand(Command *c){
 if(!c)
   return 1;
 std::pair<CommandMap::iterator, bool> it = ChanCommandMap.insert(std::make_pair(c->name, c));
 if(it.second != true){
   log(LOG_NORMAL, "Command %s already loaded!", c->name.c_str());
   return 2;
 }
 c->mod = this;
 return 0;
}
/** 
 * \fn int module::DelCommand(Command *c)
 * \brief delete commands from modules in the bot
 * \param command The command to add
 */
int module::DelChanCommand(Command *c){
  if(!c)
    return 1;
  if(!ChanCommandMap.erase(c->name))
    return 2;
  c->mod = NULL;
  return 0;
}

/*******************************************************************/
/** 
 * \fn Command *FindCommand(const Flux::string &name)
 * \brief Find a command in the command map
 * \param name A string containing the command name you're looking for
 */
Command *FindCommand(const Flux::string &name){
 if(name.empty())
   return NULL;
 CommandMap::iterator it = Commandsmap.find(name);
 if(it != Commandsmap.end())
   return it->second;
 return NULL;
}
Command *FindChanCommand(const Flux::string &name){
  if(name.empty())
   return NULL;
 CommandMap::iterator it = ChanCommandMap.find(name);
 if(it != ChanCommandMap.end())
   return it->second;
 return NULL;
}
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
static ModErr ModuleCopy(const Flux::string &name, Flux::string &output)
{
  Flux::string input;
  if(Config->ModuleDir.empty())
    input = Config->Binary_Dir + "/" + name + ".so";
  else
    input = Config->Binary_Dir + "/" + Config->ModuleDir + "/" + name + ".so";
  
  struct stat s;
  if (stat(input.c_str(), &s) == -1)
	  return MOD_ERR_NOEXIST;
  else if (!S_ISREG(s.st_mode))
	  return MOD_ERR_NOEXIST;
  std::ifstream source(input.c_str(), std::ios_base::in | std::ios_base::binary);
  if (!source.is_open())
	  return MOD_ERR_NOEXIST;
  
  output = TempFile(output);

  log(LOG_RAWIO, "Runtime module location: %s", output.c_str());
  
  std::ofstream target(output.c_str(), std::ios_base::in | std::ios_base::binary);
  if (!target.is_open())
  {
	  source.close();
	  return MOD_ERR_FILE_IO;
  }

  int want = s.st_size;
  char *buffer = new char[s.st_size];
  while (want > 0 && !source.fail() && !target.fail())
  {
	  source.read(buffer, want);
	  int read_len = source.gcount();

	  target.write(buffer, read_len);
	  want -= read_len;
  }
  delete [] buffer;
  
  source.close();
  target.close();

  return !source.fail() && !target.fail() ? MOD_ERR_OK : MOD_ERR_FILE_IO;
}
/*  This code was found online at http://www.linuxjournal.com/article/3687#comment-26593 */
template<class TYPE> TYPE class_cast(void *symbol)
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
  if(modname.empty())
    return MOD_ERR_PARAMS;
  if(FindModule(modname))
    return MOD_ERR_EXISTS;
  log(LOG_NORMAL,"Attempting to load module [%s]", modname.c_str());
  
  Flux::string mdir = Config->Binary_Dir + "/runtime/"+modname;
  if(modname.find(".so"))
    mdir += ".XXXXXX";
  else
    mdir += ".so.XXXXXX";
  
  ModErr er = ModuleCopy(modname, mdir);
  if(er != MOD_ERR_OK){
    log(LOG_TERMINAL, "Runtime copy error: %s", DecodeModErr(er).c_str());
    return er;
  }
  dlerror();
  
  void *handle = dlopen(mdir.c_str(), RTLD_LAZY);
  const char *err = dlerror();
  if(!handle && err && *err)
  {
   log(LOG_NORMAL, "[%s] %s", modname.c_str(), err);
   remove(modname.c_str());
   return MOD_ERR_NOLOAD;
  }
  dlerror();
  
  module *(*f)() = class_cast<module *(*)()>(dlsym(handle, "ModInit"));
  err = dlerror();
  if(!f && err && *err){
   log(LOG_NORMAL, "No module init function, moving on.");
   dlclose(handle);
   remove(modname.c_str());
   return MOD_ERR_NOLOAD;
  }
  if(!f)
    throw CoreException("Can't find module constructor, yet no moderr?");
  
  module *m;
  try
  {
    m = f(); 
  }
  catch (const ModuleException &e)
  {
   log(LOG_NORMAL,"Error while loading %s: %s", modname.c_str(), e.GetReason());
   return MOD_ERR_EXCEPTION;
  }
  m->filepath = mdir;
  m->filename = modname+".so";
  m->handle = handle;
  FOREACH_MOD(I_OnModuleLoad, OnModuleLoad(m));
  return MOD_ERR_OK;
}
bool ModuleHandler::DeleteModule(module *m)
{
  if (!m || !m->handle)
	  return false;
  
  void *handle = m->handle;
  Flux::string filepath = m->filepath;
  
  log(LOG_DEBUG, "Unloading module %s", m->name.c_str());
  
  dlerror();
  void (*df)(module *m) = class_cast<void (*)(module *)>(dlsym(m->handle, "Moduninit"));
  const char *err = dlerror();
  if (!df || err)
  {
	  log(LOG_DEBUG, "No destroy function found for %s, chancing delete...", m->name.c_str());
	  delete m; /* we just have to chance they haven't overwrote the delete operator then... */
  }
  else
	  df(m); /* Let the module delete it self, just in case */

  if (dlclose(handle))
	  log(LOG_NORMAL, "[%s.so] %s", m->name.c_str(), dlerror());
  if (!filepath.empty())
	  Delete(filepath.c_str());
  
  return true;
}
bool ModuleHandler::Unload(module *m){
  if(!m)
    return false;
  FOREACH_MOD(I_OnModuleUnload, OnModuleUnload(m));
  DeleteModule(m);
  return true;
}
void ModuleHandler::UnloadAll(){
  for(Flux::insensitive_map<module*>::iterator it = Modules.begin(); it != Modules.end(); ++it)
    Unload(it->second);
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
  log(LOG_DEBUG, "Cleaning up runtime directory.");
  Flux::string dirbuf = Config->Binary_Dir + "/runtime";
  DIR *dirp = opendir(dirbuf.c_str());
	if (!dirp)
	{
		log(LOG_DEBUG, "Cannot open directory (%s)", dirbuf.c_str());
		return;
	}
	struct dirent *dp;
	while ((dp = readdir(dirp)))
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
      log(LOG_NORMAL, "ERROR loading module %s: %s", tok.c_str(), DecodeModErr(e).c_str());
  }
}
/******************End Configuration variables********************/