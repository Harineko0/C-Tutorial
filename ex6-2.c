#include "stdio.h"
#define N 11

int main() {
    int seq[N], a, b;

//    seq[0] = 0;
//    seq[1] = 1;
    a = 0;
    b = 1;

    for (int i = 2; i < N; i++) {
//        seq[i] = seq[i - 1] + seq[i - 2];
        int c = a + b;
        printf("%d\n", c);
        a = b;
        b = c;
    }

//    for (int i = 0; i < N; i++) {
//        printf("f_%d: %d\n", i, seq[i]);
//    }
}