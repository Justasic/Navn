/* Navn IRC bot -- POSIX sysinfo emulator for windows
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */

#pragma once
#ifndef SYSINFO_H
#define SYSINFO_H
#ifdef _WIN32
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

extern CoreExport int sysinfo(struct sysinfo *);

#endif // _WIN32
#endif // SYSINFO_H