/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#include "flux_net_irc.hpp"

/**
 * \file weather.h Header file holding the \a weather function.
 * \author Lordofsraam (very proud of his xml pseudo-parsing). Polished by Justasic.
 */

/**
 * \defgroup weatherM Weather Module
 * This is the module for the weather function.
 * For a better description see the function description.
 * \section commands Commands associated with this module.
 * \subsection weather !weather
 * Follow \a !weather with either a U.S. postal code or a city name and country/state name.
 * @{
 */



/**
 * \fn weather(bool act):module("Weather", act, PRIORITY_DONTCARE){ this->SetDesc("Shows the weather for your location"); }
 * \brief Says the weather.
 * This is a good example of how to use the xml pseudo-parser we have in the \a flux_net_irc namespace.
 * If \a !weather is said then it takes whatever follows that and uses wget to get an xml file from
 * the google weather api. From there it pseudo-parses the file using functions from \a flux_net_irc
 * and throws the needed contents out of the main socket as a private message (using \a privmsg).
 */
class CommandCWeather : public Command
{
public:
  CommandCWeather():Command("!WEATHER", 1, 1)
  {
    this->SetDesc("Displays the weather");
    this->SetSyntax("\2location\2");
  }
  void Run(CommandSource &source, const Flux::vector &params)
  {
    User *u = source.u;
    Channel *c = source.c;
    Flux::string area = params[0], tmpfile = TextFile::TempFile(Config->Binary_Dir+"/runtime/navn_xml.tmp.XXXXXX"), wget;
    if(tmpfile.empty()){
      Log() << "Failed to get temp file";
      return;
    }
    area.trim();
    wget = "wget -q -O "+tmpfile+" - http://www.google.com/ig/api?weather="+(area.is_number_only()?area:area.url_str());
    system(wget.c_str());
    XMLFile *xf = new XMLFile(tmpfile);

    Flux::string city = xf->Tags["xml_api_reply"].Tags["weather"].Tags["forecast_information"].Tags["city"].Attributes["data"].Value;
    Flux::string condition = xf->Tags["xml_api_reply"].Tags["weather"].Tags["current_conditions"].Tags["condition"].Attributes["data"].Value;
    Flux::string temp_f = xf->Tags["xml_api_reply"].Tags["weather"].Tags["current_conditions"].Tags["temp_f"].Attributes["data"].Value;
    Flux::string temp_c = xf->Tags["xml_api_reply"].Tags["weather"].Tags["current_conditions"].Tags["temp_c"].Attributes["data"].Value;
    Flux::string humidity = xf->Tags["xml_api_reply"].Tags["weather"].Tags["current_conditions"].Tags["humidity"].Attributes["data"].Value;
    Flux::string windy = xf->Tags["xml_api_reply"].Tags["weather"].Tags["current_conditions"].Tags["wind_condition"].Attributes["data"].Value;

    delete xf;
    c->SendMessage("%s Current Condition: %s, %s, %s, %s °F %s °C", city.strip().c_str(), condition.strip().c_str(), humidity.strip().c_str(), windy.strip().c_str(), temp_f.c_str(), temp_c.c_str());
    Log(u, this) << "to get weather for area '" << area << "'";
  }
  bool OnHelp(CommandSource &source, const Flux::string &nill)
  {
    this->SendSyntax(source);
    source.Reply(" ");
    source.Reply("This command provides the weather for\n"
		 "the requested location. Powered by google,\n"
		 "You may use a city name or area code or any\n"
		 "other location identifing text\n");
    return true;
  }
};
class weather:public module{
  CommandCWeather rainy;
public:
  weather(const Flux::string &Name):module(Name){
    this->SetAuthor("Lordofsraam");
    this->SetVersion(VERSION);
    this->AddChanCommand(&rainy);
  }
};

/**
 * @}
 */
MODULE_HOOK(weather)
