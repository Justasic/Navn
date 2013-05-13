/* Navn IRC bot -- POSIX sysinfo emulator for windows
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
#include <tlhelp32.h>

// Since windows doesn't seem to have a simple "Number of current processes" available
// we'll just have to make our own >:(
inline int GetProcessList()
{
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;
	long Processes = 0;

	// Snapshot the current processes and make sure it's valid
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if(hProcessSnap == INVALID_HANDLE_VALUE)
		return Processes;

	// Set the size of the structure before using it.
	pe32.dwSize = sizeof(PROCESSENTRY32);

	// Retrieve information about the first process,
	// and exit if unsuccessful
	if(!Process32First(hProcessSnap, &pe32))
	{
		CloseHandle(hProcessSnap);          // clean the snapshot object
		return Processes;
	}

	// Count the total number of processes
	do
	{
		Processes++;
	}
	while(Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);

	return Processes;
}

int sysinfo(struct sysinfo *info)
{
	SYSTEM_INFO si;
	MEMORYSTATUSEX statex;

	ZeroMemory(&si, sizeof(SYSTEM_INFO));
	statex.dwLength = sizeof(statex);
	GetSystemInfo(&si);

	if(!GlobalMemoryStatusEx(&statex))
		return -1;

	// System Uptime
	info->uptime = GetTickCount64() / 1000 % 60;

	// Load times - windows does not have this so say -1 or 0 or nothing basically
	info->loads[0] = -1;
	info->loads[1] = -1;
	info->loads[2] = -1;

	// Ram usages - note that these may not be exact to what linux has
	info->freeram = statex.ullAvailPhys;
	info->freeswap = statex.ullAvailVirtual;
	info->sharedram = 0;
	info->totalram = statex.ullTotalPhys;
	info->bufferram = statex.ullTotalPageFile;
	info->totalswap = statex.ullTotalVirtual;

	// Processes
	info->procs = GetProcessList();
	return 0;
}
#endif