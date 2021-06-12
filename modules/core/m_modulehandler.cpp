/* Navn IRC bot -- module.handling commands
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */

#include "modules.h"

class CommandMList : public Command
{
public:
	CommandMList(Module *m): Command(m, "MODLIST", C_PRIVATE, 0, 1)
	{
		this->SetDesc("Lists all loaded Modules");
		this->SetSyntax("\037priority\037");
	}

	void Run(CommandSource &source, const Flux::vector &params) override
	{
		const Flux::string priority = params.size() == 2 ? params[1] : "";
		int c = 0;

		if(priority.empty())
		{
			for(auto & Modules_it : Modules)
			{
				source.Reply("\2%-16s\2 %s [%s]", Modules_it.second->name.c_str(), Modules_it.second->GetAuthor().c_str(),
				             ModuleHandler::DecodePriority(Modules_it.second->GetPriority()).c_str());
				++c;
			}
		}
		else
		{
			// There is probably a WAY easier way of doing this but whatever
			for(auto & Modules_it : Modules)
			{
				if(priority.equals_ci("LAST") || priority == '1')
				{
					source.Reply("\2%-16s\2 %s [%s]", Modules_it.second->name.c_str(), Modules_it.second->GetAuthor().c_str(),
					             ModuleHandler::DecodePriority(Modules_it.second->GetPriority()).c_str());
					++c;
				}
				else if(priority.equals_ci("NORMAL") || priority == '2')
				{
					source.Reply("\2%-16s\2 %s [%s]", Modules_it.second->name.c_str(), Modules_it.second->GetAuthor().c_str(),
					             ModuleHandler::DecodePriority(Modules_it.second->GetPriority()).c_str());
					++c;
				}
				else if(priority.equals_ci("FIRST") || priority == '3')
				{
					source.Reply("\2%-16s\2 %s [%s]", Modules_it.second->name.c_str(), Modules_it.second->GetAuthor().c_str(),
					             ModuleHandler::DecodePriority(Modules_it.second->GetPriority()).c_str());
					++c;
				}
			}
		}

		source.Reply("Total of \2%i\2 Modules", c);
		Log(source.u, this) << "to list all Module" << (priority.empty() ? "" : " with priority " + priority);
	}

	bool OnHelp(CommandSource &source, const Flux::string&) override
	{
		this->SendSyntax(source);
		source.Reply(" ");
		source.Reply("This command displays a list of all Modules\n"
		             "or Modules by priority and displays who created\n"
		             "the Modules and what priority it has in the bot");
		return true;
	}
};

class CommandMLoad : public Command
{
public:
	CommandMLoad(Module *m): Command(m, "MODLOAD", C_PRIVATE, 1, 1)
	{
		this->SetDesc("Load a Module");
		this->SetSyntax("\37name\37");
	}

	void Run(CommandSource &source, const Flux::vector &params) override
	{
		const Flux::string Module = params[1];

		if(Module.empty())
			this->SendSyntax(source);
		else if(!source.u->IsOwner())
			source.Reply(ACCESS_DENIED);
		else
		{
			ModErr e = ModuleHandler::LoadModule(Module);

			if(e != MOD_ERR_OK)
			{
				source.Reply("Failed to load Module %s: %s", Module.c_str(), DecodeModErr(e).c_str());
				Log(source.u, this) << "to load " << Module << " and failed: " << DecodeModErr(e);
			}
			else
			{
				source.Reply("Module \2%s\2 loaded successfully", Module.c_str());
				Log(source.u, this) << "to load " << Module;
			}
		}
	}

	bool OnHelp(CommandSource &source, const Flux::string&) override
	{
		this->SendSyntax(source);
		source.Reply(" ");
		source.Reply("This command loads a Module into the bot.\n"
		             "If the Module fails to load, it will print\n"
		             "an error message, if it crashes, the bot will\n"
		             "automatically notify the owner and unload that Module\n"
		             "Note: You must be the bots owner to use this command");
		return true;
	}
};

class CommandMUnload : public Command
{
public:
	CommandMUnload(Module *m): Command(m, "MODUNLOAD", C_PRIVATE, 1, 1)
	{
		this->SetDesc("Unloads a Module");
		this->SetSyntax("\37name\37");
	}

	void Run(CommandSource &source, const Flux::vector &params) override
	{
		const Flux::string Modulestr = params[1];

		if(!source.u->IsOwner())
			source.Reply(ACCESS_DENIED);
		else
		{
			Module *moo = FindModule(Modulestr);

			if(!moo)
			{
				source.Reply("Module \002%s\002 isn't loaded", Modulestr.c_str());
				return;
			}

			if(!moo->handle || moo->GetPermanent())
			{
				source.Reply("Unable to remove Module \002%s\002", Modulestr.c_str());
				return;
			}

			if(!ModuleHandler::Unload(moo))
			{
				source.Reply("Failed to remove Module %s", Modulestr.c_str());
				Log(source.u, this) << "to unload " << Modulestr << " and failed";
			}
			else
			{
				source.Reply("Module \2%s\2 unloaded successfully", Modulestr.c_str());
				Log(source.u, this) << "to unload " << Modulestr;
			}
		}
	}

	bool OnHelp(CommandSource &source, const Flux::string&) override
	{
		this->SendSyntax(source);
		source.Reply(" ");
		source.Reply("This command unloads a Module from the bot\n"
		             "Note: You must be the bots owner to use this command");
		return true;
	}
};

class CommandMReload : public Command
{
public:
	CommandMReload(Module *m): Command(m, "MODRELOAD", C_PRIVATE, 1, 1)
	{
		this->SetDesc("Reloads a Module");
		this->SetSyntax("\37name\37");
	}

	void Run(CommandSource &source, const Flux::vector &params) override
	{
		const Flux::string Modulestr = params[1];

		if(!source.u->IsOwner())
			source.Reply(ACCESS_DENIED);
		else
		{
			Module *mo = FindModule(Modulestr);

			if(!mo)
			{
				source.Reply("Module \002%s\002 isn't loaded", Modulestr.c_str());
				return;
			}

			if(!mo->handle || mo->GetPermanent())
			{
				source.Reply("Unable to reload Module \002%s\002", Modulestr.c_str());
				return;
			}

			bool err = ModuleHandler::Unload(mo);
			ModErr err2 = ModuleHandler::LoadModule(Modulestr);

			if(!err || err2 != MOD_ERR_OK)
			{
				source.Reply("Failed to reload Module %s%s", Modulestr.c_str(), err2 != MOD_ERR_OK ? Flux::string(": " + DecodeModErr(err2)).c_str() : "");
				Log(source.u, this) << "to reload " << Modulestr << " and failed";
			}
			else
			{
				source.Reply("Module \2%s\2 reloaded successfully", Modulestr.c_str());
				Log(source.u, this) << "to reload " << Modulestr;
			}
		}
	}

	bool OnHelp(CommandSource &source, const Flux::string&) override
	{
		this->SendSyntax(source);
		source.Reply(" ");
		source.Reply("This command reloads the Module, it does\n"
		             "the same as modunload and modload just in\n"
		             "one command to be run which is much faster");
		return true;
	}
};

class CommandMInfo : public Command
{
public:
	CommandMInfo(Module *m): Command(m, "MODINFO", C_PRIVATE, 1, 1)
	{
		this->SetDesc("Provides info on a Module");
		this->SetSyntax("\37name\37");
	}

	void Run(CommandSource &source, const Flux::vector &params) override
	{
		const Flux::string modd = params[1];

		if(modd.empty())
		{
			this->SendSyntax(source);
			return;
		}

		Module *mo = FindModule(modd);

		if(!mo)
		{
			source.Reply("Module \2%s\2 is not loaded", modd.c_str());
			return;
		}

		source.Reply("******** \2%s\2 Info ********", mo->name.c_str());
		source.Reply("Module: \2%s\2 Version: \2%s\2 Author: \2%s\2\nLoaded: \2%s\2", mo->filename.c_str(), mo->GetVersion().c_str(), mo->GetAuthor().c_str(), do_strftime(mo->GetLoadTime()).c_str());

		Flux::string cmds;

		for(auto & elem : Commandsmap)
			if(elem.second->mod == mo)//For /msg commands
				cmds += elem.second->name + " ";

		cmds.trim();

		for(auto & elem : ChanCommandMap)
			if(elem.second->mod == mo) //For Channel Commands
				cmds += elem.second->name + " ";

		cmds.trim();

		if(cmds.empty())
			source.Reply("Adds no commands");
		else
			source.Reply("Adds commands: \2%s\2", cmds.c_str());

		source.Reply("******** End Info ********");

		Log(source.u, this) << "to show info on Module " << mo->name;
	}

	bool OnHelp(CommandSource &source, const Flux::string&) override
	{
		this->SendSyntax(source);
		source.Reply(" ");
		source.Reply("This command displays Module information\n"
		             "based off the author of the Module, its\n"
		             "priority, how many commands it adds, the\n"
		             "version of the Module and when it was loaded");
		return true;
	}
};

class M_Handler : public Module
{
	CommandMList list;
	CommandMLoad load;
	CommandMReload reload;
	CommandMUnload unload;
	CommandMInfo info;
	Flux::vector CurrentModuleList;
public:
	M_Handler(const Flux::string &Name): Module(Name), list(this), load(this), reload(this), unload(this), info(this)
	{
		this->SetAuthor("Justasic");
		this->SetVersion(VERSION);
		this->SetPriority(PRIORITY_FIRST);
		this->SetPermanent(true);
		Implementation i[] = { I_OnStart, I_OnReload };
		ModuleHandler::Attach(i, this, sizeof(i) / sizeof(Implementation));
	}

	void OnStart(int, char **) override
	{
		CurrentModuleList = ParamitizeString(Config->Modules, ',');

		for(auto & elem : CurrentModuleList)
		{
			elem.trim();
//       printf("[%i] '%s'\n", i, CurrentModuleList[i].c_str());
		}
	}

	bool FindInVector(const Flux::string &nname, const Flux::vector &vector)
	{
		for(const auto & elem : vector)
		{
			const Flux::string found = elem;

			if(found.equals_cs(nname))
				return true;
		}

		return false;
	}

	void OnReload() override
	{
		Flux::vector updatedmodlist = ParamitizeString(Config->Modules, ',');

		for(Flux::vector::iterator it; it != updatedmodlist.end(); ++it)
		{
			Flux::string Modulename = *it;
			Modulename.trim();
			Module *m = FindModule(Modulename);

			if(!m)
			{
				Log() << "Rehash loaded Module " << Modulename;
				ModuleHandler::LoadModule(Modulename);
			}

			if(!FindInVector(Modulename, updatedmodlist) && m)
			{
				Log() << "Rehash unloaded Module " << Modulename;
				ModuleHandler::Unload(m);
			}
		}
	}
};
MODULE_HOOK(M_Handler)
