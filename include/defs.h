/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#ifndef defs_h
#define defs_h
#include "INIReader.h"
#include "extern.h"
#include "flux.h"


/*List the constants used throughout the script.
If you wanna edit the server, channel, nick, etc that the bot
connects to, do so here, NOT in the main code.
*/
#ifdef HAVE_SETJMP_H
jmp_buf sigbuf;
#endif
char **my_av, **my_envp;
bool nofork, dev, protocoldebug, IsOper, quitting, started = false;
Flux::string binary_path, bot_bin, binary_dir, quitmsg, server_name;
const Flux::string password = make_pass();
char segv_location[255];
time_t starttime = 0;
/**********************Version Variables*************************/
const Flux::string VERSION_LONG = Flux::stringify(VERSION)+"-"+Flux::stringify(VERSION_GIT)+Flux::stringify(VERSION_EXTRA);
#endif

