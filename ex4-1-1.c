#include <stdio.h>

const float PI = 3.14;

int main() {
    float radius;

    printf("plz enter a radius\n3");
    scanf("%f", &radius);

    float surface = PI * radius * radius;
    float circumference = 2 * PI * radius;

    printf("S: %f, C: %f", surface, circumference);
}