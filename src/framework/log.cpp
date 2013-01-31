#include "log.h"

#include <stdarg.h>
#include <time.h>
#include <stdio.h>
#include <crt/snprintf.h>
#ifndef __S3E__
#include <sys/timeb.h>
#endif

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#endif

#ifdef __S3E__
#include "s3eDebug.h"
#endif

FILE *g_fpLog = NULL;
const char *g_logFilename = "run.log";

const size_t LOG_BUFFER_SIZE = 8096;
char _tempBuffer[LOG_BUFFER_SIZE];
char _buffer[LOG_BUFFER_SIZE];

void LogStart()
{
#ifdef DESKTOP
	g_fpLog = fopen(g_logFilename, "w");
	if (!g_fpLog)
	{
		snprintf(_buffer, LOG_BUFFER_SIZE, "Log file \"%s\" could not be opened. Further LogWrite() calls will only appear in STDOUT.\n", g_logFilename);
		printf("%s", _buffer);
#ifdef _MSC_VER
		OutputDebugStringA(_buffer);
#endif
		return;
	}
#endif
}

void LogEnd()
{
#ifdef DESKTOP
	if (g_fpLog)
		fclose(g_fpLog);
#endif
}

void LogWrite(const char *tag, const char *category, const char *format, ...)
{
	static char timestamp[15];

	int hour;
	int minute;
	int second;
	unsigned int milliseconds;

#ifdef __S3E__
	struct timeval tv;
	gettimeofday(&tv, NULL);
	struct tm *currentDate = localtime(&tv.tv_sec);

	hour = currentDate->tm_hour;
	minute = currentDate->tm_min;
	second = currentDate->tm_sec;
	milliseconds = (unsigned int)((tv.tv_usec / 1000.0) + 0.5);
#else
	timeb seconds;
	ftime(&seconds);
    struct tm *currentDate = localtime(&seconds.time);

	hour = currentDate->tm_hour;
	minute = currentDate->tm_min;
	second = currentDate->tm_sec;
	milliseconds = seconds.millitm;
#endif

	// Prepend the date/time
	snprintf(timestamp, 15, "[%02d:%02d:%02d,%03d]", hour, minute, second, milliseconds);

	// now the user-provided format string and arguments...
	va_list args;
	va_start(args, format);
	vsnprintf(_tempBuffer, LOG_BUFFER_SIZE, format, args);
	va_end(args);

	// write it all out
	// if there was a category supplied, then add that in too
	if (category != NULL)
		snprintf(_buffer, LOG_BUFFER_SIZE, "%s [%s] [%s] %s", timestamp, tag, category, _tempBuffer);
	else
		snprintf(_buffer, LOG_BUFFER_SIZE, "%s [%s] %s", timestamp, tag, _tempBuffer);

	if (g_fpLog)
		fprintf(g_fpLog, "%s", _buffer);
#ifdef DESKTOP
	printf("%s", _buffer);
#ifdef _MSC_VER
	OutputDebugStringA(_buffer);
#endif
#endif
#ifdef __S3E__
#ifdef DEBUG
	s3eDebugTraceLine(_buffer);
#else
	// ensures our own logging output still gets output in ARM release builds
	printf("%s", _buffer);
#endif
#endif
}
