#include <stdio.h>

const float PI = 3.14;

int main() {
    float radius, increment;
    int times;

    printf("plz enter a radius\n");
    scanf("%f", &radius);

    printf("plz enter a increment\n");
    scanf("%f", &increment);

    printf("plz enter a times\n");
    scanf("%d", &times);

    for (int i = 0; i < times; i++) {
        float newRadius = radius + increment * i;
        float surface = PI * newRadius * newRadius;
        float circumference = 2 * PI * newRadius;

        printf("S: %f, C: %f\n", surface, circumference);
    }
}