#include "stdio.h"
#include "math.h"
#define N 10

int main() {
    int id[N];
    int score[N];
    float ave = 0, sd = 0;

    // Input
    for (int i = 0; i < N; i++) {
        printf("id: ");
        scanf("%d", &id[i]);

        printf("score: ");
        scanf("%d", &score[i]);
    }

    // 平均
    for (int i = 0; i < N; i++) {
        ave += score[i];
    }

    ave /= N;

    // 標準偏差
    for (int i = 0; i < N; i++) {
        int diff = score[i] - ave;
        sd += diff * diff;
    }

    sd /= N;
    sd = sqrt(sd);

    printf("-----------------------------\n");

    for (int i = 0; i < N; i++) {
        printf("id: %d, score: %d\n", id[i], score[i]);
    }

    printf("average: %f\n", ave);\
    printf("standard deviation: %f\n", sd);
}