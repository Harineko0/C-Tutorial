#include "test.h"
#include "hull.h"
#include <string.h>

int test();

int main() {
    test();
}

int test() {
    int size = 3;

    ull hull_fill_0[size], hull_fill_0_exp[] = {0, 0, 0};
    fill(0, hull_fill_0, size);
    ull hull_fill_1[size], hull_fill_1_exp[] = {ULL_ALL_MASK, ULL_ALL_MASK, ULL_ALL_MASK};
    fill(1, hull_fill_1, size);
    expectHULL("fill_0", hull_fill_0, hull_fill_0_exp, size);
    expectHULL("fill_1", hull_fill_1, hull_fill_1_exp, size);

    ull hull_andor_0[size],
            hull_or_0_0[]    = {0b10010101, 0b01000001, 0b10010100},
            hull_or_0_1[]    = {0b01010001, 0b01000000, 0b01100110},
            hull_or_0_exp[]  = {0b11010101, 0b01000001, 0b11110110},
            hull_and_0_exp[] = {0b00010001, 0b01000000, 0b00000100};
    or(hull_andor_0, size, hull_or_0_0, hull_or_0_1, NULL);
    expectHULL("or_0", hull_andor_0, hull_or_0_exp, size);
    and(hull_andor_0, size, hull_or_0_0, hull_or_0_1, NULL);
    expectHULL("and_0", hull_andor_0, hull_and_0_exp, size);
}