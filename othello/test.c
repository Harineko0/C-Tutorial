#include "hull.h"
#include <stdio.h>

void expectULL(char* testName, ull input, ull expect) {
    if (input == expect) {
        printf("o Test %s passed\n", testName);
    } else {
        printf("x Test %s failed (input: %llu)\n", testName, input);
    }
}

void expectHULL(char* testName, hull input, hull expect, int size) {
    if (hull_equals(input, size, expect, size)) {
        printf("o Test %s passed\n", testName);
    } else {
        printf("x Test %s failed\n", testName);
        printf("    ");
        printHULL(input, size);
    }
}