// These are very simple, basic, no-frills functions that
// EASTL expects us to create for it to operate.

// ---- Allocator --------------------------------------------------------------

#include <EASTL/allocator.h>
#include <stdlib.h>               // malloc

void* operator new[](size_t size, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
{
	return malloc(size);
}

void* operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
{
	// this allocator doesn't support alignment
	EASTL_ASSERT(alignment <= 8);
	return malloc(size);
}

// -----------------------------------------------------------------------------


// ---- For strings ------------------------------------------------------------

#include <EASTL/string.h>

int Vsnprintf8(char8_t* pDestination, size_t n, const char8_t* pFormat, va_list arguments)
{
#if defined(_MSC_VER) && !defined(__S3E__)
	return _vsnprintf(pDestination, n, pFormat, arguments);
#else
	return vsnprintf(pDestination, n, pFormat, arguments);
#endif
}

// -----------------------------------------------------------------------------
