/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
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