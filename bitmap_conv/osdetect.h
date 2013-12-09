#ifndef _OSDETECT_H_
#define _OSDETECT_H_

#if defined(WIN32) || defined(_WIN32) || defined(_W64)
#define	__WINDOWS__
#include <windows.h>
#elif defined(__linux__) || (__linux)
#define __LINUX__
#endif

#endif