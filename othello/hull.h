#ifndef _HULL_H_
#define _HULL_H_
#include <stdbool.h>

#define ULL_ALL_MASK 0xFFFFFFFFFFFFFFFF

typedef unsigned long long ull;
/// @brief Hyper Unsigned Long Long. 64bit以上の正の整数を配列で表す.
typedef unsigned long long hull[];

void fill(int bit, hull result, int size);
void or(hull result, int size, ...);
void and(hull result, int size, ...);
void xor(hull result, int size, ...);
void not(hull result, int size, hull input);
void shiftR(hull h, int size, int amount);
void shiftL(hull h, int size, int amount);
int count_bit(hull h, int size);
bool hull_equals(const hull a, int sizeA, const hull b, int sizeB);
void printHULL(const hull h, int size);

#endif // _HULL_H_