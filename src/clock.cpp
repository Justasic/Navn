#include "clock.h"


Clock::Clock(){}

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
  time(&rawtime);
  ptm = gmtime(&rawtime);
  std::stringstream r;
  int h = ptm->tm_hour-6;
  CorrectHour(h);
  r << h << ":" << ptm->tm_min << ":" << ptm->tm_sec;
  return r.str();
}
Flux::string Clock::EST()
{
  time(&rawtime);
  ptm = gmtime(&rawtime);
  std::stringstream r;
  int h = (ptm->tm_hour)-4;
  CorrectHour(h);
  r << h << ":" << ptm->tm_min << ":" << ptm->tm_sec;
  return r.str();
}
Flux::string Clock::UTC()
{
  time(&rawtime);
  ptm = gmtime(&rawtime);
  std::stringstream r;
  int h = (ptm->tm_hour);
  CorrectHour(h);
  r << h << ":" << ptm->tm_min << ":" << ptm->tm_sec;
  return r.str();
}
Flux::string Clock::CCT()
{
  time(&rawtime);
  ptm = gmtime(&rawtime);
  std::stringstream r;
  int h = (ptm->tm_hour)+8;
  CorrectHour(h);
  r << h << ":" << ptm->tm_min << ":" << ptm->tm_sec;
  return r.str();
}
Flux::string Clock::PST()
{
  time(&rawtime);
  ptm = gmtime(&rawtime);
  std::stringstream r;
  int h = (ptm->tm_hour)-7;
  CorrectHour(h);
  r << h << ":" << ptm->tm_min << ":" << ptm->tm_sec;
  return r.str();
}
Flux::string Clock::AUS()
{
  time(&rawtime);
  ptm = gmtime(&rawtime);
  std::stringstream r;
  int h = (ptm->tm_hour)+10;
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