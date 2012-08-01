/* Navn IRC bot -- Windows shit
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */

#pragma once
#ifndef WINDOWS_H
#define WINDOWS_H
#ifdef _WIN32
	/* include the various windows libraries for compile */
 	// Don't include half the development libraries into the bot, they're not needed
 	#define WIN32_LEAN_AND_MEAN
 	// Stop windows.h from including winsock.h since winsock.h and winsock2.h are incompatible
 	#define _WINSOCKAPI_
 	// Prevent min() and max() from being defined
 	#define NOMINMAX
 	#include <WinSock2.h>
 	#include <ws2tcpip.h>
 	#include <windows.h>
	#include <io.h>
	#include <wchar.h>
	#include <process.h>
	#include <fcntl.h>
 	#include <sys/timeb.h>
	#include <sys/stat.h>
	#include <direct.h>
	#include <exception>
	#include <new>
	#include <new.h>
	#include <stdio.h>
 	#include "flux.h"
 	#include "SocketException.h"

	/* we have our own read and write */
	#define read read_not_used
	#define write write_not_used
	#undef read
	#undef write

	/* redefine some functions and variables because microsoft has their own standards */
	//#define DEPRECATED(x) x
	#define RTLD_LAZY 0
	#define RTLD_LOCAL 0
	#define F_GETFL 0
	#define F_SETFL 1
	#define O_NONBLOCK 1
	#define MSG_NOSIGNAL 0
	#define popen _popen
	#define pclose _pclose
	#define O_WRONLY _O_WRONLY
	#define O_CREAT _O_CREAT
	#define S_IREAD _S_IREAD
	#define S_IWRITE _S_IWRITE
	#define snprintf    _snprintf
	#define vsnprintf   _vsnprintf
	#define strcasecmp  _stricmp
	#define strncasecmp _strnicmp
	#define execve _execve
	#define Delete _unlink
	#define getpid GetCurrentProcessId
	#define S_ISREG(mode)  (((mode) & S_IFMT) == S_IFREG)
 	#define S_ISDIR(mode)  (((mode) & S_IFMT) == S_IFDIR)
 	#define sleep(x) Sleep(x * 1000)
	#define GetCurrentDir(x, y) GetCurrentDirectory(y, x)
 	#define getcwd(x, y) GetCurrentDirectory(y, x)

 	// We have our own strerror handler, move windows' to the void.
	#ifdef strerror
	# define strerror _strerror
	# undef strerror
	#endif
	extern CoreExport const char *decodeerrno(int);
	#define strerror decodeerrno

	#ifdef EINPROGRESS
	# undef EINPROGRESS
	#endif
	#define EINPROGRESS WSAEWOULDBLOCK

	// System info stuff
	struct CoreExport sysinfo
	{
		long uptime;             /* Seconds since boot */
		unsigned long loads[3];  /* 1, 5, and 15 minute load averages */
		unsigned long totalram;  /* Total usable main memory size */
		unsigned long freeram;   /* Available memory size */
		unsigned long sharedram; /* Amount of shared memory */
		unsigned long bufferram; /* Memory used by buffers */
		unsigned long totalswap; /* Total swap space size */
		unsigned long freeswap;  /* swap space still available */
		unsigned short procs;    /* Number of current processes */
		char _f[22];             /* Pads structure to 64 bytes */
	};

	struct CoreExport utsname
	{
		// why is this an incomplete type but machine is not?
		char sysname[2048];    /* Operating system name (e.g., "Linux") */
		char nodename[2048];   /* Name within "some implementation-defined network" */
		char release[2048];    /* OS release (e.g., "2.6.28") */
		char version[2048];    /* OS version */
		char machine[];    /* Hardware identifier */
	};

	/* reinvent some functions as they don't exist in windows */
	extern void *dlopen(const char*, int);
	extern char *dlerror(void);
	extern void *dlsym(void*, const char*);
	extern int dlclose(void*);
	extern CoreExport int setenv(const char *name, const char *value, int overwrite);
	extern CoreExport int unsetenv(const char *name);
	extern CoreExport int gettimeofday(timeval *tv, void*);
	extern CoreExport int sysinfo(struct sysinfo *);
	extern CoreExport int uname(struct utsname *);
	extern CoreExport int mkstemp(char *input);
	extern CoreExport int read(int, char*, size_t);
	extern CoreExport int write(int, const char*, size_t);
	extern CoreExport int windows_close(int);
	extern CoreExport int windows_accept(int, struct sockaddr*, int*);
	extern CoreExport int windows_inet_pton(int, const char*, void*);
	extern CoreExport const char *windows_inet_ntop(int, const void*, char*, size_t);
	extern CoreExport int fcntl(int, int, int);

	// Windows socket bullshit
	class CoreExport WSAInitialization : public Base
	{
	public:
		WSADATA wsa;
		WSAInitialization()
		{
			if(WSAStartup(MAKEWORD(2, 0), &wsa))
				throw CoreException("Failed to initialize WinSock library");
		}

		~WSAInitialization()
		{
			WSACleanup();
		}
	};

	#define SET_SEGV_LOCATION() snprintf(segv_location, sizeof(segv_location), "%s %d %s", __FILE__, __LINE__, __FUNCTION__);

	// We have our own Directory information and crap
	struct CoreExport dirent
	{
		int d_ino;
		char *d_name;
	};

	struct CoreExport DIR
	{
		dirent ent;
		HANDLE handle;
		WIN32_FIND_DATA data;
		bool read_first;
	};

	extern CoreExport DIR *opendir(const char*);
	extern CoreExport dirent *readdir(DIR*);
	extern CoreExport int closedir(DIR*);

	// Module hook for windows
	#define MODULE_HOOK(x) \
	extern "C" DllExport Module *ModInit(const Flux::string&); \
	extern "C" DllExport void ModunInit(x *); \
	extern "C" Module *ModInit(const Flux::string &name) { return new x(name); } \
	BOOLEAN WINAPI DllMain(HINSTANCE, DWORD nReason, LPVOID) { return TRUE; } \
	extern "C" void ModunInit(x *m) { if(m) delete m; }
#else // *nix
	//If we're compiling with C++ 11
	#ifdef __GXX_EXPERIMENTAL_CXX0X__
	# define _CXX11
	# include <atomic>
	# include <thread>
	#endif // ifndef __GXX_EXPERIMENTAL_CXX0X__

	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netdb.h>
	#include <unistd.h>
	#include <arpa/inet.h>
	#include <arpa/nameser.h>
	#include <resolv.h>
	#include <dirent.h>
	#include <unistd.h>
	#include <sys/sysinfo.h>
	#include <sys/utsname.h>
	#include <sys/time.h>
	#include <alloca.h>
	#include <sys/select.h>
	#include "flux.h"

	#define SET_SEGV_LOCATION() snprintf(segv_location, sizeof(segv_location), "%s %d %s", __FILE__, __LINE__, __PRETTY_FUNCTION__);

	#define GetCurrentDir getcwd
	#define Delete unlink
	#define MODULE_HOOK(x) \
	extern "C" Module *ModInit(const Flux::string &name) { return new x(name); } \
	extern "C" void ModunInit(x *m) { if(m) delete m; }
#endif // _WIN32

#endif // WINDOWS_H
