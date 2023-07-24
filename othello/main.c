#include "test.h"
#include "hull.h"
#include <string.h>

int test();

int main() {
    test();
}

int test() {
    int size = 3;

    ull hull_fill_0[size],
        hull_fill_0_exp[] = {0, 0, 0},
        hull_fill_1[size],
        hull_fill_1_exp[] = {ULL_ALL_MASK, ULL_ALL_MASK, ULL_ALL_MASK};
    fill(0, hull_fill_0, size);
    fill(1, hull_fill_1, size);
    expectHULL("fill_0", hull_fill_0, hull_fill_0_exp, size);
    expectHULL("fill_1", hull_fill_1, hull_fill_1_exp, size);

    ull hull_and_0[size],
        hull_or_0[size],
        hull_xor_0[size],
        hull_or_0_0[]    = {0b10010101, 0b01000001, 0b10010100},
        hull_or_0_1[]    = {0b01010001, 0b01000000, 0b01100110},
        hull_or_0_exp[]  = {0b11010101, 0b01000001, 0b11110110},
        hull_and_0_0[]   = {0b10010101, 0b01000001, 0b10010100},
        hull_and_0_1[]   = {0b01010001, 0b01000000, 0b01100110},
        hull_and_0_exp[] = {0b00010001, 0b01000000, 0b00000100},
        hull_xor_0_0[]   = {0b10010101, 0b01000001, 0b10010100},
        hull_xor_0_1[]   = {0b01010001, 0b01000000, 0b01100110},
        hull_xor_0_exp[] = {0b11000100, 0b00000001, 0b11110010},
        hull_not_0[]     = {0b01010001, 0b01000000, 0b01100110},
        hull_not_0_exp[] = {0b1111111111111111111111111111111111111111111111111111111110101110, 0b1111111111111111111111111111111111111111111111111111111110111111, 0b1111111111111111111111111111111111111111111111111111111110011001};
    or(hull_or_0, size, hull_or_0_0, hull_or_0_1, NULL);
    and(hull_and_0, size, hull_and_0_0, hull_and_0_1, NULL);
    xor(hull_xor_0, size, hull_xor_0_0, hull_xor_0_1, NULL);
    not(hull_not_0, size, hull_not_0);
    expectHULL("or_0", hull_or_0, hull_or_0_exp, size);
    expectHULL("and_0", hull_and_0, hull_and_0_exp, size);
    expectHULL("xor_0", hull_xor_0, hull_xor_0_exp, size);
    expectHULL("not_0", hull_not_0, hull_not_0_exp, size);

    ull hull_shiftR_0[]     = {0b10010101, 0b01000001, 0b10010100},
        hull_shiftR_0_exp[] = {0b00001001, 0x5000000000000004, 0x1000000000000009},
        hull_shiftL_0[]     = {0b10010101, 0b0101000000000000000000000000000000000000000000000000000000000100, 0b1001010000000000000000000000000000000000000000000000000000000010},
        hull_shiftL_0_exp[] = {0b100101010101, 0b0000000000000000000000000000000000000000000000000000000001001001, 0b0100000000000000000000000000000000000000000000000000000000100000};
    shiftR(hull_shiftR_0, size, 4);
    shiftL(hull_shiftL_0, size, 4);
    expectHULL("shiftR_0", hull_shiftR_0, hull_shiftR_0_exp, size);
    expectHULL("shiftL_0", hull_shiftL_0, hull_shiftL_0_exp, size);

}