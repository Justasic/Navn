/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#include "thread.h"
static inline pthread_attr_t *GetAttr()
{
  static pthread_attr_t attr;
  if(pthread_attr_init(&attr))
    throw CoreException("Error calling pthread_attr_init for Threads");
  if(pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE))
    throw CoreException("Unable to make threads joinable");
  return &attr;
}
Thread::Thread():exit(false) {}
Thread::~Thread() {}
void Thread::Notify() {}
void *EntryPoint(void *parameter)
{
  Thread *thread = static_cast<Thread*>(parameter);
  thread->ToRun();
  thread->SetExitState();
  pthread_exit(0);
}
void Thread::SetExitState()
{
 this->Notify();
 exit = true;
}
bool Thread::GetExitState() const
{
 return exit; 
}
void Thread::OnNotify()
{
  this->Join();
}
void Thread::Start()
{
 if(pthread_create(&this->Handle, GetAttr(), EntryPoint, this))
 {
  throw CoreException("Could not Create Thread: "+Flux::stringify(strerror(errno)));
 }
}
void Thread::Join()
{
  this->SetExitState();
  pthread_join(Handle, NULL);
}
void Thread::Exit()
{
  this->SetExitState();
  pthread_exit(0); 
}