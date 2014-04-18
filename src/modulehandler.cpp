/* Navn IRC bot -- Module Handler Functions
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */

#include "module.h"

Flux::insensitive_map<Module *> Modules;
std::vector<Module *> ModuleHandler::EventHandlers[I_END];

/**
 * \fn Module *FindModule(const Flux::string &name)
 * \brief Find a Module in the Module list
 * \param name A string containing the Module name you're looking for
 */
Module *FindModule(const Flux::string &name)
{
	auto it = Modules.find(name);

	if(it != Modules.end())
		return it->second;

	return nullptr;
}

/**
 * \fn bool ModuleHandler::Attach(Implementation i, Module *mod)
 * \brief module.hook for the FOREACH_MOD events
 * \param Implementation The Implementation of the call list you want your Module to have
 * \param Module the Module the Implementation is on
 */
bool ModuleHandler::Attach(Implementation i, Module *mod)
{
	if(std::find(EventHandlers[i].begin(), EventHandlers[i].end(), mod) != EventHandlers[i].end())
		return false;

	EventHandlers[i].push_back(mod);
	return true;
}

/// \overload void ModuleHandler::Attach(Implementation *i, Module *mod, size_t sz)
void ModuleHandler::Attach(Implementation *i, Module *mod, size_t sz)
{
	for(size_t n = 0; n < sz; ++n)
		Attach(i[n], mod);
}

/**
 * \fn Flux::string DecodeModErr(ModErr err)
 * \brief Decodes Module errors into a useful string to use in user-end functions
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
 * \fn bool ModuleHandler::Detach(Implementation i, Module *mod)
 * \brief Unhook for the module.hook ModuleHandler::Attach()
 * \param Implementation The Implementation of the call list you want your Module to detach
 * \param Module the Module the Implementation is on
 */
bool ModuleHandler::Detach(Implementation i, Module *mod)
{
	auto x = std::find(EventHandlers[i].begin(), EventHandlers[i].end(), mod);

	if(x == EventHandlers[i].end())
		return false;

	EventHandlers[i].erase(x);
	return true;
}

/**
 * \fn void ModuleHandler::DetachAll(Module *m)
 * \brief Remove all event lists from the Module
 * \param Module the Module the Implementation is on
 */
void ModuleHandler::DetachAll(Module *m)
{
	for(size_t n = I_BEGIN + 1; n != I_END; ++n)
		Detach(static_cast<Implementation>(n), m);
}

/**
 * \fn ModErr ModuleHandler::LoadModule(const Flux::string &modname)
 * \brief Load a Module into the bot
 * \param Module the Module to load
 */
ModErr ModuleHandler::LoadModule(const Flux::string &modname)
{
	SET_SEGV_LOCATION();

	if(modname.empty())
		return MOD_ERR_PARAMS;

	if(FindModule(modname))
		return MOD_ERR_EXISTS;

	Log() << "Attempting to load Module [" << modname << ']';

	Flux::string mdir = binary_dir + "/runtime/" + (modname.search(".so") ? modname + ".XXXXXX" : modname + ".so.XXXXXX");
	Flux::string input = Flux::string(binary_dir + "/" + (Config->ModuleDir.empty() ? modname : Config->ModuleDir + "/" + modname) + ".so").replace_all_cs("//", "/");

	TextFile mod(input);
	Flux::string output = TextFile::TempFile(mdir);
	Log(LOG_RAWIO) << "Runtime Module location: " << output;
	mod.Copy(output);

	if(mod.GetLastError() != FILE_IO_OK)
	{
		Log(LOG_RAWIO) << "Runtime Copy Error: " << mod.DecodeLastError();
		return MOD_ERR_FILE_IO;
	}

	dlerror();

	// FIXME: Somehow the binary_dir variable is lost when this executes >:|
	void *handle = dlopen(output.c_str(), RTLD_LAZY | RTLD_LOCAL);
	const char *err = dlerror();

	if(!handle && err && *err)
	{
		Log() << '[' << modname << "] " << err;
		return MOD_ERR_NOLOAD;
	}

	dlerror();
	Module *(*f)(const Flux::string &) = function_cast<Module * ( *)(const Flux::string &)>(dlsym(handle, "ModInit"));
	err = dlerror();

	if(!f && err && *err)
	{
		Log() << "No Module init function, moving on.";
		dlclose(handle);
		return MOD_ERR_NOLOAD;
	}

	if(!f)
		throw CoreException("Can't find Module constructor, yet no moderr?");

	Module *m;

	try
	{
		m = f(modname);
	}
	catch(const ModuleException &e)
	{
		Log() << "Error while loading " << modname << ": " << e.GetReason();
		return MOD_ERR_EXCEPTION;
	}

	m->filepath = output;
	m->filename = (modname.search(".so") ? modname : modname + ".so");
	m->handle = handle;

	FOREACH_MOD(OnModuleLoad, m);

	return MOD_ERR_OK;
}

/**
 * \fn bool ModuleHandler::DeleteModule(Module *m)
 * \brief Delete the Module from Module lists and unload it from navn completely
 * \param Module the Module to be removed
 */
bool ModuleHandler::DeleteModule(Module *m)
{
	SET_SEGV_LOCATION();

	if(!m || !m->handle)
		return false;

	void *handle = m->handle;
	Flux::string filepath = m->filepath;

	dlerror();
	void (*df)(Module *) = function_cast<void ( *)(Module *)>(dlsym(m->handle, "ModunInit"));
	const char *err = dlerror();

	if(!df && err && *err)
	{
		Log(LOG_WARN) << "No destroy function found for " << m->name << ", chancing delete...";
		delete m; /* we just have to chance they haven't overwrote the delete operator then... */
	}
	else
		df(m); /* Let the Module delete it self, just in case */


	if(handle)
		if(dlclose(handle))
			Log() << "[" << m->name << ".so] " << dlerror();

	if(!filepath.empty())
		Delete(filepath.c_str());

	return true;
}

/**
 * \fn bool ModuleHandler::Unload(Module *m)
 * \brief Unloads the Module safely and announces the Module unload event
 * \param Module the Module to be unloaded
 */
bool ModuleHandler::Unload(Module *m)
{
	if(!m || m->GetPermanent())
		return false;

	FOREACH_MOD(OnModuleUnload, m);
	return DeleteModule(m);
}

/**
 * \fn void ModuleHandler::UnloadAll()
 * \brief Safely unloads ALL Modules from navn
 */
void ModuleHandler::UnloadAll()
{
	for(auto it = Modules.begin(), it_end = Modules.end(); it != it_end;)
	{
		Module *m = it->second;
		++it;
		FOREACH_MOD(OnModuleUnload, m);
		DeleteModule(m);
	}

	Modules.clear();
}

/**
 * \fn Flux::string ModuleHandler::DecodePriority(ModulePriority p)
 * \brief Decodes the Module priority to a string used in user-end functions
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
	Flux::string dirbuf = binary_dir + "/runtime/";

	if(!TextFile::IsDirectory(dirbuf))
	{
#ifndef _WIN32

		if(mkdir(dirbuf.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0)
			throw CoreException(printfify("Error making new runtime directory: %s", strerror(errno)));

#else

		if(!CreateDirectory(dirbuf.c_str(), NULL))
			throw CoreException(printfify("Error making runtime new directory: %s", strerror(errno)));

#endif
	}
	else
	{
		Flux::vector files = TextFile::DirectoryListing(dirbuf);

		for(auto & file : files)
			Delete(Flux::string(dirbuf + (file)).c_str());
	}
}
/******************Configuration variables***********************/

/**Rehash void
 * \fn void ReadConfig()
 * \deprecated This will be removed soon.
 * This will re-read the config file values when told to do so
 */
void ModuleHandler::LoadModuleList(const Flux::vector &list)
{
	for(const auto & elem : list)
	{
		ModErr e = LoadModule(elem);

		if(e != MOD_ERR_OK)
			Log() << "Error loading Module " << elem << ": " << DecodeModErr(e);
	}

//   sepstream sep(Config->Modules, ',');
//   Flux::string tok;
//
//   while(sep.GetToken(tok))
//   {
//     tok.trim();
//     ModErr e = ModuleHandler::LoadModule(tok);
//     if(e != MOD_ERR_OK)
//       Log() << "ERROR loading Module " << tok << ": " << DecodeModErr(e);
//   }
}
/******************End Configuration variables********************/
