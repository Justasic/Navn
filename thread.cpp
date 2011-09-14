/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
/*#include "thread.h"
static inline pthread_attr_t *GetAttr()
{
  static pthread_attr_t attr;
  if(pthread_attr_init(&attr))
    throw CoreException("Error calling pthread_attr_init for Threads");
  if(pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE))
    throw CoreException("Unable to make threads joinable");
  return &attr;
}*/
