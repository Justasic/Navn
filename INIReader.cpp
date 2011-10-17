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
  int linenum, error =0;
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
    
    if(line[0] == ';' || line[0] == '#' || line.empty())
      continue;  // Do nothing if any of this is true
    /********************************************/
    unsigned c=0, len = line.length();
    for(; c < len; ++c)
    {
      char ch = line[c];
      if(in_comment){
	if(ch == '*' && c+1 < len && line[c+1] == '/')
	{
	  in_comment = false;
	  contin = true;
	  ++c;
	}
	continue;
      }
      else if(ch == '/' && c+1 < len && line[c+1] == '*')
      {
	in_comment = true;
	++c;
	continue;
      }
    }
    if(line.search("/*") && line.search("*/")){
      in_comment = contin = false;
      line = line.erase(line.find("/*"), line.find("*/"));
      line.trim();
    }
    if(in_comment || contin)
      continue;
    /********************************************/
    //printf("PARSING: %s\n", line.c_str());
    if(line[0] == '[' && line[line.size() -1] == ']')
    {
      line = line.erase(0,1);
      section = line.erase(line.size()-1,line.size());
      section.trim(); 
    }
    else if((line[0] == '[' && line[line.size()-1] != ']') || (line[0] != '[' && line[line.size() -1] == ']'))
      error = linenum;
    else if(!line.empty() && line.find_first_of('=')){
      name = line;
      int d = line.find_first_of('=');
      if(line.find_first_of(';') < (unsigned)d)
	error = linenum;
      else if(d > 0){
	name = name.erase(d, name.size()-d);
	name.trim();
      }
      /************************************/
      value = line;
      value = value.erase(0,value.find('=')+1);
      value.trim();
      if(value.find_first_of(';')){ //We only erase ';' (semi-colons) if we find them, we cannot erase # signs for
	int i = value.find_first_of(';'); // channels would look like comments.. maybe we can fix this one day..
	if(i > 0){
	  value = value.erase(i, Flux::string::npos);
	}
      }
      value.trim();
      /************************************/
      //printf("PARSED: %s | %s | %s | %s | %i\n", line.c_str(), value.c_str(), name.c_str(), section.c_str(), error);
      if(error != 0)
	break;
      else if(value.empty() || section.empty() || name.empty() || value.search(';'))
	error = linenum;
      else
      _values[this->MakeKey(section, name)] = value;
    }else
      error = linenum;
   }
   if(in_comment)
      error = linenum;
   file.close();
  }else
    return -1;
  return error;
}
INIReader::INIReader(const Flux::string &filename)
{
  _error = this->Parse(filename);
}
INIReader::~INIReader(){ }
int INIReader::ParseError()
{
    return _error;
}

Flux::string INIReader::Get(const Flux::string &section, const Flux::string &name, const Flux::string &default_value)
{
    Flux::string key = MakeKey(section, name);
    return _values.count(key) ? _values[key] : default_value;
}

long INIReader::GetInteger(const Flux::string &section, const Flux::string &name, long default_value)
{
    Flux::string valstr = Get(section, name, "");
    const char* value = valstr.c_str();
    char* end;
    // This parses "1234" (decimal) and also "0x4D2" (hex)
    long n = strtol(value, &end, 0);
    return end > value ? n : default_value;
}

Flux::string INIReader::MakeKey(const Flux::string &section, const Flux::string &name)
{
    Flux::string key = section + "." + name;
    // Convert to lower case to make lookups case-insensitive
    for (unsigned i = 0; i < key.length(); i++)
        key[i] = tolower(key[i]);
    return key;
}
/**************************************************************************************/
BotConfig::BotConfig()
{
  SET_SEGV_LOCATION();
 Flux::string conffile = binary_dir + "/bot.conf";
 try
 {
  this->Parser = new INIReader(conffile);
  this->Binary_Dir = binary_dir;
  this->Read();
  if(this->Parser->ParseError() == -1)
    throw ConfigException("Cannot open '"+conffile+"'");
  if(this->Parser->ParseError() != 0)
    throw ConfigException("Error on line "+this->Parser->ParseError());
 }catch(const ConfigException &e){
   if (starttime == time(NULL))
     throw CoreException(e.GetReason());
   else
      Log(LOG_TERMINAL) << "Config Exception: " << e.GetReason();
   //delete this; //This makes segfault :D
 }
}
BotConfig::~BotConfig() { if(Parser) delete Parser; }
void BotConfig::Read(){
  SET_SEGV_LOCATION();
  this->LogFile = this->Parser->Get("Log","Log_File","navn.log");
  this->ServicesAccount = this->Parser->Get("Bot","NickServ_Account","");
  this->ServicesPass = this->Parser->Get("Bot","NickServ_Password","");
  this->Owner = this->Parser->Get("Bot","Owner","");
  this->Realname = this->Parser->Get("Connect","Realname",Flux::string("The Navn Bot "+Flux::stringify(VERSION)));
  this->Ident = this->Parser->Get("Connect","Ident","Navn");
  this->BotNick = this->Parser->Get("Bot","Nick","Navn");
  this->Channel = this->Parser->Get("Bot","Channel","#Test");
  this->Port = this->Parser->Get("Connect","Port","6667");
  this->Server = this->Parser->Get("Connect", "Server", "");
  this->LogChannel = this->Parser->Get("Modules", "LogChannel","");
  this->PidFile = this->Parser->Get("Bot","PID File","navn.pid");
  this->UserPass = this->Parser->Get("Bot","Password","Navn");
  this->OperatorAccount = this->Parser->Get("Oper","Oper_Username","");
  this->OperatorPass = this->Parser->Get("Oper","Oper_Password","");
  this->ModuleDir = Parser->Get("Modules", "ModuleDir", "");
  this->Modules = Parser->Get("Modules", "Modules", "");
  Log(LOG_TERMINAL) << "\033[22;31mReading Config File\033[22;30m...\033[22;36m";
}

