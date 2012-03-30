/* Navn IRC bot -- Weather module
 * 
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */
#include "modules.h"

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
  CommandCWeather(module *m):Command(m, "!WEATHER", C_CHANNEL, 1, 1)
  {
    this->SetDesc("Displays the weather");
    this->SetSyntax("\2location\2");
  }
  void Run(CommandSource &source, const Flux::vector &params)
  {
    User *u = source.u;
    Channel *c = source.c;
    Flux::string area = params[0], tmpfile = TextFile::TempFile(Config->Binary_Dir+"/runtime/navn_xml.tmp.XXXXXX");
    
    if(tmpfile.empty())
    {
      Log() << "Failed to get temp file";
      return;
    }
    area.trim();

    Flux::string url = Config->Parser->Get("Modules", "WeatherURL", "http://www.google.com/ig/api?weather=%l");
    url = url.replace_all_cs("%l", area.is_number_only()?area:area.url_str());

    Log(LOG_DEBUG) << "wget weather url \"" << url << "\" for !weather command used";
    system(Flux::string("wget -q -O "+tmpfile+" - "+url).c_str());
    XMLFile *xf = new XMLFile(tmpfile);

    Flux::string city = xf->Tags["xml_api_reply"].Tags["weather"].Tags["forecast_information"].Tags["city"].Attributes["data"].Value;
    Flux::string condition = xf->Tags["xml_api_reply"].Tags["weather"].Tags["current_conditions"].Tags["condition"].Attributes["data"].Value;
    Flux::string temp_f = xf->Tags["xml_api_reply"].Tags["weather"].Tags["current_conditions"].Tags["temp_f"].Attributes["data"].Value;
    Flux::string temp_c = xf->Tags["xml_api_reply"].Tags["weather"].Tags["current_conditions"].Tags["temp_c"].Attributes["data"].Value;
    Flux::string humidity = xf->Tags["xml_api_reply"].Tags["weather"].Tags["current_conditions"].Tags["humidity"].Attributes["data"].Value;
    Flux::string windy = xf->Tags["xml_api_reply"].Tags["weather"].Tags["current_conditions"].Tags["wind_condition"].Attributes["data"].Value;

    delete xf;
    
    if(city.strip().empty())
    {
      source.Reply("Weather information for \2%s\2 not found.", area.c_str());
      return;
    }
    int temp_k = (int)temp_c + 273; // Calculate degrees kelvin from degrees celsius
    c->SendMessage("%s Current Condition: %s, %s, %s, %s °F %s °C %i °K", city.strip().c_str(), condition.strip().c_str(), humidity.strip().c_str(), windy.strip().c_str(), temp_f.c_str(), temp_c.c_str(), temp_k);
    
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
  weather(const Flux::string &Name):module(Name), rainy(this)
  {
    this->SetAuthor("Lordofsraam");
    this->SetVersion(VERSION);
  }
};

/**
 * @}
 */
MODULE_HOOK(weather)
