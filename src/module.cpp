/* Navn IRC bot -- Module Functions
 * 
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */

#include "module.h"
//This code sucks, you know it and I know it. 
//Move on and call me an idiot later.

/** 
 * \fn module::module(Flux::string n)
 * \brief Module Constructor
 * \param name Name of the module
 */
module::module(const Flux::string &n) : name(n), author(""), version(""), handle(NULL), Priority(PRIORITY_DONTCARE), loadtime(time(NULL)), filename(""),  filepath(""), permanent(false)
{
  SET_SEGV_LOCATION();
  
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

void module::SetPermanent(bool value) { this->permanent = value; }

bool module::GetPermanent() { return this->permanent; }
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
