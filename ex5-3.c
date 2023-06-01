#include "stdio.h"
#include "float.h"

float abs(float n) {
    if (n >= 0) {
        return n;
    } else {
        return -n;
    }
}

int main() {
    float x;
    int m;

    printf("Please enter x: ");
    scanf("%f", &x);

    printf("Please enter m: ");
    scanf("%d", &m);

    float root, oldRoot;

    root = (float)x;

    do {
        oldRoot = root;

        float expo = 1;

        for (int i = 0; i < m; i++) {
            expo = expo * oldRoot;
        }

        root = oldRoot * (1 - ((1 - x * (1 / expo)) / (float)m));
        printf("%f, %f\n", root, oldRoot);

    } while (abs(oldRoot - root) > FLT_EPSILON);

    printf("result: %f", root);
}