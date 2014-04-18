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
 * \fn Module::Module(Flux::string n)
 * \brief Module Constructor
 * \param name Name of the Module
 */

Module::Module(const Flux::string &n) : author(""), version(""), loadtime(time(nullptr)), Priority(PRIORITY_DONTCARE), permanent(false), handle(nullptr), name(n), filename(""),  filepath("")
{
	SET_SEGV_LOCATION();

	if(FindModule(this->name))
		throw ModuleException("Module already exists!");

	Modules[this->name] = this;

	if(InTerm())
		Log() << "Loaded Module " << n;
}

/**
 * \fn Module::~Module()
 * \brief Module destructor
 */
Module::~Module()
{
	SET_SEGV_LOCATION();
	Log(LOG_DEBUG) << "Unloading Module " << this->name;
	ModuleHandler::DetachAll(this);
	Modules.erase(this->name);
}

/**
 * \fn void Module::SetAuthor(const Flux::string &person)
 * \brief Sets the Module author, can only be set by the Module its self
 * \param Author the name of the author(s) to be set
 */
void Module::SetAuthor(const Flux::string &person)
{
	this->author = person;
}

/**
 * \fn void Module::SetVersion(const Flux::string &ver)
 * \brief Sets the Module version, can only be set by the Module its self
 * \param Version the Module version
 */
void Module::SetVersion(const Flux::string &ver)
{
	this->version = ver;
}

/**
 * \fn void Module::SetAuthor(const Flux::string &person)
 * \brief Sets the Module priority, can only be set by the Module its self
 * \param Priority sets the priority of the Module
 */
void Module::SetPriority(ModulePriority p)
{
	this->Priority = p;
}

void Module::SetPermanent(bool value)
{
	this->permanent = value;
}
