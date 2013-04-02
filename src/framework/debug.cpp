#include "debug.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <crt/snprintf.h>

#include "log.h"
#include "util/msgbox.h"

const int DEBUG_ASSERT_BUFFER_SIZE = 2048;

void DebugInit()
{
#if defined(_WIN32) && defined(_DEBUG) && defined(DEBUG_MEM_LEAK_CHECK)
	int tmpFlags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	tmpFlags |= _CRTDBG_CHECK_ALWAYS_DF;
	tmpFlags |= _CRTDBG_DELAY_FREE_MEM_DF;
	tmpFlags |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(tmpFlags);
#endif

#ifdef DEBUG
	LOG_DEBUG(LOGCAT_DEBUG, "DEBUG enabled for this build.\n");
#endif
}

void DebugClose()
{
}

int DebugAssert(const char *expression, const char *file, unsigned long line, const char *function, int *ignore)
{
	char buffer[DEBUG_ASSERT_BUFFER_SIZE];
	int ret = 0;

	snprintf(buffer, DEBUG_ASSERT_BUFFER_SIZE,
	    "ASSERT FAILED\n\n"
	    "EXPRESSION: %s\n"
	    "FILE: %s\n"
	    "LINE: %ld\n"
	    "FUNCTION: %s\n",
		expression, file, line, function);

	LOG_ERROR(LOGCAT_DEBUG, buffer);
	MSGBOX_RESULT result = MsgBox_AbortRetryIgnore(MSGBOX_TYPE_WARNING, buffer, "Assert Failed");
	switch (result)
	{
		case MSGBOX_RESULT_ABORT:
			ret = 1;
			break;
		case MSGBOX_RESULT_IGNORE:
			*ignore = 1;
			ret = 0;
			break;
		case MSGBOX_RESULT_RETRY:
			ret = 0;
			break;
		default:
			ret = 0;
	}

#ifdef DESKTOP
	fflush(NULL);
#endif
	return ret;
}
