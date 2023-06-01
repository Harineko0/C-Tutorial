#include "stdio.h"

int test(char *testName, char *input, const char *expect) {
    if (input == expect) {
        printf("✓ %s: success", testName);
    } else {
        printf("x %s: failure\ninput: %s", testName, input);
    }
}