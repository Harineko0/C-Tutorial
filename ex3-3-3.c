#include <stdio.h>

// 計算
int main() {
    double a, b;

    printf("Please enter a number a\n");
    scanf("%lf", &a);
    printf("Please enter a number b\n");
    scanf("%lf", &b);

    printf("%e + %e = %e\n", a, b, a + b);
    printf("%e - %e = %e\n", a, b, a - b);
    printf("%e * %e = %e\n", a, b, a * b);
    printf("%e / %e = %e\n", a, b, a / b);
    // Cause ERROR !
    printf("%e % %e = %e\n", a, b, a % b);
}