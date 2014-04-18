/* Timer include stuff.
 *
 * Functions ported from Anope IRC Services
 *
 * (C) 2003-2011 Anope Team
 *
 * Based on the original code of Epona by Lara.
 * Based on the original code of Services by Andy Church.
 * All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html)
 */
#pragma once

#include "includes.h"
#include "clock.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

class CoreExport Timer : public Base
{
private:
	/** The time this was created
	 */
	time_t settime;

	/** The triggering time
	 */
	time_t trigger;

	/** Numer of seconds between triggers
	 */
	long secs;

	/** True if this is a repeating timer
	 */
	bool repeat;

public:
	/**
	 * @fn Timer(long time_from_now, time_t now = time(NULL), bool repeating = false)
	 * Default constructor, initializes the triggering time
	 * @param time_from_now The number of seconds from now to trigger the timer
	 * @param now The time now
	 * @param repeating Repeat this timer every time_from_now if this is true
	 */
	Timer(long time_from_now, time_t now = time(NULL), bool repeating = false);

	/**
	 * @fn virtual ~Timer()
	 * Default destructor, removes the timer from the list
	 */
	virtual ~Timer();

	/**
	 * @fn void SetTimer(time_t t)
	 * Set the trigger time to a new value
	 * @param t The new time
	 */
	void SetTimer(time_t t);

	/**
	 * @fn time_t GetTimer()
	 * Retrieve the triggering time
	 * @return The trigger time
	 */
	time_t GetTimer() const;

	/**
	 * @fn bool GetRepeat()
	 * Returns true if the timer is set to repeat
	 * @return Returns true if the timer is set to repeat
	 */
	bool GetRepeat() const;

	/**
	 * @fn void SetSecs(time_t t)
	 * Set the interval between ticks
	 * @param t The new interval
	 */
	void SetSecs(time_t t);

	/** Returns the interval between ticks
	 * @return The interval
	 */
	long GetSecs() const;

	/** Returns the time this timer was created
	 * @return The time this timer was created
	 */
	time_t GetSetTime() const;

	/** Called when the timer ticks
	 * This should be overridden with something useful
	 */
	virtual void Tick(time_t ctime) = 0;
};

/** This class manages sets of Timers, and triggers them at their defined times.
 * This will ensure timers are not missed, as well as removing timers that have
 * expired and allowing the addition of new ones.
 */
class CoreExport TimerManager : public Base
{
	/** A list of timers
	 */
	static std::vector<Timer *> Timers;
public:
	/** Add a timer to the list
	 * @param T A Timer derived class to add
	 */
	static void AddTimer(Timer *T);

	/** Deletes a timer
	 * @param T A Timer derived class to delete
	 */
	static void DelTimer(Timer *T);

	/** Tick all pending timers
	 * @param ctime The current time
	 */
	static void TickTimers(time_t ctime = time(NULL));

	/** Compares two timers
	 */
	static bool TimerComparison(Timer *one, Timer *two);
};

