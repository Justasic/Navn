/* Navn IRC bot -- Clock functions for weather Module
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */

#include "clock.h"

Clock::Clock()
{
  time(&rawtime);
  ptm = gmtime(&rawtime);
  isdst = (ptm->tm_isdst);
}

void Clock::CorrectHour(int &h)
{
  if (h < 0)
    h = 24 + h;
  else if (h > 24)
    h -= 24;
}

Flux::string Clock::Local()
{
  time_t t;
  time(&t);
  return asctime(localtime(&t));
}

Flux::string Clock::CustomOffset(int i)
{
  int h = (ptm->tm_hour+i);
  CorrectHour(h);
  return printfify("%02d:%02d:%02d", h, ptm->tm_min, ptm->tm_sec);
}

Flux::string Clock::MST()
{
  int h = ptm->tm_hour - isdst?6:7;
  CorrectHour(h);
  return printfify("%02d:%02d:%02d", h, ptm->tm_min, ptm->tm_sec);
}

Flux::string Clock::EST()
{
  int h = (ptm->tm_hour) - isdst?9:10;
  CorrectHour(h);
  return printfify("%02d:%02d:%02d", h, ptm->tm_min, ptm->tm_sec);
}

Flux::string Clock::UTC()
{
  int h = (ptm->tm_hour);
  CorrectHour(h);
  return printfify("%02d:%02d:%02d", h, ptm->tm_min, ptm->tm_sec);
}

Flux::string Clock::CCT()
{
  int h = (ptm->tm_hour) + 15;
  CorrectHour(h);
  return printfify("%02d:%02d:%02d", h, ptm->tm_min, ptm->tm_sec);
}

Flux::string Clock::PST()
{
  int h = (ptm->tm_hour) - isdst?6:7;
  Log(LOG_TERMINAL) << "IS DST: " << std::boolalpha << isdst << " " << ptm->tm_isdst;
  CorrectHour(h);
  return printfify("%02d:%02d:%02d", h, ptm->tm_min, ptm->tm_sec);
}

Flux::string Clock::AUS()
{
  int h = (ptm->tm_hour) + isdst?11:12;
  CorrectHour(h);
  return printfify("%02d:%02d:%02d", h, ptm->tm_min, ptm->tm_sec);
}

/**
 * \fn Flux::string duration(const time_t &t)
 * Expresses in a string the period of time represented by a given amount
 * of seconds (with days/hours/minutes).
 * \param seconds time in seconds
 * \return buffer
 */
Flux::string duration(const time_t &t)
{
  /* We first calculate everything */
  time_t days = (t / 86400);
  time_t hours = (t / 3600) % 24;
  time_t minutes = (t / 60) % 60;
  time_t seconds = (t) % 60;

  if (!days && !hours && !minutes)
    return value_cast<Flux::string>(seconds) + " " + (seconds != 1 ? "seconds" : "second");
  else
  {
    bool need_comma = false;
    Flux::string buffer;
    if (days)
    {
      buffer = value_cast<Flux::string>(days) + " " + (days != 1 ? "days" : "day");
      need_comma = true;
    }
    if (hours)
    {
      buffer += need_comma ? ", " : "";
      buffer += value_cast<Flux::string>(hours) + " " + (hours != 1 ? "hours" : "hour");
      need_comma = true;
    }
    if (minutes)
    {
      buffer += need_comma ? ", " : "";
      buffer += value_cast<Flux::string>(minutes) + " " + (minutes != 1 ? "minutes" : "minute");
    }
    return buffer;
  }
}

Flux::string do_strftime(const time_t &t, bool short_output)
{
  tm tm = *localtime(&t);
  char buf[BUFSIZE];
  strftime(buf, sizeof(buf), "%b %d %H:%M:%S %Y %Z", &tm);
  if (short_output)
    return buf;
  if (t < time(NULL))
    return Flux::string(buf) + " " + printfify("(%s ago)", duration(time(NULL) - t).c_str());
  else
    return Flux::string(buf) + " " + printfify("(%s from now)", duration(t - time(NULL)).c_str());
}
