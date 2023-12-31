#include "stdio.h"
#include "math.h"
#define LENGTH 3



int main() {
    int id[LENGTH];
    int score[LENGTH];
    float ave = 0, sd = 0;
    int trueNum = LENGTH;

    // Input
    for (int i = 0; i < LENGTH; i++) {
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