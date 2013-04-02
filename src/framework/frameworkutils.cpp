#include "common.h"

#include <stdlib.h>

void SeedRnd(int seed)
{
	srand(seed);
}

int Rnd(int low, int high)
{
	return rand() % ((high - low) + 1) + low;
}

float Rnd(float low, float high)
{
	return low + (float)rand() / ((float)RAND_MAX / (high - low));
}
