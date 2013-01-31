#ifndef PORTABLE_CRT_SNPRINTF_H_INCLUDED__
#define PORTABLE_CRT_SNPRINTF_H_INCLUDED__

#include <stdio.h>
#include <stdarg.h>

#if defined(_MSC_VER) && defined(_WIN32) && !defined(snprintf)

int c99_snprintf(char* str, size_t size, const char* format, ...);
int c99_vsnprintf(char* str, size_t size, const char* format, va_list ap);

// these functions shamelessly copied from:
// http://stackoverflow.com/questions/2915672/snprintf-and-visual-studio-2010
// they serve to mimic C99 standard snprintf functionality (like the return
// value) on MSVC (which does it's own non-standard thing)

#define snprintf c99_snprintf

inline int c99_snprintf(char* str, size_t size, const char* format, ...)
{
    int count;
    va_list ap;
	
    va_start(ap, format);
    count = c99_vsnprintf(str, size, format, ap);
    va_end(ap);
	
    return count;
}

inline int c99_vsnprintf(char* str, size_t size, const char* format, va_list ap)
{
    int count = -1;
	
    if (size != 0)
        count = _vsnprintf_s(str, size, _TRUNCATE, format, ap);
    if (count == -1)
        count = _vscprintf(format, ap);
	
    return count;
}

#endif

#endif
