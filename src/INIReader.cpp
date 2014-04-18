/* Navn IRC bot -- INI File parser
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */

// Read an INI file into easy-to-access name/value pairs.

#include <cctype>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include "INIReader.h"
#include "log.h"

bool in_comment = false;
int INIReader::Parse(const Flux::string &filename)
{
	SET_SEGV_LOCATION();
	std::ifstream file(filename.c_str());
	int linenum = 0;
	Flux::string line, section, name, value;

	if(file.is_open())
	{
		while(file.good())
		{
			bool contin = false;
			std::getline(file, line.std_str());
			linenum++;
			line.trim();
			//printf("UNPARSED: %s\n", line.c_str());

			if(line[0] == '#' || line.empty())
				continue;

			/********************************************/
			unsigned c = 0, len = line.length();

			for(; c < len; ++c)
			{
				char ch = line[c];

				if(in_comment)
				{
					if(ch == '*' && c + 1 < len && line[c + 1] == '/')
					{
						in_comment = false;
						line = line.erase(c, c + 2);
						line.trim();

						if((line[0] == '[' && line[line.size() - 1] == ']') || (!line.empty() && line.find_first_of('=')))
							continue;
						else
							contin = true;

						++c;
					}

					continue;
				}
				else if(ch == '/' && c + 1 < len && line[c + 1] == '*')
				{
					in_comment = true;
					++c;
					continue;
				}
			}

			if(line.search("/*") && line.search("*/"))
			{
				in_comment = contin = false;
				line = line.erase(line.find("/*"), line.find("*/"));
				line.trim();
			}

			if(in_comment || contin)
				continue;

			/********************************************/
			//printf("PARSING: %s\n", line.c_str());
			if(line[0] == '[' && line[line.size() - 1] == ']')
			{
				line = line.erase(0, 1);
				section = line.erase(line.size() - 1, line.size());
				section.trim();
			}
			else if((line[0] == '[' && line[line.size() - 1] != ']') || (line[0] != '[' && line[line.size() - 1] == ']'))
				throw ConfigException(printfify("Brackets not terminated: %i", linenum));
			else if(!line.empty() && line.find_first_of('='))
			{
				name = line;
				int d = line.find_first_of('=');

				if(line.find_first_of(';') < static_cast<unsigned>(d))
					throw ConfigException(printfify("Cannot have semi-colon immediately after assignment: %i", linenum));
				else if(d > 0)
				{
					name = name.erase(d, name.size() - d);
					name.trim();
				}

				/************************************/
				value = line;
				value = value.erase(0, value.find('=') + 1);
				value.trim();

				if(value.find_first_of(';'))
				{
					//We only erase ';' (semi-colons) if we find them, we cannot erase # signs for
					int i = value.find_first_of(';'); // channels would look like comments.. maybe we can fix this one day..

					if(i > 0)
					{
						if(i + 1 <= static_cast<int>(value.size()) && value[i + 1] == ';')
							value = value.replace_all_cs(";;", ";");
						else
							value = value.erase(i, Flux::string::npos);
					}
				}

				value.trim();
				/************************************/
				//printf("PARSED: %s | %s | %s | %s | %i\n", line.c_str(), value.c_str(), name.c_str(), section.c_str(), error);

				if(value.empty() || section.empty() || name.empty())
					throw ConfigException(printfify("Empty value/section/name: %i", linenum));
				else
					_values[this->MakeKey(section, name)] = value;
			}
			else
				throw ConfigException(printfify("Undefined data: %i", linenum));
		}

		if(in_comment)
			throw ConfigException(printfify("Unterminated comment: %i", linenum));

		file.close();
	}
	else
		return -1;

	return 0;
}
/**
 * \class INIReader The config parser class, this parses the INI file for config values
 * \brief This class contains the actual parsing of values from the text file to a map containing all the config values
 * \fn INIReader::INIReader(const Flux::string &filename)
 * \param Flux::string The filename to parse for a config value.
 */
INIReader::INIReader(const Flux::string &filename)
{
	_error = this->Parse(filename);
}
INIReader::~INIReader() { }
/**
 * \brief returns a int of the last parse error
 * \fn int INIReader::ParseError()
 * \return integer of last parse error
 */
int INIReader::ParseError()
{
	return _error;
}
/**
 * \brief Returns a Flux::string from the config containing whatever value you requested
 * \fn Flux::string INIReader::Get(const Flux::string &section, const Flux::string &name, const Flux::string &default_value)
 * \return Flux::string with your value from the config
 * \param Flux::string The section the value is in
 * \param Flux::string the name of the variable in the config
 * \param bool the default value if there is no value in the config
 */
Flux::string INIReader::Get(const Flux::string &section, const Flux::string &name, const Flux::string &default_value)
{
	Flux::string key = MakeKey(section, name);
	return _values.count(key) ? _values[key] : default_value;
}
/**
 * \brief Returns an boolean value from the config
 * \fn bool INIReader::GetBoolean(const Flux::string &section, const Flux::string &name, bool default_value)
 * \return boolean of the value
 * \param Flux::string The section the value is in
 * \param Flux::string the name of the variable in the config
 * \param bool the default value if there is no value in the config
 */
bool INIReader::GetBoolean(const Flux::string &section, const Flux::string &name, bool default_value)
{
	Flux::string valstr = Get(section, name, "");
	valstr.trim();

	if(valstr.empty())
		return default_value;

	if(valstr.equals_ci("yes") || valstr.equals_ci("1") || valstr.equals_ci("y") || valstr.equals_ci("true"))
		return true;
	else if(valstr.equals_ci("false") || valstr.equals_ci("0") || valstr.equals_ci("n") || valstr.equals_ci("no"))
		return false;

	return default_value;
}
/**
 * \brief Returns an integer value from the config
 * \fn long INIReader::GetInteger(const Flux::string &section, const Flux::string &name, long default_value)
 * \return long integer of the value
 * \param Flux::string The section the value is in
 * \param Flux::string the name of the variable in the config
 * \param long the default value if there is no value in the config
 */
long INIReader::GetInteger(const Flux::string &section, const Flux::string &name, long default_value)
{
	Flux::string valstr = Get(section, name, "");
	const char *value = valstr.c_str();
	char *end;
	// This parses "1234" (decimal) and also "0x4D2" (hex)
	long n = strtol(value, &end, 0);
	return end > value ? n : default_value;
}

float INIReader::GetFloat(const Flux::string &section, const Flux::string &name, float default_value)
{
	Flux::string valstr = Get(section, name, "");
	float val = static_cast<float>(valstr);

	return val > 0 ? val : default_value;
}

Flux::string INIReader::MakeKey(const Flux::string &section, const Flux::string &name)
{
	Flux::string key = section + "." + name;

	// Convert to lower case to make lookups case-insensitive
	for(auto & elem : key)
		elem = tolower(elem);

	return key;
}
/**************************************************************************************/
BotConfig::BotConfig(BotConfig *old)
{
	SET_SEGV_LOCATION();
	Flux::string conffile = binary_dir + "/bot.conf";

	try
	{
		this->Parser = new INIReader(conffile);

		if(!this->Parser)
			throw ConfigException("Cannot read config parser (is the config file there?)");

		this->Read();

		if(this->Parser->ParseError() == -1)
			throw ConfigException("Cannot open '" + conffile + "'");

		if(this->Parser->ParseError() != 0)
			throw ConfigException(printfify("Error on line %i", this->Parser->ParseError()));
	}
	catch(const ConfigException &e)
	{
		if(!old)
			throw CoreException(printfify("Config: %s", e.GetReason()));
		else
			Log() << "Config Exception: " << e.GetReason();

		return;
	}
}

BotConfig::~BotConfig()
{
	if(this->Parser) delete this->Parser;
}

void BotConfig::Read()
{
	SET_SEGV_LOCATION();
	this->LogFile     = this->Parser->Get("Log", "Log_File", "navn.log");
	this->PingTimeoutTime = this->Parser->GetInteger("Bot", "PingTimeoutTime", 120);
	this->Owners      = ParamitizeString(this->Parser->Get("Bot", "Owners", ""), ',');
	this->Realname    = this->Parser->Get("Connect", "Realname", Flux::string("The Navn Bot " + value_cast<Flux::string>(VERSION)));
	this->Ident       = this->Parser->Get("Connect", "Ident", "Navn");
	this->ReconnectTime   = this->Parser->GetInteger("Connect", "ReconnectTime", 30);
	this->ReconnectTries  = this->Parser->GetInteger("Connect", "ReconnectTries", 5);
	this->BotNick     = this->Parser->Get("Connect", "Nick", "Navn");
	this->Channel     = this->Parser->Get("Connect", "Channel", "#Test");
	this->Port        = this->Parser->Get("Connect", "Port", "6667");
	this->Server      = this->Parser->Get("Connect", "Server", "");
	this->LogChannel  = this->Parser->Get("Modules", "LogChannel", "");
	this->PidFile     = this->Parser->Get("Bot", "PID File", "navn.pid");
	this->UserPass    = this->Parser->Get("Bot", "Password", "Navn");
	this->FantasyPrefix  = this->Parser->Get("Bot", "FantasyPrefix", "!");
	this->OperatorAccount = this->Parser->Get("Oper", "Oper_Username", "");
	this->OperatorPass    = this->Parser->Get("Oper", "Oper_Password", "");
	this->ModuleDir   = Parser->Get("Modules", "ModuleDir", "");
	this->Modules     = Parser->Get("Modules", "Modules", "");
	this->SockWait    = Parser->GetInteger("Bot", "Socket Timeout", 5);
	this->LogTime     = Parser->GetInteger("Log", "Log Time", 0);
	this->ServerPassword  = Parser->Get("Connect", "Password", "");
	this->ServicesAccount = this->Parser->Get("Services", "Account", "");
	this->ServicesPass    = this->Parser->Get("Services", "Password", "");
	this->ServicesSendString = this->Parser->Get("Services", "Send String", "");
	this->IdentOnConn     = this->Parser->GetBoolean("Services", "Identify on connect", true);
	this->ServicesService = this->Parser->Get("Services", "Service", "");
	this->AutoIdentString = this->Parser->Get("Services", "AutoIdent String", "");
	this->WelcomeMessage  = this->Parser->Get("Bot", "Welcome Message", "");
	this->LogAge      = this->Parser->GetInteger("Log", "Log Age", 2);
	this->LogColor    = this->Parser->Get("Log", "Color", "\033[22;36m").replace_all_cs("\\033", "\033");
	this->LogChan     = this->Parser->Get("Log", "Log Channel", "");
	this->BurstRate   = this->Parser->GetInteger("SendQ", "Burst Rate", 7);
	this->SendQLines  = this->Parser->GetInteger("SendQ", "Lines", 5);
	this->SendQRate   = this->Parser->GetInteger("SendQ", "Rate", 5);
	this->SendQEnabled    = this->Parser->GetBoolean("SendQ", "Enabled", true);
}

