/* Navn IRC bot -- General includes
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */
#pragma once

/**
 *\file includes.h Inlcludes header file
 *\brief Automatically includes all of the things we've used so far.
 *\author Justasic
 * The only use of this file is to fix the includes hell that
 * Justasic was having earlier. if you can fix it an easier
 * way then do so.
 */
#include "Socket.h"
#include "timers.h"
#include "SocketException.h"
#include "thread.h"
#include "INIReader.h"
#include "ThreadEngine.h"
#include <utility>
#include <stdint.h>
#include <csignal>
#include <cstdlib>
#include <sys/stat.h>
#include <cmath>
