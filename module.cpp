#include <module.h>
/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
//This code sucks, you know it and I know it. 
//Move on and call me an idiot later.
std::vector<module*> moduleList;
/** 
 * \fn module::module(Flux::string n, bool a, ModulePriority p)
 * \brief Module Constructor
 * \param name Name of the module
 * \param activated Wether the module is activated or not
 * \param priority The module priority
 */
module::module(Flux::string n, bool a, ModulePriority p){
  name = n;
  activated = a;
  priority = p;
  moduleList.push_back(this);
}
/** 
 * \fn void module::SetDesc(const Flux::string &desc)
 * \brief Sets the module description
 * \param desc The description for the module
 */
void module::SetDesc(const Flux::string &description){
  this->desc = description;
}
/** 
 * \fn const Flux::string module::GetDesc() const
 * \brief Get the modules description
 */
const Flux::string module::GetDesc() const{
 return this->desc; 
}
/* commands stuff */
CommandMap Commandsmap;
/** 
 * \fn int module::AddCommand(Command *c)
 * \brief Adds commands from modules into the bot
 * \param command The command to add
 */
int module::AddCommand(Command *c){
 if(!c)
   return 1;
 std::pair<CommandMap::iterator, bool> it = Commandsmap.insert(std::make_pair(c->name, c));
 if(it.second != true){
   log("Command %s already loaded!", c->name.c_str());
   return 2;
 }
 c->mod = this;
 return 0;
}
/** 
 * \fn int module::DelCommand(Command *c)
 * \brief delete commands from modules in the bot
 * \param command The command to add
 */
int module::DelCommand(Command *c){
  if(!c)
    return 1;
  if(!Commandsmap.erase(c->name))
    return 2;
  c->mod = NULL;
  return 0;
}
/*******************************************************************/
/** 
 * \fn Command *FindCommand(const Flux::string &name)
 * \brief Find a command in the command map
 * \param name A string containing the command name you're looking for
 */
Command *FindCommand(const Flux::string &name){
 if(name.empty())
   return NULL;
 CommandMap::iterator it = Commandsmap.find(name);
 if(it != Commandsmap.end())
   return it->second;
 return NULL;
}
/** 
 * \fn module *FindModule(const Flux::string &name)
 * \brief Find a module in the module vector
 * \param name A string containing the module name you're looking for
 */
module *FindModule(const Flux::string &name){
 for(std::vector<module*>::const_iterator it = moduleList.begin(), it_end = moduleList.end(); it != it_end; ++it){
  module *m = *it;
  if(m->name == name)
    return m;
 }
 return NULL;
}
/*******************************************************************/

std::vector<module *> ModuleHandler::EventHandlers[I_END];
/** 
 * \fn bool ModuleHandler::Attach(Implementation i, module *mod)
 * \brief Module hook for the FOREACH_MOD macro
 * \param Implementation The Implementation of the call list you want your module to have
 * \param Module the module the Implementation is on
 */
bool ModuleHandler::Attach(Implementation i, module *mod){
  if(std::find(EventHandlers[i].begin(), EventHandlers[i].end(), mod) != EventHandlers[i].end())
    return false;
  EventHandlers[i].push_back(mod);
  return true;
}
/** 
 * \fn bool ModuleHandler::Detach(Implementation i, module *mod)
 * \brief Unhook for the module hook ModuleHandler::Attach()
 * \param Implementation The Implementation of the call list you want your module to detach
 * \param Module the module the Implementation is on
 */
bool ModuleHandler::Detach(Implementation i, module *mod){
  std::vector<module*>::iterator x = std::find(EventHandlers[i].begin(), EventHandlers[i].end(), mod);
  
  if(x == EventHandlers[i].end())
    return false;
  EventHandlers[i].erase(x);
  return true;
}