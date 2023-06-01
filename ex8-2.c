#include "stdio.h"
#include "math.h"
#define N 3

/// Bubble Sort

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

    // region Bubble Sort
    for (int i = 1; i < trueNum; i++) {
        for (int j = 0; j < trueNum - i; j++) {
            if (id[i] > id[i + 1]) {
                int tmpId = id[i];
                id[i] = id[i + 1];
                id[i + 1] = tmpId;

                int tmpScore = score[i];
                score[i] = score[i + 1];
                score[i + 1] = tmpScore;
            }
        }
    }
    // endregion

    // Output
    printf("-----------------------------\n");

    for (int i = 0; i < trueNum; i++) {
        printf("id: %d, score: %d\n", id[i], score[i]);
    }

    printf("average: %f\n", ave);\
    printf("standard deviation: %f\n", sd);

    // Search
    printf("-----------------------------\n");

    int searchId, searchResult, isFounded = 0;
    printf("search query: ");
    scanf("%d", &searchId);

    for (int i = 0; i < trueNum; i++) {
        if (id[i] == searchId) {
            searchResult = score[i];
            isFounded = 1;
        }
    }

    if (isFounded) {
        printf("score of %d: %d", searchId, searchResult);
    } else {
        printf("student not found");
    }
}