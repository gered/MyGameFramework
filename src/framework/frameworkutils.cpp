#include "common.h"

#include <stdlib.h>

void SeedRnd(int32_t seed)
{
	srand(seed);
}

int32_t Rnd(int32_t low, int32_t high)
{
	return rand() % ((high - low) + 1) + low;
}

float Rnd(float low, float high)
{
	return low + (float)rand() / ((float)RAND_MAX / (high - low));
}
