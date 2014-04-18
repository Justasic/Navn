#pragma once

#cmakedefine HAVE_SYS_TYPES_H 1
#cmakedefine HAVE_STDINT_H 1
#cmakedefine HAVE_FCNTL_H 1
#cmakedefine HAVE_STDDEF_H 1
#cmakedefine HAVE_BACKTRACE 1
#cmakedefine HAVE_SETJMP_H 1
#cmakedefine HAVE_SYS_EPOLL_H 1
#cmakedefine HAVE_GETTIMEOFDAY 1
#cmakedefine HAVE_SETGRENT 1
#cmakedefine HAVE_STRCASECMP 1
#cmakedefine HAVE_STRICMP 1
#cmakedefine HAVE_STRINGS_H 1
#cmakedefine HAVE_STRLCAT 1
#cmakedefine HAVE_STRLCPY 1
#cmakedefine HAVE_SYS_SELECT_H 1
#cmakedefine HAVE_UMASK 1
#cmakedefine HAVE_EVENTFD 1
#define LIBINTL "@LIBINTL_INCLUDE@"

#define VERSION_MAJOR @navn_MAJOR_VERSION@
#define VERSION_MINOR @navn_MINOR_VERSION@
#define VERSION_PATCH @navn_PATCH_LEVEL@
#define VERSION "@VERSION_SIMPLE@"
#define VERSION_GIT "@VERSION_GIT@"
#define VERSION_EXTRA "@navn_VERSION_EXTRA@"
#define VERSION_FULL "@VERSION_FULL@"

#cmakedefine HAVE_UINT8_T 1
#cmakedefine HAVE_U_INT8_T 1
#cmakedefine HAVE_INT16_T 1
#cmakedefine HAVE_UINT16_T 1
#cmakedefine HAVE_U_INT16_T 1
#cmakedefine HAVE_INT32_T 1
#cmakedefine HAVE_UINT32_T 1
#cmakedefine HAVE_U_INT32_T 1

#ifdef HAVE_SYS_EPOLL_H
# include <sys/epoll.h>
#endif
#ifdef HAVE_SETJMP_H
# include <setjmp.h>
#endif
#ifdef HAVE_BACKTRACE
#include <execinfo.h>
#endif
#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif
#ifdef HAVE_STDINT_H
# include <stdint.h>
#endif
#ifdef HAVE_STDDEF_H
# include <stddef.h>
#endif

#ifdef HAVE_UINT8_T
typedef uint8_t uint8;
#else
# ifdef HAVE_U_INT8_T
typedef u_int8_t uint8;
# else
#  ifdef _WIN32
typedef unsigned __int8 uint8;
#  else
typedef unsigned short uint8;
#  endif
# endif
#endif

#ifdef HAVE_INT16_T
typedef int16_t int16;
#else
# ifdef _WIN32
typedef signed __int16 int16;
# else
typedef int int16;
# endif
#endif

#ifdef HAVE_UINT16_T
typedef uint16_t uint16;
#else
# ifdef HAVE_U_INT16_T
typedef u_int16_t uint16;
# else
#  ifdef _WIN32
typedef unsigned __int16 uint16;
#  else
typedef unsigned int uint16;
#  endif
# endif
#endif

#ifdef HAVE_INT32_T
typedef int32_t int32;
#else
# ifdef _WIN32
typedef signed __int32 int32;
# else
typedef long int32;
# endif
#endif

#ifdef HAVE_UINT32_T
typedef uint32_t uint32;
#else
# ifdef HAVE_U_INT32_T
typedef u_int32_t uint32;
# else
#  ifdef _WIN32
typedef unsigned __int32 uint32;
#  else
typedef unsigned long uint32;
#  endif
# endif
#endif

/*
# ifdef _WIN32
# ifdef MSVCPP
#  define snprintf _snprintf
# endif
# define popen _popen
# define pclose _pclose
# define ftruncate _chsize
# ifdef MSVCPP
#  define PATH_MAX MAX_PATH
# endif
# define MAXPATHLEN MAX_PATH
# define bzero(buf, size) memset(buf, 0, size)
# ifdef MSVCPP
#  define strcasecmp stricmp
# endif
# define sleep(x) Sleep(x * 1000)
# endif
*/
