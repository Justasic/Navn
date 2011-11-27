#include "flux_net_irc.hpp"

class cooldowntimer : public Timer
{
public:
  int requested;
  cooldowntimer():Timer(300, time(NULL), true){ }
  void Tick(time_t){
    Log(LOG_TERMINAL) << Log::TimeStamp() << " Requests reset";
    requested = 0;
  }
};

class encyclopedia : public module
{
  cooldowntimer cdt; //Make a cool down timer so we dont request too many too fast and get us banned from wikipedia!
private:
  Flux::string query;
  void Sanitize(Flux::string &victim)
  {
    victim = victim.replace_all_cs("&","");
    victim = victim.replace_all_cs("|","");
    victim = victim.replace_all_cs(">","");
    victim = victim.replace_all_cs("<","");
    victim = victim.replace_all_cs("!","");
    victim = victim.replace_all_cs("(","");
    victim = victim.replace_all_cs(")","");
    victim = victim.replace_all_cs("*","");
    victim = victim.replace_all_cs("{","");
    victim = victim.replace_all_cs("}","");
    victim = victim.replace_all_cs("`","");
    victim = victim.replace_all_cs("\"","");
    victim = victim.replace_all_cs("\'","");
    victim = victim.replace_all_cs(".","");
    victim = victim.replace_all_cs("?","");
  }
  Flux::string TooManyRequests()
  {
    Flux::string Errors[] = { //These errors must be generic for any user, whether its their 1st time asking or their 30'th
      "I am sorry but it seems you've requested too many times, please try again later",
      "It seems I have been asked too many questions, I cannot answer them all!",
      "The answer to life the universe and everything is 42, so please stop asking!",
      "Why are humans so curious? Some of these questions are hard, I need a break",
      "You have simply asked too many questions for me to answer"
    };
    return Errors[randint(0,4)];
  }
  Flux::string RandomOops()
  {
    Flux::string Errors[] = {
      "I'm sorry, I don't know anything about that.",
      "Forgive me, I cannot seem to find anything about that subject matter.",
      "Pardon my ignorance, I am drawing a blank on the subject."
    };
    return Errors[randint(0,2)];
  }
  Flux::string RandomAmb()
  {
    Flux::string Errors[] = {
      "Please be a little more specific.",
      "Sorry, but that was a little ambiguous. Please specify what you are referring to.",
      "That was a little vague. Try one more time?"
    };
    return Errors[randint(0,2)];
  }
  void Brain(User *u, Flux::string q)
  {
    if(!(cdt.requested >= 30))
    {
      cdt.requested++;
      Log(LOG_TERMINAL) << "Requests: " << cdt.requested;
      Sanitize(q);
      Flux::string str = "python brain.py "+q;
      Flux::string information = execute(str.c_str());
      information.trim();
      if (information.search_ci("For search options, see Help:Searching")) u->SendMessage(RandomOops());
      else if (information.search_ci("may refer to:") ||  information.search_ci("reasons this message may be displayed:") ) u->SendMessage(RandomAmb());
      else u->SendMessage(information.strip());
    }else
      u->SendMessage(TooManyRequests());
  }
  void SetQuery(unsigned n, const std::vector<Flux::string> &params)
  {
    query.clear();
    if (n < params.size()) for(unsigned i=n; i < params.size(); ++i) query += params[i]+' ';
  }

public:
  encyclopedia(const Flux::string &Name):module(Name)
  {
    this->SetAuthor("Lordofsraam");
    this->SetVersion(VERSION);
    this->SetPriority(PRIORITY_LAST);
    ModuleHandler::Attach(I_OnPrivmsg, this);
  }
  void OnPrivmsg(User *u, Channel *c, const std::vector<Flux::string> &params)
  {
    //std::vector<Flux::string> MessageParams = StringVector(params, ' ');
    Flux::string msg;
    for(unsigned i=0; i < params.size(); ++i)
      msg += params[i]+' ';

    Flux::string cmd = params.empty()?"":params[0];
    
    if(cmd.equals_ci("!encyclopedia"))
    {
      SetQuery(1, params);
      Brain(u,query);
    }
    if(msg.search_ci(Config->BotNick+", what do you know about "))
    {
      SetQuery(6, params);
      Brain(u, query);
    }
    else if(msg.search_ci(Config->BotNick+", what is a ") ^ msg.search_ci(Config->BotNick+", what is the") ^ msg.search_ci(Config->BotNick+", tell me about ") ^ msg.search_ci(Config->BotNick+", who are the ") ^ msg.search_ci(Config->BotNick+", what is an "))
    {
      SetQuery(4, params);
      Brain(u, query);
    }
    else if(msg.search_ci(Config->BotNick+", what is ") ^ msg.search_ci(Config->BotNick+", what are ") ^ msg.search_ci(Config->BotNick+", who is ") ^ msg.search_ci(Config->BotNick+", what's a ") ^ msg.search_ci(Config->BotNick+", what's an "))
    {
      SetQuery(3, params);
      Brain(u, query);
    }
    else if(msg.search_ci(Config->BotNick+", tell me what you know about "))
    {
      SetQuery(7, params);
      Brain(u, query);
    }
  }
};
MODULE_HOOK(encyclopedia)