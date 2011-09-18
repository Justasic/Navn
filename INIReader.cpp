// Read an INI file into easy-to-access name/value pairs.

#include <cctype>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include "INIReader.h"
int INIReader::Parse(const Flux::string &filename)
{
 std::ifstream file(filename.c_str());
  int linenum, error =0;
  Flux::string line, section, name;
  
  if(file.is_open())
  {
   while(file.good())
   { 
    std::getline(file, line.tostd());
    linenum++;
    line.trim();
    
    if(line[0] == ';' || line[0] == '#' || line.empty()){ continue; } // Do nothing if any of this is true
    else if(line[0] == '[' && line[line.size() -1] == ']')
    {
      line = line.erase(0,1);
      section = line.erase(line.size()-1,line.size());
      section.trim();
      if(section.empty())
	error = linenum; 
    }
    else if((line[0] == '[' && line[line.size()-1] != ']') || (line[0] != '[' && line[line.size() -1] == ']'))
      error = linenum; 
    else if(!line.empty() && line.find('=')){
      name = line;
      int d = line.find_first_of('=');
      if(d > 0){
	name = name.erase(d, name.size()-d);
	name.trim();
	if(name.empty())
	  error = linenum;
      }
      /************************************/
      line = line.erase(0,line.find('=')+1);
      line.trim();
      if(line.find_first_of(';')){ //We only erase ';' (semi-colons) if we find them, we cannot erase # signs for
	int i = line.find_first_of(';'); // channels would look like comments.. maybe we can fix this one day..
	if(i > 0){
	  line = line.erase(i, line.size()-i);
	}
      }
      line.trim();
      if(line.empty())
	error = linenum;
      /************************************/
      if(error != 0)
	break;
      else
      _values[this->MakeKey(section, name)] = line;
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
