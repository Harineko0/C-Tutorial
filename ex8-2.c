#include "stdio.h"
#include "math.h"
#define LENGTH 3

/// Bubble Sort

int main() {
    int id[LENGTH];
    int score[LENGTH];
    float ave = 0, sd = 0;
    int trueLength = LENGTH;

    // Input
    for (int i = 0; i < LENGTH; i++) {
        printf("id: ");
        scanf("%d", &id[i]);

        if (id[i] == -1) {
            trueLength = i;
            break;
        }

        printf("score: ");
        scanf("%d", &score[i]);
    }

    // 平均
    for (int i = 0; i < trueLength; i++) {
        ave += score[i];
    }

    ave /= trueLength;

    // 標準偏差
    for (int i = 0; i < trueLength; i++) {
        int diff = (float)score[i] - ave;
        sd += diff * diff;
    }

    sd /= trueLength;
    sd = sqrt(sd);

    // region Bubble Sort
    for (int i = 0; i < trueLength; i++) {
        for (int j = i; j < trueLength; j++) {
            if (id[j] > id[j + 1]) {
                int tmpId = id[j];
                id[j] = id[j + 1];
                id[j + 1] = tmpId;

                int tmpScore = score[j];
                score[j] = score[j + 1];
                score[j + 1] = tmpScore;
            }
        }
    }
    // endregion

    // Output
    printf("-----------------------------\n");

    for (int i = 0; i < trueLength; i++) {
        printf("id: %d, score: %d\n", id[i], score[i]);
    }

    printf("average: %f\n", ave);\
    printf("standard deviation: %f\n", sd);

    // Search
    printf("-----------------------------\n");

    int searchId, searchResult, isFounded = 0;
    printf("search query: ");
    scanf("%d", &searchId);

    for (int i = 0; i < trueLength; i++) {
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