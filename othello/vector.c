#include "vector.h"
#include <stdio.h>

struct vector add_vector(struct vector a, struct vector b) {
    struct vector new;
    new.x = a.x + b.x;
    new.y = a.y + b.y;
    return new;
}

struct vector scala_vector(struct vector a, int n) {
    struct vector new;
    new.x = a.x * n;
    new.y = a.y * n;
    return new;
}

void printv(struct vector v) {
    printf("(%d, %d)", v.x, v.y);
}