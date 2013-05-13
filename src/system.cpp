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

	if(GetCurrentDir(buffer, sizeof(buffer)))
	{
		Flux::string bin_var = dir, fullpath;
		Flux::string::size_type n = bin_var.rfind("/");

		if(bin_var[0] == '/')
			fullpath = bin_var.substr(0, n);
		else
			fullpath = Flux::string(buffer) + "/" + bin_var.substr(0, n);

		bot_bin1 = bin_var.substr(n + 1, bin_var.length());

		return fullpath;
	}

	return "/";
}

class irc_string: Flux::string
{
	/** \class irc_string
	 * NOTE this MUST be included in the main file.
	 * This class wraps around a Flux::string (usually revieved from the irc server)
	 * and attempts to decode all the info recieved from the server
	 * so as to make it easier to read different kinds of information
	 * (ei. who said something, their host, username, etc.)
	 * \param toks Private vector that will contain the words in \a message.
	 * \param raw_Flux::string Will hold the exact Flux::string that was recieved from the server.
	 * \param usernick Will hold the usernick of the person who sent the message.
	 * \param host Will hold the host of the person who sent the message.
	 * \param user Will hold the user of the person who sent the message.
	 * \param channel Will hold the channel of where the message was said.
	 * \param message Will hold the message (stripped of irc protocol stuff)
	 * \deprecated Find a better way of using this.
	 */
private:
	std::vector<Flux::string> toks;
public:
	Flux::string raw_string;
	Flux::string usernick;
	Flux::string host;
	Flux::string user;
	Flux::string channel;
	Flux::string message;
	/**
	 * \fn irc_string(Flux::string reply)
	 * \deprecated This class is deprecated, find another way of using your strings
	 *Constructor for \a irc_string class
	 * This is where the magic happens. The constructor takes the Flux::string
	 * and breaks it down into its component parts to find the
	 * \a usernick \a host \a user \a channel and \a message
	 */
	DEPRECATED(irc_string(Flux::string reply))
	{
		raw_string = reply;
		usernick = reply.isolate(':', '!');
		host = reply.isolate('@', ' ');
		user = reply.isolate('!', '@');
		channel = '#' + reply.isolate('#', ' ');
		Flux::string space = " ";
		size_t pos = reply.find(" :");
		pos += 2;

		for(unsigned i = pos; i < reply.size(); i++)
		{
			if(reply.at(i) == ' ')
			{
				message.append(space);
			}
			else
			{
				message = message + reply.at(i);
			}
		}

		if(message.size() > 2)
		{
			message.resize(message.size() - 2);
		}

		Flux::string fmessage = message;
		char *cmessage = const_cast<char *>(fmessage.c_str());
		char *pch;
		pch = strtok(cmessage, " ");

		while(pch != NULL)
		{
			toks.push_back(pch);
			pch = strtok(NULL, " ");
		}
	}
	/**
	 * \fn Flux::string params(int i)
	 * \brief Returns individual words from the message of a reply
	 * \deprecated use Module parameter vectors instead
	 * Because \a toks is private, this is its "get" function.
	 * We made this so someone writing a Module doesn't try to go out
	 * of bounds while accessing an array.
	 * \param i An integer value.
	 * \return A Flux::string with the single specified word.
	 */
	DEPRECATED(Flux::string params(unsigned i))
	{
		if(i >= toks.size())
		{
			return "";
		}
		else
		{
			return toks[i];
		}
	}
	/**
	 * \overload Flux::string params(int b, int e)
	 * \brief Overload of params. Returns a range of words.
	 * \deprecated
	 * We overloaded \a params() so that you could get a range of words from the message
	 *  as requeseted by Justasic.
	 * \param b An integer value describing the place of the first word wanted.
	 * \param e An integer value describing the place of the last word wanted.
	 * \return A Flux::string withing the range of words specified by \a b and \a e
	 */
	DEPRECATED(Flux::string params(unsigned b, unsigned e))
	{
		Flux::string buf = "";

		if(b >= toks.size())
		{
			b = toks.size();
		}

		if(e >= toks.size())
		{
			e = toks.size() - 1;
		}

		for(unsigned i = b; i <= (e); i++)
		{
			buf += toks[i];
			buf.append(" ");
		}

		return buf;
	}
	/**
	 * \fn static Flux::string isolate(char begin, char end, Flux::string msg)
	 * \brief Isolates a Flux::string between two characters
	 * \deprecated use the plain isolate function instead
	 * Finds the first character, then begins to add every consecutive character from there to a Flux::string
	 *  until it reaches the end character.
	 * \param begin The character saying where the cut should begin.
	 * \param end The character saying where the cut should end.
	 * \param msg The Flux::string you are wanting to isolate from.
	 */
	DEPRECATED(static Flux::string isolate(char begin, char end, Flux::string msg))
	{
		Flux::string to_find;
		size_t pos = msg.find(begin);
		pos += 1;

		for(unsigned i = pos; i < msg.length(); i++)
		{
			if(msg[i] == end)
				break;
			else
				to_find = to_find + msg[i];
		}

		return to_find;
	}
	/**
	 * \fn bool said(Flux::string findee)
	 * \brief Check if something was said.
	 * \deprecated use Flux::string::search() and Flux::string::search_ci() instead
	 * \param findee The Flux::string you want to check if was said.
	 * \return True if \a findee was found, false otherwise.
	 */
	DEPRECATED(bool said(Flux::string findee))
	{
		unsigned i = raw_string.find(findee);

		if(i != Flux::string::npos)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	/**
	 * \overload static bool said(Flux::string source, Flux::string findee)
	 * \brief Static overload of said() function.
	 * \deprecated
	 * \param source A Flux::string that is to be searched through.
	 * \param findee The Flux::string that is to be found.
	 * We overloaded the said function and made it static because we thought it would be
	 *  very useful to have outside of an \a irc_string object.
	 * \return True if \a findee was found, false otherwise.
	 */
	DEPRECATED(static bool said(Flux::string source, Flux::string findee))
	{
		unsigned i = source.find(findee);

		if(i != Flux::string::npos)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};
/**
 * \fn Flux::string removeCommand(Flux::string command, Flux::string s)
 * \brief Removes a command from a Flux::string.
 * \param command String to be taken out.
 * \param s Original Flux::string.
 * \deprecated use Module parameters system instead
 * This takes out \a command from \a s and returns \a s without \a command It's very useful when you want
 * to use the rest of a Flux::string as an argument for a command.
 * \return A Flux::string \a s without \a command.
 */
Flux::string removeCommand(Flux::string command, Flux::string s)
{
	size_t pos = s.find(command);
	return s.substr(pos + (command.size()) + 1);
}

/**
 * \fn Flux::string urlify(Flux::string raw_searchstring)
 * \brief Replaces special chars in a Flux::string with url compliant codes.
 * \param raw_searchstring
 * \deprecated use Flux::string::url_str() instead
 * Goes through each character in a Flux::string and if it finds a special character,
 * it replaces it with what would be in a url for that character.
 * \return A Flux::string without any special characters other than %
 */
Flux::string urlify(const Flux::string &received)
{
	Flux::string string;

	for(unsigned i = 0; i < received.size(); ++i)
	{
		switch(received[i])
		{
			case '%':
				string = string + "%25";
				break;
			case ' ':
				string = string + "%20";
				break;
			case '+':
				string = string + "%2B";
				break;
			case '$':
				string = string + "%24";
				break;
			case '&':
				string = string + "%26";
				break;
			case ',':
				string = string + "%2C";
				break;
			case '/':
				string = string + "%2F";
				break;
			case ':':
				string = string + "%3A";
				break;
			case ';':
				string = string + "%3B";
				break;
			case '=':
				string = string + "%3D";
				break;
			case '?':
				string = string + "%3F";
				break;
			case '@':
				string = string + "%40";
				break;
			case '#':
				string = string + "%23";
				break;
			case '>':
				string = string + "%3E";
				break;
			case '<':
				string = string + "%3C";
				break;
			case '{':
				string = string + "%7B";
				break;
			case '}':
				string = string + "%7D";
				break;
			case '|':
				string = string + "%7C";
				break;
			case '\\':
				string = string + "%5C";
				break;
			case '^':
				string = string + "%5E";
				break;
			case '~':
				string = string + "%7E";
				break;
			case '[':
				string = string + "%5B";
				break;
			case ']':
				string = string + "%5D";
				break;
			case '`':
				string = string + "%60";
				break;
			case '*':
				string = string + "%2A";
				break;
			case '(':
				string = string + "%28";
				break;
			case ')':
				string = string + "%29";
				break;
			case '\'':
				string = string + "%27";
				break;
			case '"':
				string = string + "%22";
				break;
			case '.':
				string = string + "%2E";
				break;
			case '-':
				string = string + "%2D";
				break;
			default:
				string = string + received[i];
		}
	}

	return string;
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
	FILE *pipe = popen(cmd, "r");

	if(!pipe) return "";

	char buffer[128];
	Flux::string result = "";

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
		throw CoreException("Can not write to PID file " + Config->PidFile);
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
				param = argsarg.substr(t + 1);
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
void startup(int argc, char **argv, char *envp[])
{
	SET_SEGV_LOCATION();

	InitSignals();
	Config = NULL;
	ircproto = NULL;
	sock = NULL;
	my_av = argv;
	my_envp = envp;
	starttime = time(NULL); //for bot uptime

#ifndef _WIN32

	if(!getuid() && !getgid())
	{
		// Cannot use log here because it will throw errors if we do.
		std::cerr << Log::TimeStamp() << " \033[22;33m[WARNING]" << (Config ? Config->LogColor : "\033[0m") << " DO NOT RUN NAVN AS ROOT!!!" << std::endl;
		std::cerr << Log::TimeStamp() << " \033[22;33m[WARNING]" << (Config ? Config->LogColor : "\033[0m") << " DO NOT RUN NAVN AS ROOT!!!" << std::endl;
		std::cerr << Log::TimeStamp() << " \033[22;33m[WARNING]" << (Config ? Config->LogColor : "\033[0m") << " I know that as a developer, I run lots as root, but don't follow me" << std::endl;
		std::cerr << Log::TimeStamp() << " \033[22;33m[WARNING]" << (Config ? Config->LogColor : "\033[0m") << " because I am bad! The bot will start in 5 seconds." << std::endl;
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

	ModuleHandler::SanitizeRuntime();
	ModuleHandler::LoadModuleList(ParamitizeString(Config->Modules, ',')); //load Modules
	WritePID(); //Write the pid file
	FOREACH_MOD(I_OnStart, OnStart(argc, argv)); //announce we are starting the bot
	Fork(); //Fork to background
}

void GarbageCollect()
{
	FOREACH_MOD(I_OnShutdown, OnShutdown());

	ModuleHandler::UnloadAll();

	// Schedule all channels for deletion
	for(Flux::insensitive_map<Channel *>::iterator it = ChanMap.begin(), it_end = ChanMap.end(); it != it_end;)
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

	if(sock)
	{
		if(!quitmsg.empty())
			sock->send(printfify("QUIT :%s\n", quitmsg.c_str()));
		else
			sock->send("QUIT");

		Log(LOG_MEMORY) << "Deleting socket @" << sock;
		delete sock;
		sock = NULL;
	}

	// Clean the runtime last incase we wanna put some temp crap in the runtime folder for deletion.
	ModuleHandler::SanitizeRuntime();

	// We need to be careful that we don't deallocate a timer, they cause segmentation faults
	for(std::vector<Base *>::iterator it = BaseReferences.begin(), it_end = BaseReferences.end(); it != it_end;)
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
