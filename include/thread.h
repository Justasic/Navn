/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#ifndef THREAD_H
#define THREAD_H
#include <includes.h>

class Thread
{
public:
  Thread();
  virtual ~Thread();
  void Join();
  /* Anope: "Exit the thread. Note that the thread still must be joined to free resources!" */
  void Exit();
  void Start();
  virtual void ToRun() =0;
};
#endif