// Read an INI file into easy-to-access name/value pairs.

#include <cctype>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include "INIReader.h"
#include "log.h"
int INIReader::Parse(const Flux::string &filename)
{
 std::ifstream file(filename.c_str());
  int linenum, error =0;
  Flux::string line, section, name, value;
  if(file.is_open())
  {
   while(file.good())
   { 
    std::getline(file, line.std_str());
    linenum++;
    line.trim();
    
    if(line[0] == ';' || line[0] == '#' || line.empty()){ continue; } // Do nothing if any of this is true
    else if(line[0] == '[' && line[line.size() -1] == ']')
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
      if(line.find_first_of(';') < (unsigned)d){
	error = linenum;
      }
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
	  value = value.erase(i, value.size()-i);
	}
      }
      value.trim();
      /************************************/
      
      if(error != 0)
	break;
      else if(value.empty() || section.empty() || name.empty() || value.find(';') != (unsigned)-1)
	error = linenum;
      else
      _values[this->MakeKey(section, name)] = value;
    }else
      error = linenum;
   }
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
 Flux::string conffile = binary_dir + "/bot.conf";
 this->Parser = new INIReader(conffile);
 this->Binary_Dir = binary_dir;
 this->Read();
}
BotConfig::~BotConfig() { if(Parser) delete Parser; }
void BotConfig::Read(){
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

