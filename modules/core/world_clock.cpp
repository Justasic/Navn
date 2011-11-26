/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */

#include "flux_net_irc.hpp"

/**
 * \file world_clock.h Header file holding the \a world_clock function.
 * \author Lordofsraam. Polished by Justasic.
 */

/**
 * \defgroup world_clockM World Clock Module
 * This is the module for the world clock function.
 * For a better description see the function description.
 * \section commands Commands associated with this module.
 * \subsection time !time
 * Say \a !time to see the current time, the GMT, and the times in New York, California, Beijing, and Sydney.
 * @{
 */

/**
 * \fn class world_clock(bool a):module("World Clock", a, PRIORITY_DONTCARE){ this->SetDesc("Shows time for your area or for the preset ones"); }
 * \brief Tells the world time.
 * Uses C++ time functions to tell the time around the world.
 */
class CommandCWClock: public Command
{
public:
  CommandCWClock():Command("!TIME", 0, 2)
  {
   this->SetDesc("Shows the time in the channel");
   this->SetSyntax("\37location\37");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    Channel *c = source.c;
    Flux::string location = params.empty()?"":params[1];
    location.trim();
    if(location.empty())
    {
      Clock * wclock = new Clock();
      c->SendMessage("Current time around the World:");
      c->SendMessage("GMT == %s", wclock->UTC().c_str());
      c->SendMessage("New York (USA) == %s", wclock->EST().c_str());
      c->SendMessage("California (USA) == %s", wclock->PST().c_str());
      c->SendMessage("Beijing (China) == %s", wclock->CCT().c_str());
      c->SendMessage("Sydney (Australia) == %s", wclock->AUS().c_str());
      tm *ptm;
      time_t rawtime;
      time(&rawtime);
      char buf[100];
      ptm = localtime(&rawtime);
      strftime(buf,100,"Navn's Time: %Z %c",ptm);
      c->SendMessage(buf);
      Log(source.u, this) << "command in " << c->name;
      return;
    }else{
      Flux::string wget, tmpfile = TextFile::TempFile(Config->Binary_Dir+"/runtime/navn_xml.tmp.XXXXXX");
      wget = "wget -q -O "+tmpfile+" - http://www.google.com/ig/api?weather="+(location.is_number_only()?location:urlify(removeCommand(this->name.ci_str(), source.message.ci_str())));
      system(wget.c_str());
      XMLFile *xf = new XMLFile(tmpfile);
      
      Flux::string city = xf->Tags["xml_api_reply"].Tags["weather"].Tags["forecast_information"].Tags["city"].Attributes["data"].Value;
      Flux::string time = xf->Tags["xml_api_reply"].Tags["weather"].Tags["forecast_information"].Tags["current_date_time"].Attributes["data"].Value;
      delete xf;
      c->SendMessage("The current time in %s is %s", city.c_str(), time.c_str());
      Delete(tmpfile.c_str());
      Log(source.u, this) << "to get time for " << location;
    }
  }
  bool OnHelp(CommandSource &source, const Flux::string &nill)
  {
    this->SendSyntax(source);
    source.Reply(" ");
    source.Reply("This command shows various time in areas\n"
		 "around the world, currently showing GMT, EST,\n"
		 "PST, CCT, and Australian timezones.\n"
		 "If a location identifying text is givin\n"
		 "the bot will go to google for time in that area");
    return true;
  }
};
class world_clock:public module{
  CommandCWClock clock;
public:
  world_clock(const Flux::string &Name):module(Name){
    this->SetAuthor("Lordofsraam");
    this->SetVersion(VERSION);
    this->AddChanCommand(&clock);
  }
};
MODULE_HOOK(world_clock)
/**
 * @}
 */
