#ifndef __FRAMEWORK_DEBUG_H_INCLUDED__
#define __FRAMEWORK_DEBUG_H_INCLUDED__

#if defined(_WIN32) && defined(_DEBUG) && defined(DEBUG_MEM_LEAK_CHECK)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DEBUG_CLIENTBLOCK new(_CLIENT_BLOCK, __FILE__, __LINE__)
#define new DEBUG_CLIENTBLOCK
#else
#define DEBUG_CLIENTBLOCK
#endif

#include <assert.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#endif

#ifdef DEBUG_STACK_TRACE
	/**
	 * Call stack tracing macro. Simply include this at the top of 
	 * any function to be tracked in the stack history. 
     */
	#ifdef __GNUC__
		#define STACK_TRACE                                    StackEntry __stackEntry(__FILE__, __LINE__, __PRETTY_FUNCTION__)
	#else
		#define STACK_TRACE                                    StackEntry __stackEntry(__FILE__, __LINE__, __FUNCSIG__)
	#endif
#else
	#define STACK_TRACE
#endif

#ifdef DEBUG
	/**
	 * Toggles a breakpoint in an attached debugger.
	 */
	#ifdef __GNUC__
		// Debug ARM builds using Marmalade SDK don't seem to like inline asm...
		#if (defined(__S3E__) && defined(__arm__) && defined(DEBUG))
			#define BREAKPOINT
		#elif (defined(__arm__) || defined(__thumb__))
			#define BREAKPOINT __asm__ ("bkpt 0")
		#else
			#define BREAKPOINT __asm__ ("int $3")
		#endif
	#elif _MSC_VER
			#define BREAKPOINT __asm int 0x3
	#else
			#error Unsupported compiler
	#endif
#else
	#define BREAKPOINT
#endif

#ifdef DEBUG
	#ifdef __GNUC__
		#define ASSERT(exp) \
		do { \
			static int ignore = 0; \
			if (!ignore && !(exp)) \
			{ \
				if (DebugAssert(#exp, __FILE__, __LINE__, __PRETTY_FUNCTION__, &ignore)) \
					BREAKPOINT; \
			} \
		} while(0)
	#elif _MSC_VER
		#define ASSERT(exp) \
		do { \
			static int ignore = 0; \
			if (!ignore && !(exp)) \
			{ \
				if (DebugAssert(#exp, __FILE__, __LINE__, __FUNCSIG__, &ignore)) \
					BREAKPOINT \
			} \
		} while(0)
	#else
		#error Unsupported compiler
	#endif
#else
	#define ASSERT(exp)	assert((exp))
#endif

#define _STATIC_ASSERT_TEST_(a, b) a ## b
#define _STATIC_ASSERT_TEST(a, b) _STATIC_ASSERT_TEST_(a, b)

/**
 * Compile-time assertions. Essentially a watered-down variant of
 * BOOST_STATIC_ASSERT and C++11's static_assert()
 */
#define STATIC_ASSERT(exp) typedef int _STATIC_ASSERT_TEST(static_assert_test_var_, __LINE__)[(exp) ? 1 : -1]

/** 
 * Maintains a list of call stack entries. Entries need to be
 * manually pushed/popped off this list.
 *
 * This class should not be used directly. Instead use the STACK_TRACE
 * macro and DebugStackTrace() function.
 */
class StackTrace
{
public:
	static StackTrace& Instance();

	StackTrace();
	~StackTrace();

	/** 
	 * Add new entry onto the top of call stack history.
	 * @param entry the entry to add
	 */
	void Push(char *entry);

	/** 
	 * Remove top entry off the call stack history.
	 */
	void Pop();

	/** 
	 * Remove all entries from the call stack history.
	 */
	void Clear();

	int GetSize()                                          { return m_entryStackPointer + 1; }

	/** 
	 * @return formatted string containing the call stack 
	 *         entries top-to-bottom
	 */
	char* GetTrace();

private:
	char *m_traceBuffer;
	char **m_entryStack;
	int m_entryStackPointer;
};

/** 
 * Used by the STACK_TRACE macro to push/pop entries onto the
 * actual stack.
 *
 * This class should not be used directly.
 */
class StackEntry
{
public:
	StackEntry(const char *file, unsigned long line, const char *function);
	~StackEntry();
};

/** 
 * Initializes the debug framework.
 */
void DebugInit();

/*
 * Closes up the debug framework.
 */
void DebugClose();

/** 
 * Logs assert information for failed assertions.
 * @param expression expression/condition that failed
 * @param file source file of expression
 * @param line source line in file of expression
 * @param function function name where assertion failed 
 * @param ignore returns whether the user selected to ignore 
 *               future asserts or not
 */
int DebugAssert(const char *expression, const char *file, unsigned long line, const char *function, int *ignore);

/** 
 * Gets the current call stack as recorded by the STACK_TRACE macros.
 * @return formatted string containing the call stack
 */
const char* DebugStackTrace();

#endif
