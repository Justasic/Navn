/* Navn IRC bot -- Primary functions (This file is old)
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */

#include "module.h"
#include "xmlfile.h"

/**Runtime directory finder
 * This will get the bots runtime directory
 * @param getprogdir(const Flux::string dir)
 */
Flux::string getprogdir(const Flux::string &dir, Flux::string &bot_bin1)
{
  char buffer[FILENAME_MAX];
  if (GetCurrentDir(buffer, sizeof(buffer)))
  {
    Flux::string bin_var = dir, fullpath;
    Flux::string::size_type n = bin_var.rfind("/");

    if (bin_var[0] == '/')
      fullpath = bin_var.substr(0, n);
    else
      fullpath = Flux::string(buffer) + "/" + bin_var.substr(0, n);

    bot_bin1 = bin_var.substr(n + 1, bin_var.length());

    return fullpath;
  }
  return "/";
}

/**
 * \fn Flux::string execute(const char *cmd)
 * \brief Sends a command to the OS
 * \param cmd A command
 * \return A Flux::string containing the response from the OS.
 */
Flux::string execute(const char *cmd)
{
    /*
    * Roses are red,
    * Violets are blue,
    * I read StackOverflow
    * And so do you!
    */
    FILE* pipe = popen(cmd, "r");
    if (!pipe)
	return "";

    char buffer[128];
    Flux::string result;
    while(!feof(pipe))
    {
	if(fgets(buffer, 128, pipe) != NULL)
	    result += buffer;
    }
    pclose(pipe);
    return result;
}
/**
 * \fn static void restart(Flux::string reason)
 * \brief Restart the bot process
 * \param reason The reason for the restart
 */
void restart(const Flux::string &reason)
{
    char CurrentPath[FILENAME_MAX];
    GetCurrentDir(CurrentPath, sizeof(CurrentPath));
    FOREACH_MOD(I_OnRestart, OnRestart(reason));

    if(reason.empty())
    {
	Log() << "Restarting: No Reason";
	if(ircproto)
	ircproto->quit("Restarting: No Reason");
    }
    else
    {
	Log() << "Restarting: " << reason;
	if(ircproto)
	ircproto->quit("Restarting: %s", reason.c_str());
    }

    chdir(CurrentPath);
    int execvpret = execve(bot_bin.c_str(), my_av, my_envp);

    if(execvpret > 0)
	throw CoreException("Restart Failed, Exiting");

    Delete(Config->PidFile.c_str());
    exit(1);
}

/**
 * \fn void Rehash()
 * \brief Reload the bot config file
 * \param boolean this boolean tells rehash if we are starting from start or not
 */
void Rehash()
{
    Log() << "Rehashing Configuration File";
    try
    {
	BotConfig *configtmp = Config;
	Config = new BotConfig(configtmp);
	delete configtmp;

	if(!Config)
	    throw ConfigException("Could not read config.");

	FOREACH_MOD(I_OnReload, OnReload());
	ModuleHandler::LoadModuleList(ParamitizeString(Config->Modules, ','));
    }
    catch(const ConfigException &ex)
    {
	Log() << "Configuration Exception Caught: " << ex.GetReason();
	for(unsigned i = 0; i < Config->Owners.size(); ++i)
	    ircproto->notice(Config->Owners[i], "Config Exception Caught: %s", ex.GetReason());
    }
}

/**
 * \fn static void remove_pidfile()
 * \brief Removes the PID file on exit
 */
static void remove_pidfile()
{
    Delete(Config->PidFile.c_str());
    if(Config)
	delete Config;
}
/**
 * \fn static void WritePID()
 * \brief Write the bots PID file
 */
static void WritePID()
{
    //logging to a text file and making the PID file.
    if(Config->PidFile.empty())
	throw CoreException("Cannot write PID file, no PID file specified.");

    if(TextFile::IsFile(Config->PidFile))
    {
	Log(LOG_DEBUG) << "Deleting stale pid file " << Config->PidFile;
	Delete(Config->PidFile.c_str());
    }

    FILE *pidfile = fopen(Config->PidFile.c_str(), "w");
    if(pidfile)
    {
	#ifdef _WIN32
	fprintf(pidfile, "%d\n", static_cast<int>(GetCurrentProcessId()));
	#else
	fprintf(pidfile, "%d\n", static_cast<int>(getpid()));
	#endif
	fclose(pidfile);
	atexit(remove_pidfile);
    }
    else
	throw CoreException("Can not write to PID file "+Config->PidFile);
}

class CommandLineArguments
{
protected:
    Flux::map<Flux::string> Arguments;
public:
    CommandLineArguments(int ac, char **av)
    {
	for(int i = 1; i < ac; ++i)
	{
	    Flux::string argsarg = av[i];
	    Flux::string param;
	    while(!argsarg.empty() && argsarg[0] == '-')
		argsarg.erase(argsarg.begin());

	    size_t t = argsarg.find('=');
	    if(t != Flux::string::npos)
	    {
		param = argsarg.substr(t+1);
		argsarg.erase(t);
	    }

	    if(argsarg.empty())
		continue;

	    Arguments[argsarg] = param;
	}
    }

    bool HasArg(const Flux::string &name, char shortname = '\0')
    {
	Flux::string Cppisstupidrighthere;
	return this->HasArg(name, shortname, Cppisstupidrighthere);
    }

    bool HasArg(const Flux::string &name, char shortname, Flux::string &args)
    {
	args.clear();

	for(Flux::map<Flux::string>::iterator it = this->Arguments.begin(); it != this->Arguments.end(); ++it)
	{
	    if(it->first.equals_ci(name) || (it->first.size() == 1 && it->first[0] == shortname))
	    {
		args = it->second;
		return true;
	    }
	}
	return false;
    }
};

/**This is the startup sequence that starts at the top to the try loop
 * @param startup(int, char)
 */
void startup(int argc, char** argv, char *envp[])
{
    SET_SEGV_LOCATION();

    InitSignals();
    Config = NULL;
    ircproto = NULL;
    //sock = NULL;
    my_av = argv;
    my_envp = envp;
    starttime = time(NULL); //for bot uptime

#ifndef _WIN32
    if(!getuid() && !getgid())
    {
	// Cannot use log here because it will throw errors if we do.
	std::cerr << Log::TimeStamp() << " \033[22;33m[WARNING]" << (Config?Config->LogColor:"\033[0m") << " DO NOT RUN NAVN AS ROOT!!!" << std::endl;
	std::cerr << Log::TimeStamp() << " \033[22;33m[WARNING]" << (Config?Config->LogColor:"\033[0m") << " DO NOT RUN NAVN AS ROOT!!!" << std::endl;
	std::cerr << Log::TimeStamp() << " \033[22;33m[WARNING]" << (Config?Config->LogColor:"\033[0m") << " I know that as a developer, I run lots as root, but don't follow me" << std::endl;
	std::cerr << Log::TimeStamp() << " \033[22;33m[WARNING]" << (Config?Config->LogColor:"\033[0m") << " because I am bad! The bot will start in 5 seconds." << std::endl;
	sleep(5);
    }
#endif

    binary_dir = getprogdir(argv[0], bot_bin);
    if(binary_dir[binary_dir.length() - 1] == '.')
	binary_dir = binary_dir.substr(0, binary_dir.length() - 2);

    Config = new BotConfig(NULL);
    if(!Config)
	throw CoreException("Config Error.");

    Flux::string dir = argv[0];
    dir = "." + dir.substr(dir.rfind('/'));

    //gets the command line paramitors if any.
    CommandLineArguments args(argc, argv);

    if(args.HasArg("developer"))
    {
	dev = nofork = true;
	Log(LOG_DEBUG) << Config->BotNick << " is started in Developer mode.";
    }

    if(args.HasArg("memory", 'm'))
    {
	memdebug = true;
	Log(LOG_MEMORY) << Config->BotNick << " is started in Memory Debug mode";
    }

    if(args.HasArg("debug", 'd'))
    {
	Log(LOG_WARN) << "This mode flag does nothing for now, it will do debug levels eventually ;)";
	Log(LOG_WARN) << "note: if you're looking for the old mode -d, type --developer instead";
	exit(EXIT_FAILURE);
    }

    if(args.HasArg("nofork", 'n'))
    {
	nofork = true;
	Log(LOG_DEBUG) << Config->BotNick << " is started With No Forking enabled.";
    }

    if(args.HasArg("help", 'h'))
    {
	Log(LOG_TERMINAL) << "\033[0mNavn Internet Relay Chat Bot v" << VERSION;
	Log(LOG_TERMINAL) << "Usage: " << dir << " [options] ...";
	Log(LOG_TERMINAL) << "-h, --help";
	Log(LOG_TERMINAL) << "    --developer (inhibits --nofork)";
	Log(LOG_TERMINAL) << "-d, --debug[=level]";
	Log(LOG_TERMINAL) << "-n, --nofork";
	Log(LOG_TERMINAL) << "-m, --memorydebug";
	Log(LOG_TERMINAL) << "-p, --protocoldebug";
	Log(LOG_TERMINAL) << "-c, --nocolor";
	Log(LOG_TERMINAL) << "This bot does have Epic Powers.";
	exit(0);
    }

    if(args.HasArg("version", 'v'))
    {
	Log(LOG_TERMINAL) << "\033[0mNavn IRC C++ Bot Version " << VERSION_FULL;
	Log(LOG_TERMINAL) << "This bot was programmed from scratch by Justasic and Lordofsraam.";
	Log(LOG_TERMINAL) << "";
	Log(LOG_TERMINAL) << "IRC: irc.Azuru.net #Computers";
	Log(LOG_TERMINAL) << "WWW: http://www.Azuru.net";
	Log(LOG_TERMINAL) << "Email: Development@Azuru.net";
	Log(LOG_TERMINAL) << "Git: git://gitorious.org:navn/navn.git";
	Log(LOG_TERMINAL) << "";
	Log(LOG_TERMINAL) << "This bot does have Epic Powers.";
	Log(LOG_TERMINAL) << "Type " << dir << " --help for help on how to use navn, or read the readme.";
	exit(0);
    }

    if(args.HasArg("protocoldebug", 'p'))
    {
	protocoldebug = true;
	Log(LOG_RAWIO) << Config->BotNick << " is started in Protocol Debug mode.";
    }

    if(args.HasArg("nocolor", 'c'))
    {
	nocolor = true;
	Log() << Config->BotNick << " is started in No Colors mode.\033[0m"; //reset terminal colors
    }

    if(!nocolor)
	Log(LOG_TERMINAL) << Config->LogColor;

    if(Config->Server.empty())
	throw CoreException("No Server Specified.");

    if(Config->Port.empty())
	throw CoreException("No Port Specified.");

    try
    {
	SocketHandler::Initialize();
    }
    catch(const SocketException &ex)
    {
	ex.Print();
	throw CoreException("Failed to initialize SocketHandler!");
    }
    ModuleHandler::SanitizeRuntime();
    ModuleHandler::LoadModuleList(ParamitizeString(Config->Modules, ',')); //load Modules
    WritePID(); //Write the pid file
    FOREACH_MOD(I_OnStart, OnStart(argc, argv)); //announce we are starting the bot
    Fork(); //Fork to background
}

void GarbageCollect()
{
    FOREACH_MOD(I_OnShutdown, OnShutdown());

    try { SocketHandler::Shutdown(); } catch (const SocketException &ex) { ex.Print(); }
    ModuleHandler::UnloadAll();
    // Schedule all channels for deletion
    for(Flux::insensitive_map<Channel*>::iterator it = ChanMap.begin(), it_end = ChanMap.end(); it != it_end;)
    {
	Channel *c = it->second;
	++it;
	Log(LOG_MEMORY) << "Deleting Channel @" << c;
	delete c;
    }
    ChanMap.clear();

    // Schedule all Users for deletion
    for(Flux::insensitive_map<User *>::iterator it = UserNickList.begin(), it_end = UserNickList.end(); it != it_end;)
    {
	User *u = it->second;
	++it;
	Log(LOG_MEMORY) << "Deleting User @" << u;
	delete u;
    }
    UserNickList.clear();

    if(ircproto)
    {
	Log(LOG_MEMORY) << "Deleting ircproto @" << ircproto;
	delete ircproto;
	ircproto = NULL;
    }

    /*if(sock)
    {
	if(!quitmsg.empty())
	    sock->send(printfify("QUIT :%s\n", quitmsg.c_str()));
	else
	    sock->send("QUIT");

	Log(LOG_MEMORY) << "Deleting socket @" << sock;
	delete sock;
	sock = NULL;
    }*/

    // Clean the runtime last incase we wanna put some temp crap in the runtime folder for deletion.
    ModuleHandler::SanitizeRuntime();

    // We need to be careful that we don't deallocate a timer, they cause segmentation faults
    for(std::vector<Base*>::iterator it = BaseReferences.begin(), it_end = BaseReferences.end(); it != it_end;)
    {
	Base *b = *++it;
	// Timers can cause errors.
    //     if(typeid(b) == typeid(Timer))
    //     {
    //       Log(LOG_WARN) << "Leftover timer: @" << b;
    //       continue;
    //     }
	Log(LOG_MEMORY) << "Deleting base reference @" << b;
	delete b;
    }
    BaseReferences.clear();
}
