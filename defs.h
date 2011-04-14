#ifndef defs_h
#define defs_h
#include "includes.h"

#define MST (-6)
#define UTC (0)
#define CCT (+8)
#define EST (-4)
#define PST (-7)
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
const string version = "v1.3.7";

    /*
    'Reply' means 'What the server is saying back'
    'unick' means that whatever user is saying to the bot or whoever said
    something or did something.
    Note the 'reply' is the raw IRC server reply incase you want
    to expand on my code
    */
string unick;
string rply;
string host;
string arg;
string chan;
string msg;
string quitmsg;
string fullhost;
string ident;
string myhost;
/******************Configuration variables***********************/
string logfile;
string nsacc;
string nspass;
string owner_nick;
string realname;
string usrname;
string nick;
string channel;
string port;
string server;
string pid_file;
string usrpass;

void ReadConfig(INIReader &config){
logfile = config.Get("Log","Log_File","navn.log");
nsacc = config.Get("Bot","NickServ_Account","");
nspass = config.Get("Bot","NickServ_Password","");
owner_nick = config.Get("Bot","Owner","Derp");
realname = config.Get("Connect","Realname","The Navn Bot "+version);
usrname = config.Get("Connect","Ident","Navn");
nick = config.Get("Bot","Nick","Navn");
channel = config.Get("Bot","Channel","#Test");
port = config.Get("Connect","Port","6667");
server = config.Get("Connect", "Server", "irc.flux-net.net");
pid_file = config.Get("Log","PID_File","navn.pid");
usrpass = config.Get("Bot","Password","Navn");
}
/******************End Configuration variables********************/

const string welcome_msg = nick+" has connected. Type !time to see the time.";
const string kick_msg = "KICK "+channel+" "+nick;
//433 replies that the nick is taken
const string nick_taken_rsl = "433 * "+nick+" :Nickname is already in use.";
const string in_the_channel = "252 "+nick;
const string ChanJoin = "366"+nick;
//005 is just the generic welcome message saying you have logged in to the server.
const string server_welcome = "005 "+nick;
const string killed = "KILL "+nick;
const string con_closed_nick = "[Registration timeout]";
const string CTCP_VERS = " :\001VERSION\001";
const string CTCP_TIME = " :\001TIME\001";
const string join_req = "PRIVMSG "+nick+" :join";
const string part_req = "PRIVMSG "+nick+" :part";
const string pass_req = "PRIVMSG "+nick+" :pass";
const string gdb_req = "PRIVMSG "+nick+" :gdb";
const string quitmsg_req = "PRIVMSG "+nick+" :quit";
const string pmsggoogle = "PRIVMSG "+nick+" :google";
const string pmsgyoutube = "PRIVMSG "+nick+" :youtube";
const string gdb_msg = "gdb#: (1) From the directory that you would do ./program from, do the following instead: gdb ./program (2) Do: r -paramiters (3) Upon crash, do: bt full to see all the output of the crash.";
const string access_denied = "Access is Denied.";
const string nl = "\r\n";
/**Command Line Help
 * for when someone does --help or -h with the bot
 * @param ./navn --help
 * @param ./navn -h
 */
void help(){
 cout << "\t Navn Internet Relay Chat Bot" << nl;
 cout << "\t \t "<< version << nl; 
 cout << nl;
 cout << "Paramiters \t   Function" << nl; 
 cout << "--help \t -h \t Displays this message." << nl; 
 cout << "--developer  -d\t Starts the bot in debug mode." << nl; 
 cout << "--dev \t -x \t Starts the bot in developer mode." << nl; 
 cout << "--nofork \t-f \t Keep" << nl;
 cout << "" <<nl;
 cout << nl;
 cout << "\t This bot does have Epic Powers." << nl;
 exit(0);
}

struct sysinfo sys_info;

#endif
