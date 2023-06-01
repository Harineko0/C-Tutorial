#include <stdio.h>

// 計算
int main() {
    int a, b;

    printf("Please enter a number a\n");
    scanf("%d", &a);
    printf("Please enter a number b\n");
    scanf("%d", &b);

    printf("%d + %d = %d\n", a, b, a + b);
    printf("%d - %d = %d\n", a, b, a - b);
    printf("%d * %d = %d\n", a, b, a * b);
    printf("%d / %d = %f\n", a, b, (float )a / (float) b);
}