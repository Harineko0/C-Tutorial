#ifndef _HULL_H_
#define _HULL_H_
#include "stdbool.h"

#define ULL_ALL_MASK 0xFFFFFFFFFFFFFFFF

typedef unsigned long long ull;
/// @brief Hyper Unsigned Long Long. 64bit以上の正の整数を配列で表す.
typedef unsigned long long hull[];
/// @brief hull のポインタ
typedef unsigned long long (*hull_p)[];

void fill(int bit, hull result, int size);
void or(hull result, int size, hull h, ...);
bool hull_equals(const hull a, int sizeA, const hull b, int sizeB);

#endif // _HULL_H_