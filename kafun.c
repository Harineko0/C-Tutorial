#include <stdio.h>
#define N 3

int main() {
    int id[N], score[N];

    for (int i = 0; i < N; i++) {
        printf("id %d: ", i);
        scanf("%d", &id[i]);
    }
}