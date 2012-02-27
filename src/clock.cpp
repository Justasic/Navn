/* Riista IRC bot -- Clock functions for weather module
 * 
 * (C) 2011-2012 Flux-Net
 * Contact us at Dev@Flux-Net.net
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
  isdst = ptm->tm_isdst?true:false;
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
  std::stringstream r;
  int h = (ptm->tm_hour+i);
  CorrectHour(h);
  r << h << ":" << ptm->tm_min << ":" << ptm->tm_sec;
  return r.str();
}
Flux::string Clock::MST()
{
  std::stringstream r;
  
  int h = ptm->tm_hour - isdst?6:7;
  CorrectHour(h);
  r << h << ":" << ptm->tm_min << ":" << ptm->tm_sec;
  return r.str();
}
Flux::string Clock::EST()
{
  std::stringstream r;
  int h = (ptm->tm_hour) - isdst?4:5;
  CorrectHour(h);
  r << h << ":" << ptm->tm_min << ":" << ptm->tm_sec;
  return r.str();
}
Flux::string Clock::UTC()
{
  std::stringstream r;
  int h = (ptm->tm_hour);
  CorrectHour(h);
  r << h << ":" << ptm->tm_min << ":" << ptm->tm_sec;
  return r.str();
}
Flux::string Clock::CCT()
{
  std::stringstream r;
  int h = (ptm->tm_hour)+8;
  CorrectHour(h);
  r << h << ":" << ptm->tm_min << ":" << ptm->tm_sec;
  return r.str();
}
Flux::string Clock::PST()
{
  std::stringstream r;
  int h = (ptm->tm_hour) - isdst?7:8;
  CorrectHour(h);
  r << h << ":" << ptm->tm_min << ":" << ptm->tm_sec;
  return r.str();
}
Flux::string Clock::AUS()
{
  std::stringstream r;
  int h = (ptm->tm_hour) + isdst?10:11;
  CorrectHour(h);
  r << h << ":" << ptm->tm_min << ":" << ptm->tm_sec;
  return r.str();
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