#include "stdio.h"
#include "math.h"
#define N 100

int main() {
    int id[N];
    int score[N];
    float ave = 0, sd = 0;
    int trueNum = N;

    // Input
    for (int i = 0; i < N; i++) {
        printf("id: ");
        scanf("%d", &id[i]);

        if (id[i] == -1) {
            trueNum = i;
            break;
        }

        printf("score: ");
        scanf("%d", &score[i]);
    }

    // 平均
    for (int i = 0; i < trueNum; i++) {
        ave += score[i];
    }

    ave /= trueNum;

    // 標準偏差
    for (int i = 0; i < trueNum; i++) {
        int diff = (float)score[i] - ave;
        sd += diff * diff;
    }

    sd /= trueNum;
    sd = sqrt(sd);

    printf("-----------------------------\n");

    for (int i = 0; i < trueNum; i++) {
        printf("id: %d, score: %d\n", id[i], score[i]);
    }

    printf("average: %f\n", ave);\
    printf("standard deviation: %f\n", sd);
}