#include <stdio.h>

// 計算
int main() {
    double a, b;

    printf("Please enter a number a\n");
    scanf("%lf", &a);
    printf("Please enter a number b\n");
    scanf("%lf", &b);

    printf("%lf + %lf = %lf\n", a, b, a + b);
    printf("%lf - %lf = %lf\n", a, b, a - b);
    printf("%lf * %lf = %lf\n", a, b, a * b);
    printf("%lf / %lf = %f\n", a, b, (float )a / (float) b);
}