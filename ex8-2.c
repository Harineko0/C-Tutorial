#include "stdio.h"
#include "math.h"
#define LENGTH 10

/// Bubble Sort

void bubble(int *id, int *score, int length) {
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < length - 1; j++) {
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
}

/// ignoreBeforeIndex: どこのindexまで無視するか. -1で無視しない
int minIndex(const int *array, int length, int ignoreBeforeIndex) {
    int resultIndex = ignoreBeforeIndex + 1;
    int min = array[resultIndex];

    for (int i = resultIndex + 1; i < length; i++) {
        int value = array[i];
        if (value < min) {
            min = array[i];
            resultIndex = i;
        }
    }

    return resultIndex;
}

void select(int *_id, int *_score, int _length) {
    // Sort
    for (int i = 0; i < _length; i++) {
        int minI = minIndex(_id, _length, i - 1);
        int tmpId = _id[i];
        int tmpScore = _score[i];
        _id[i] = _id[minI];
        _score[i] = _score[minI];
        _id[minI] = tmpId;
        _score[minI] = tmpScore;
    }
}

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

    select(id, score, trueLength);

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