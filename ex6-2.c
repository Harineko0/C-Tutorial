#include "stdio.h"
#define N 10

int main() {
    int seq[N];

    seq[0] = 0;
    seq[1] = 1;

    for (int i = 2; i < N; i++) {
        seq[i] = seq[i - 1] + seq[i - 2];
    }

    for (int i = 0; i < N; i++) {
        printf("%d, ", seq[i]);
    }
}