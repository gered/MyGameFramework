#include "../debug.h"

#include "hashing.h"

#include <ctype.h>
#include <string.h>

HashedValue HashString(const char *str)
{
	STACK_TRACE;
	const unsigned long base = 65521L;
	const unsigned long nmax = 5552;

#define D01(buf, i) { s1 += tolower(buf[i]); s2 += s1; }
#define D02(buf, i) D01(buf, i); D01(buf, i + 1);
#define D04(buf, i) D02(buf, i); D02(buf, i + 2);
#define D08(buf, i) D04(buf, i); D04(buf, i + 4);
#define D016(buf) D08(buf, 0); D08(buf, 8);

	if (str == NULL)
		return 0;

	unsigned long s1 = 0;
	unsigned long s2 = 0;

	for (size_t len = strlen(str); len > 0; )
	{
		unsigned long k = len < nmax ? len : nmax;
		len -= k;

		while (k >= 16)
		{
			D016(str);
			str += 16;
			k -= 16;
		}

		if (k != 0)
		{
			do
			{
				s1 += *str++;
				s2 += s1;
			}
			while (--k);
		}

		s1 %= base;
		s2 %= base;
	}

#undef D01
#undef D02
#undef D04
#undef D08
#undef D016

	return reinterpret_cast<HashedValue>((s2 << 16) | s1);
}
