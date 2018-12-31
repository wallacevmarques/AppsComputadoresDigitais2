/**********************************************************************
  util/gaul/gaul_config_win.h
 **********************************************************************

  gaul_config_win.h - GAUL configuration settings for windows.
  Copyright ©2003, Stewart Adcock <stewart@linux-domain.com>
  All rights reserved.

  The latest version of this program should be available at:
  http://gaul.sourceforge.net/

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.  Alternatively, if your project
  is incompatible with the GPL, I will probably agree to requests
  for permission to use the terms of any other license.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY WHATSOEVER.

  A full copy of the GNU General Public License should be in the file
  "COPYING" provided with this distribution; if not, see:
  http://www.gnu.org/

 **********************************************************************

  Synopsis:	GAUL configuration settings for windows.

  		All windows-specific configuration should be done here.

 **********************************************************************/

#ifndef GAUL_CONFIG_WIN_H_INCLUDED
#define GAUL_CONFIG_WIN_H_INCLUDED 1

#define USE_WINDOWS_H 0

#if defined __CYGWIN32__ && !defined __CYGWIN__
#  define __CYGWIN__ __CYGWIN32__
#endif

#include <sys/time.h>
#include <sys/types.h>
#include <limits.h>
#include <values.h>
#include <unistd.h>
#include <string.h>
#include <io.h>

#if USE_WINDOWS_H == 1
#include <windows.h>
#endif

#ifndef errno
/* Some sytems #define this! */
extern int errno;
#endif

#define HAVE_STDBOOL_H 1
#define HAVE__BOOL 1

/*
 * These need to be modified for each release:
 */
#define GA_MAJOR_VERSION 0
#define GA_MINOR_VERSION 1846
#define GA_PATCH_VERSION 0
#define GA_VERSION_STRING "0.1846-0"
#define GA_UNAME_STRING "<Unknown windows machine>"
#define GA_BUILD_DATE_STRING "26/08/03"

/*
 * Functions available on this platform:
 */

#define HAVE_MEMCPY 1
#define HAVE_MEMMOVE 1
#define HAVE_SINCOS 1
#define HAVE_STRDUP 1
#define HAVE_STRNDUP 1
#define HAVE_STRCPY 1
#define HAVE_STRNCPY 1
#define HAVE_STRCMP 1
#define HAVE_STRNCMP 1
#define HAVE_STRCASECMP 1
#define HAVE_STRNCASECMP 1
#define HAVE_STRICMP 1
#define HAVE_STRNICMP 1
#define HAVE_USLEEP 1
#define HAVE_INDEX 1
#define HAVE_RINDEX 1
#define HAVE_WAITPID 1
#define HAVE_FDOPEN 1
#define HAVE_GETPWUID 1
#define HAVE_SELECT 1
#define HAVE_ATEXIT 1
#define HAVE_ON_EXIT 1
#define HAVE_MEMSCAN 1
#define HAVE_MEMREV 1
#define HAVE_MEMCHR 1
#define HAVE_MEMMEM 1
#define HAVE_MEMCMP 1
#define HAVE_SETLOCALE 1
#define HAVE_STRCHR 1
#define HAVE_STRRCHR 1
#define HAVE_STRCAT 1
#define HAVE_STRCATV 1
#define HAVE_STRREV 1
#define HAVE_STRSPN 1
#define HAVE_STRCSPN 1
#define HAVE_STRERROR 1
#define HAVE_STRLCAT 1
#define HAVE_STRLCPY 1
#define HAVE_STRSTR 1
#define HAVE_STRPBRK 1
#define HAVE_STRSEP 1
#define HAVE_STRNFILL 1
#define HAVE_STRSIGNAL 1
#define HAVE_VSNPRINTF 1
#define HAVE_SNPRINTF 1
#define HAVE_ITOA 1
#define HAVE_BCOPY 1
#define HAVE_BCMP 1
#define HAVE_STRLEN 1
#define HAVE_STRNCPY 1
#define HAVE_STRTOD 1
#define HAVE_STRTOK 1
#define HAVE_MEMSET 1

/*
 * These are defined if windows.h is included:
 */
#if USE_WINDOWS_H == 1
#define HAVE_MIN 1
#define HAVE_MAX 1
#define HAVE_GETHOSTNAME 1
#else
#define HAVE_MIN 0
#define HAVE_MAX 0
#define HAVE_GETHOSTNAME 0
#endif

/*
 * Default constants:
 */

#define V3_QUICK 1
#define W32_CRIPPLED 1
#define HAVE_SLANG 0
#define AVLTREE_KEY_TYPE void *
#define GA_NUM_PROCESSES_ENVVAR_STRING "GA_NUM_PROCESSES"
#define GA_NUM_THREADS_ENVVAR_STRING "GA_NUM_THREADS"

#define MEMORY_ALLOC_SAFE 1
#define MEMORY_ALLOC_DEBUG 0

#endif /* GAUL_CONFIG_WIN_H_INCLUDED */

