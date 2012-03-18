/* Navn IRC bot -- Threading engine header
 * 
 * (C) 2011-2012 Flux-Net
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */
#ifndef THREAD_H
#define THREAD_H
#include "includes.h"
#include <pthread.h>

class CoreExport Thread : public Base
{
  pthread_t Handle;
  bool exit;
public:
  /* Anope: "Exit the thread. Note that the thread still must be joined to free resources!" */
  Thread();
  virtual ~Thread();
  void SetExitState();
  void OnNotify();
  void Notify();
  bool GetExitState() const;
  void Join();
  void Exit();
  void Start();
  virtual void ToRun() =0;
};
#endif