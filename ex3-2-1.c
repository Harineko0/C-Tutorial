#include <stdio.h>

// 3-1-3: a = 2147483647 でバグ
// 3-1-4: a = 1, b = 2 でバグ
int main() {
    printf("size of int: %llu bytes", sizeof(int));
}