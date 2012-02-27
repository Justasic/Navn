/* Riista IRC bot -- Clock class header
 * 
 * (C) 2011-2012 Flux-Net
 * Contact us at Dev@Flux-Net.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */


#ifndef CLOCK_H
#define CLOCK_H
#include "module.h"

class CoreExport Clock
{
private:
  time_t rawtime;
  struct tm *ptm;
  void CorrectHour(int &);
  bool isdst;
public:
  Clock();
  Flux::string Local();
  Flux::string CustomOffset(int);
  Flux::string MST();
  Flux::string EST();
  Flux::string UTC();
  Flux::string CCT();
  Flux::string PST();
  Flux::string AUS();
};

#endif