#include <math.h>
#include <stdio.h>
#include <float.h>

float fAbs(float f) {
    if (f > 0) {
        return f;
    } else {
        return -f;
    }
}

void solution(float a, float b, float c) {
    // 判別式 D
    float discri = b * b - 4 * a * c;

    if (fAbs(discri) < FLT_EPSILON) {
        // 重解. FLT_EPSILONで誤差許容

        // 解
        float s = -b / (2 * a);

        printf("x = %f\n", s);

    } else if (discri > 0) {
        // 実数解二つ
        float sqrtD = sqrt(discri);

        // 解 solution
        float s1 = (-b + sqrtD) / 2 * a;
        float s2 = (-b - sqrtD) / 2 * a;

        printf("x = %f, %f\n", s1, s2);

    } else if (discri < 0 && fAbs(discri) > FLT_EPSILON) {
        // 解なし
        printf("No real solution.\n");
    }
}

int test(int testNum, float input, float expect) {
    if ((input - expect) < FLT_EPSILON) {
        printf("✓ Test %d: success", testNum);
    } else {
        printf("x Test %d: failure", testNum);
    }
}

int main() {
    float a, b, c;

    printf("Please enter a\n");
    scanf("%f", &a);

    printf("Please enter b\n");
    scanf("%f", &b);

    printf("Please enter c\n");
    scanf("%f", &c);

    solution(a, b, c);
}