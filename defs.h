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
const string version = "v1.3.6";

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
//make sure the channel name is the same case as the one on IRC
/**************************configuration***********************/
string server = "irc.Flux-Net.net";
string port = "6667";
string channel = "#Test";
string nick = "Navn";
string usrname = "Navn";
string realname = "The Navn Bot "+version;
string owner_nick = "Justasic";
string nspass = "letmein";
string nsacc = "Navn";
string logfile = "navn.log";
/**************************configuration***********************/
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
const string CTCP_VERS = "PRIVMSG "+nick+" :\001VERSION\001";
const string CTCP_TIME = "PRIVMSG "+nick+" :\001TIME\001";
const string help_req = "PRIVMSG "+nick+" :help";
const string join_req = "PRIVMSG "+nick+" :join";
const string part_req = "PRIVMSG "+nick+" :part";
const string pass_req = "PRIVMSG "+nick+" :pass";
const string restart_req = "PRIVMSG "+nick+" :restart";
const string gdb_req = "PRIVMSG "+nick+" :gdb";
const string quitmsg_req = "PRIVMSG "+nick+" :quit";
const string pmsggoogle = "PRIVMSG "+nick+" :google";
const string pmsgyoutube = "PRIVMSG "+nick+" :youtube";
const string gdb_msg = "gdb#: (1) From the directory that you would do ./program from, do the following instead: gdb ./program (2) Do: r -paramiters (3) Upon crash, do: bt full to see all the output of the crash.";
const string access_denied = "Access Denied.";
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
