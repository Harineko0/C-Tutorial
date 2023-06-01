#include <stdio.h>

// 16進数
int main() {
    int a, b;

    printf("Please enter a number a\n");
    scanf("%d", &a);
    printf("Please enter a number b\n");
    scanf("%d", &b);

    printf("%x + %x = %x\n", a, b, a + b);
    printf("%x - %x = %x\n", a, b, a - b);
    printf("%x * %x = %x\n", a, b, a * b);
    printf("%x / %x = %x\n", a, b, a / b);
}