#include "stdio.h"
#include "math.h"
#include "string.h"
#define LENGTH 10

/*
1
5
2
3
3
7
4
1
5
1
6
8
7
10
8
5
9
10
10
3
 */

/// Bubble Sort
void bubble(int *score, int *id, int length) {
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < length - 1; j++) {
            if (score[j] > score[j + 1]) {
                int tmpScore = score[j];
                score[j] = score[j + 1];
                score[j + 1] = tmpScore;

                int tmpId = id[j];
                id[j] = id[j + 1];
                id[j + 1] = tmpId;
            }
        }
    }
}

int main() {
    int id[LENGTH];
    int score[LENGTH];
    float ave = 0, sd = 0;
    int trueLength = LENGTH;

    memset(id, 0, sizeof id);
    memset(score, 0, sizeof score);

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

    // ソート
    bubble(score, id, trueLength);

    // Output
    printf("-----------------------------\n");

    for (int i = 0; i < trueLength; i++) {
        printf("id: %3d,  score: %3d\n", id[i], score[i]);
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