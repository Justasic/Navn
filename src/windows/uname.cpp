/* Navn IRC bot -- POSIX uname emulator for windows
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */

#ifdef _WIN32
#include "windows_navn.h"

extern Flux::string printfify(const char *, ...);

int uname(struct utsname *info)
{
	// get the system information.
	OSVERSIONINFOEX wininfo;
	SYSTEM_INFO si;
	Flux::string WindowsVer = GetWindowsVersion();
	Flux::string cputype;
	char hostname[256] = "\0";
	ZeroMemory(&wininfo, sizeof(OSVERSIONINFOEX));
	ZeroMemory(&si, sizeof(SYSTEM_INFO));
	wininfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	if(!GetVersionEx(reinterpret_cast<OSVERSIONINFO *>(&wininfo)))
		return -1;

	GetSystemInfo(&si);

	// Get the hostname
	if(gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR)
		return -1;

	if(si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
		cputype = "64-bit";
	else if(si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL)
		cputype = "32-bit";
	else if(si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
		cputype = "Itanium 64-bit";

	// Fill the utsname struct with the windows system info
	strcpy(info->sysname, "Windows");
	strcpy(info->nodename, hostname);
	strcpy(info->release, WindowsVer.c_str());
	strcpy(info->version, printfify("%ld.%ld-%ld", wininfo.dwMajorVersion, wininfo.dwMinorVersion, wininfo.dwBuildNumber).c_str());
	strcpy(info->machine, cputype.c_str());

	// Null-Terminate
	info->nodename[strlen(info->nodename) - 1] = '\0';
	info->sysname[strlen(info->sysname) - 1] = '\0';
	info->release[strlen(info->sysname) - 1] = '\0';
	info->version[strlen(info->version) - 1] = '\0';
	info->machine[strlen(info->machine) - 1] = '\0';
}
#endif