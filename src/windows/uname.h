/* Navn IRC bot -- POSIX uname emulator for windows
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */

#pragma once
#ifndef UNAME_H
#define UNAME_H
#ifdef _WIN32
#include "flux.h"

struct CoreExport utsname
{
	// why is this an incomplete type but machine is not?
	char sysname[2048];    /* Operating system name (e.g., "Linux") */
	char nodename[2048];   /* Name within "some implementation-defined network" */
	char release[2048];    /* OS release (e.g., "2.6.28") */
	char version[2048];    /* OS version */
	char machine[];        /* Hardware identifier */
};

extern CoreExport int uname(struct utsname *);

#endif // _WIN32
#endif // UNAME_H