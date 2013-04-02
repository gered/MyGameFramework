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
 * Initializes the debug framework.
 */
void DebugInit();

/**
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

#endif
