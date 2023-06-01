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
    printf("%e / %e = %e1\n", a, b, (float )a / (float) b);
}

/**
* Please enter a number a
1e-300
Please enter a number b
1e-300
1.000000e-300 + 1.000000e-300 = 2.000000e-300
1.000000e-300 - 1.000000e-300 = 0.000000e+00
1.000000e-300 * 1.000000e-300 = 0.000000e+00
1.000000e-300 / 1.000000e-300 = nan1
*/