#ifndef __FRAMEWORK_COMMON_H_INCLUDED__
#define __FRAMEWORK_COMMON_H_INCLUDED__

#include <stdint.h>
#include <cstddef>

typedef unsigned long ulong;
typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x)	                                   if (x) { (x)->Release(); (x) = NULL; }
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(x)	                                   if (x) { delete (x); (x) = NULL; }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x)	                           if (x) { delete [] (x); (x) = NULL; }
#endif

/**
 * Returns the lower of two values.
 * @param a the first value
 * @param b the second value
 * @return the lower value
 */
template <class T>
inline T Min(T a, T b)
{
	return a < b ? a : b;
}

/**
 * Returns the higher of two values.
 * @param a the first value
 * @param b the second value
 * @return the higher value
 */
template <class T>
inline T Max(T a, T b)
{
	return a > b ? a : b;
}

/**
 * Determines if a bit is set in the given bitfield.
 * @param bit the bit to check
 * @param bitfield the bit field to check the bit in
 * @return true if set, false if not
 */
template <class T1, class T2>
inline bool IsBitSet(T1 bit, T2 bitfield)
{
	return (bitfield & bit) != 0;
}

/**
 * Sets a bit in a given bitfield.
 * @param bit the bit to set
 * @param bitfield the bit field to set the bit in
 */
template <class T1, class T2>
inline void SetBit(T1 bit, T2 &bitfield)
{
	bitfield |= bit;
}

/**
 * Clears a bit in a given bitfield.
 * @param bit the bit to clear
 * @param bitfield the bit field to clear the bit in
 */
template <class T1, class T2>
inline void ClearBit(T1 bit, T2 &bitfield)
{
	bitfield &= ~bit;
}

/**
 * Toggles a bit (inverts it's current state) in a given bitfield.
 * @param bit the bit to toggle
 * @param bitfield the bit field to toggle the bit in
 */
template <class T1, class T2>
inline void ToggleBit(T1 bit, T2 &bitfield)
{
	bitfield ^= bit;
}

/**
 * Seeds the system random number generator
 * @param seed the value to seed the random number generator with
 */
void SeedRnd(int seed);

/**
 * Returns a random integer.
 * @param low the low end of the range to generate a random integer within
 * @param high the high end of the range to generate a random integer within
 * @return the generated random integer
 */
int Rnd(int low, int high);

float Rnd(float low, float high);

#endif