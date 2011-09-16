#ifndef ENC_H
#define ENC_H

class encyclopedia : public module
{

public:
  encyclopedia(bool a):module("Encyclopedia", a, PRIORITY_LAST)
  { 
    this->SetDesc("Encyclopedia module."); 
  }
    
  
	ModuleReturn run(CommandSource &source, std::vector<Flux::string> &params)
	{

		Flux::string sn = "SEND_NOTHING";

		Flux::string cmd = params.empty()?"":params[0];

		if (source.c)
		{
			Flux::string str = "python brain.py "+source.message;
			str = str.replace_all_cs("&","");
			str = str.replace_all_cs("|","");
			Flux::string information = execute(str.c_str());
			information.trim();
			if (information != sn)
			{
				source.Reply(information);
			}
			else
			{
			}
		}

		return MOD_RUN; 
	}
};
#endif