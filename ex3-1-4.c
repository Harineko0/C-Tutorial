#include <stdio.h>

// unsigned
int main() {
    unsigned int a, b;

    printf("Please enter a number a\n");
    scanf("%u", &a);
    printf("Please enter a number b\n");
    scanf("%u", &b);

    printf("%u + %u = %u\n", a, b, a + b);
    printf("%u - %u = %u\n", a, b, a - b);
    printf("%u * %u = %u\n", a, b, a * b);
    printf("%u / %u = %f\n", a, b, (float )a / (float) b);
}