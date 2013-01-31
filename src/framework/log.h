#ifndef __FRAMEWORK_LOG_H_INCLUDED__
#define __FRAMEWORK_LOG_H_INCLUDED__

#include <stdio.h>

#include "logcategories.h"

#define LOG_INFO(c, ...) (LogWrite("INFO", c, __VA_ARGS__))
#define LOG_WARN(c, ...) (LogWrite("WARN", c, __VA_ARGS__))
#define LOG_ERROR(c, ...) (LogWrite("ERROR", c, __VA_ARGS__))
#ifdef DEBUG
	#define LOG_DEBUG(c, ...) (LogWrite("DEBUG", c, __VA_ARGS__))
#else
	#define LOG_DEBUG(c, ...) 
#endif

extern FILE *g_fpLog;
extern const char *g_logFilename;

/** 
 * Opens the log file for writing. If the open fails, any LogWrite calls will
 * only output logging information to STDOUT.
 */
void LogStart();

/**
 * Closes the log file.
 */
void LogEnd();

/** 
 * Writes a message to the debugging log and stdout.
 * @param tag short string to tag the log message with 
 * @param category short string to categorize the log message with
 * @param format the message to write (automatically prefixed 
 *               with timestamp)
 */
void LogWrite(const char *tag, const char *category, const char *format, ...);

#endif
