#ifndef defs_h
#define defs_h
#include "includes.h"

#define GetCurrentDir getcwd
using namespace std;

/*List the constants used throughout the script.
If you wanna edit the server, channel, nick, etc that the bot
connects to, do so here, NOT in the main code.
*/
char **my_av;
char ** my_envp;
int days, hours, mins;
bool in_channel = false;
bool nofork = false;
bool dev = false;
bool quitting = false;
const Flux::string version = "2.0.1";
bool IsOper = false;
Flux::string binary_path, bot_bin, binary_dir;
/**Runtime directory finder
 * This will get the bots runtime directory
 * @param getprogdir(const Flux::string dir)
 */
Flux::string getprogdir(const Flux::string dir){
  char buffer[FILENAME_MAX];
  if (GetCurrentDir(buffer, sizeof(buffer))) {
    Flux::string remainder = dir;
    bot_bin = remainder;
    Flux::string::size_type n = bot_bin.rfind("/");
    Flux::string fullpath;
    if (bot_bin[0] == '/')
      fullpath = bot_bin.substr(0, n);
    else
      fullpath = Flux::string(buffer) + "/" + bot_bin.substr(0, n);
    bot_bin = bot_bin.substr(n + 1, remainder.length());
    return fullpath;
  }
  return "/";
}

Flux::string unick;
Flux::string rply;
Flux::string host;
Flux::string chan;
Flux::string raw;
Flux::string msg;
Flux::string quitmsg;
Flux::string fullhost;
Flux::string ident;
Flux::string myhost;
/******************Configuration variables***********************/
Flux::string logfile;
Flux::string nsacc;
Flux::string nspass;
Flux::string owner_nick;
Flux::string realname;
Flux::string usrname;
Flux::string nick;
Flux::string channel;
Flux::string port;
Flux::string server;
Flux::string LogChannel;
Flux::string pid_file;
Flux::string usrpass;
Flux::string ouser;
Flux::string opass;
/**Rehash void
 * This will re-read the config file values when told to do so
 * @param ReadConfig(INIReader &config)
 */
void ReadConfig(INIReader &config){
logfile = config.Get("Log","Log_File","navn.log");
nsacc = config.Get("Bot","NickServ_Account","");
nspass = config.Get("Bot","NickServ_Password","");
owner_nick = config.Get("Bot","Owner","Derp");
realname = config.Get("Connect","Realname","The Navn Bot "+version.tostd());
usrname = config.Get("Connect","Ident","Navn");
nick = config.Get("Bot","Nick","Navn");
channel = config.Get("Bot","Channel","#Test");
port = config.Get("Connect","Port","6667");
server = config.Get("Connect", "Server", "irc.flux-net.net");
pid_file = config.Get("Log","PID_File","navn.pid");
usrpass = config.Get("Bot","Password","Navn");
ouser = config.Get("Oper","Oper_Username","");
opass = config.Get("Oper","Oper_Password","");
LogChannel = config.Get("Modules", "LogChannel","");
}
/******************End Configuration variables********************/

#define welcome_msg "%s has joined. Type '/msg %s help' to see a list of commands."
const Flux::string in_the_channel = "252 "+nick;
const Flux::string ChanJoin = "366"+nick;
//005 is just the generic welcome message saying you have logged in to the server.
const Flux::string access_denied = "Access is Denied.";
const Flux::string nl = "\n";

/**Command Line Help
 * for when someone does --help or -h with the bot
 * @param ./navn --help
 * @param ./navn -h
 */
void help(char** argv){
 printf("Navn Internet Relay Chat Bot v%s\n", version.c_str());
 printf("Usage: %s [options]\n", argv[0]);
 printf("-h, --help\n");
 printf("-d, --developer\n");
 printf("-f, --nofork\n");
 printf("This bot does have Epic Powers.\n");
 exit(0);
}

#endif

