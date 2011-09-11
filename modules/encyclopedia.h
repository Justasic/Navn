#ifndef ENC_H
#define ENC_H

class encyclopedia : public module
{

public:
  encyclopedia(bool a):module("Encyclopedia", a, PRIORITY_LAST)
  { 
    this->SetDesc("Encyclopedia module."); 
  }
  
  std::string brain(std::string query)
  {
    std::string result = "";
    std::string to_search = "python brain.py "+query;
    system(to_search.c_str());
    std::string line;
    std::ifstream myfile("dump.txt");
    if (myfile.is_open())
    {
      while(myfile.good())
      {
	std::getline(myfile,line);
	result += line;
      }
      myfile.close();
      system("rm dump.txt");
      return result;
    }
    return result;
  }
  
  ModuleReturn run(CommandSource &source, std::vector<Flux::string> &params)
  {
    User *u = source.u;
    Channel *c = source.c;
    Flux::string cmd = params.empty()?"":params[0];
    
    if((cmd == "!encyclopedia"))
    {
     source.Reply(brain(params[1].tostd()));
    }
    
    return MOD_RUN; 
  }
};
#endif