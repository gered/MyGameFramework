#include "debug.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <crt/snprintf.h>

#include "log.h"
#include "util/msgbox.h"

const int32_t STACK_ENTRY_STACK_SIZE = 32;
const int32_t STACK_ENTRY_BUFFER_SIZE = 512;
const int32_t STACK_TRACE_BUFFER_SIZE = 2048;
const int32_t DEBUG_ASSERT_BUFFER_SIZE = 2048;

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

#ifdef DEBUG_STACK_TRACE
	snprintf(buffer, DEBUG_ASSERT_BUFFER_SIZE,
	    "ASSERT FAILED\n\n"
	    "EXPRESSION: %s\n"
	    "FILE: %s\n"
	    "LINE: %ld\n"
	    "FUNCTION: %s\n\n"
	    "STACK TRACE:\n%s\n", 
		expression, file, line, function, DebugStackTrace());
#else
	snprintf(buffer, DEBUG_ASSERT_BUFFER_SIZE,
	    "ASSERT FAILED\n\n"
	    "EXPRESSION: %s\n"
	    "FILE: %s\n"
	    "LINE: %ld\n"
	    "FUNCTION: %s\n",
		expression, file, line, function);
#endif

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

const char* DebugStackTrace()
{
#ifdef DEBUG_STACK_TRACE
    return StackTrace::Instance().GetTrace();
#endif
	return "";
}

StackTrace::StackTrace()
{
	m_traceBuffer = new char[STACK_TRACE_BUFFER_SIZE];
	m_entryStack = new char*[STACK_ENTRY_STACK_SIZE];
	for (int i = 0; i < STACK_ENTRY_STACK_SIZE; ++i)
		m_entryStack[i] = new char[STACK_ENTRY_BUFFER_SIZE];
	m_entryStackPointer = 0;
	Clear();
}

StackTrace::~StackTrace()
{
	delete[] m_traceBuffer;
	for (int i = 0; i < STACK_ENTRY_STACK_SIZE; ++i)
		delete[] m_entryStack[i];
	delete[] m_entryStack;
	Clear();
}

StackTrace& StackTrace::Instance()
{
	static StackTrace stackTrace;
	return stackTrace;
}

void StackTrace::Push(char *entry)
{
	assert(m_entryStackPointer < STACK_ENTRY_STACK_SIZE);
	snprintf(m_entryStack[m_entryStackPointer], STACK_ENTRY_BUFFER_SIZE, "%s", entry);
	++m_entryStackPointer;
}

void StackTrace::Pop()
{
	--m_entryStackPointer;
	m_entryStack[m_entryStackPointer][0] = '\0';
}

void StackTrace::Clear()
{
	m_entryStackPointer = 0;
}

char* StackTrace::GetTrace()
{
	*m_traceBuffer = '\0';
	for (int pointer = m_entryStackPointer; pointer >= 0; --pointer)
		snprintf(m_traceBuffer, STACK_TRACE_BUFFER_SIZE, "%s%s\n", m_traceBuffer, m_entryStack[pointer]);

	return m_traceBuffer;
}

StackEntry::StackEntry(const char *file, unsigned long line, const char *function)
{
	static char buffer[STACK_ENTRY_BUFFER_SIZE];
	snprintf(buffer, STACK_ENTRY_BUFFER_SIZE, "%s:%ld:%s", file, line, function);
	StackTrace::Instance().Push(buffer);
}

StackEntry::~StackEntry()
{
	StackTrace::Instance().Pop();
}
