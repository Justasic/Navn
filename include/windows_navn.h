/* Navn IRC bot -- Windows shit
 * 
 * (C) 2011-2012 Flux-Net
 * Contact us at Dev@Flux-Net.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */

#ifndef WINDOWS_H
#define WINDOWS_H
#ifdef _WIN32
	/* include the various windows libraries for compile */
	#include <winsock2.h>
	#include <ws2tcpip.h>
	#include <windows.h>
	#include <sys/timeb.h>
	#include <direct.h>
	#include <windows.h>
	#include <execption>
	#include <new>
	#include <new.h>
	#include <stdio.h>

	/* we have our own read and write */
	#define read read_not_used
	#define write write_not_used
	#include <io.h>
	#undef read
	#undef write

	/* redefine some functions because microsoft has their own standards */
	#define MARK_DEPRECATED
	#define DEPRECATED x
	#define RTLD_LAZY 0
	#define F_GETFL 0
	#define F_SETFL 1
	#define O_NONBLOCK 1
	#define snprintf _snprintf
	#define vsnprintf _vsnprintf
	#define stat _stat
	#define S_ISREG(x) ((x) & _S_IFREG)
	#define GetCurrentDir(x, y) GetCurrentDirectory(y, x)

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
	
	/* reinvent some functions as they don't exist in windows */
	extern void *dlopen(const char*, int);
	extern char *dlerror(void);
	extern void *dlsym(void*, const char*);
	extern int dlclose(void*);
	extern CoreExport int read(int, char*, size_t);
	extern CoreExport int write(int, const char*, size_t);
	extern CoreExport int windows_close(int);
	extern CoreExport int windows_accept(int, struct sockaddr*, int*);
	extern CoreExport int windows_inet_pton(int, const char*, void*);
	extern CoreExport const char *windows_inet_ntop(int, const void*, char*, size_t);
	extern CoreExport int fcntl(int, int, int);

	struct dirent
	{
		int d_ino;
		char *d_name;
	};
	struct DIR
	{
		dirent ent;
		HANDLE handle;
		WIN32_FIND_DATA data;
		bool read_first;
	};
	DIR *opendir(const char*);
	dirent *readdir(DIR*);
	int closedir(DIR*);

	#define MODULE_INIT(x) \
	extern "C" DllExport Module *ModInit(const Flux::string&); \
	extern "C" DllExport void ModunInit(x *); \
	extern "C" Module *ModInit(const Flux::string &name) { return new x(name); } \
	BOOLEAN WINAPI DllMain(HINSTANCE, DWORD nReason, LPVOID) { return TRUE; } \
	extern "C" void ModunInit(x *m) { if(m) delete m; }
#else // *nix
	//If we're compiling with C++ 11
	#if defined(__GXX_EXPERIMENTAL_CXX0X__) && defined(HAVE_CXX_11)
	# define _CXX11
	# include <atomic>
	# include <thread>
	#endif
	
	#define GetCurrentDir getcwd
	#define Delete unlink
	#define MODULE_HOOK(x) \
	extern "C" module *ModInit(const Flux::string &name) { return new x(name); } \
	extern "C" void ModunInit(x *m) { if(m) delete m; }
#endif // _WIN32

#endif // WINDOWS_H