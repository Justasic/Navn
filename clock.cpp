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