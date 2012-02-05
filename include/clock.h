#ifndef CLOCK_H
#define CLOCK_H
#include "module.h"

class CoreExport Clock
{
private:
  time_t rawtime;
  struct tm * ptm;
  void CorrectHour(int &);
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