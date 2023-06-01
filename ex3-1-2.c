#include <stdio.h>

int main() {
    int a, b;

    a = 5;
    b = 3;

    printf("%d + %d = %d\n", a, b, a + b);
    printf("%d - %d = %d\n", a, b, a - b);
    printf("%d * %d = %d\n", a, b, a * b);
    printf("%d / %d = %f\n", a, b, (float )a / (float) b);
}