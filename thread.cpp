/*#include "thread.h"
void *Thread::inthread(void*)
{
  this->Run();
}
void Thread::Start()
{
  pthread_create(&this->t1, NULL, &(this->inthread), NULL);
}
void Thread::Join()
{
  pthread_join(this->t1, NULL);
}*/