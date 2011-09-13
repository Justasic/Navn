#ifndef ENC_H
#define ENC_H

class encyclopedia : public module
{

public:
  encyclopedia(bool a):module("Encyclopedia", a, PRIORITY_LAST)
  { 
    this->SetDesc("Encyclopedia module."); 
  }
  
  /*Flux::string brain(const Flux::string &query)
  {
    std::string result, to_search = "python brain.py "+query.tostd();
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
  }*/
  
  ModuleReturn run(CommandSource &source, std::vector<Flux::string> &params)
  {
    Flux::string cmd = params.empty()?"":params[0];
    
    if((cmd == "!encyclopedia"))
    {
      Flux::string str = "python brain.py "+params[1], request = execute(str.c_str());
     source.Reply(request);
    }
    
    return MOD_RUN; 
  }
};
#endif