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

/// 選択ソート
void select(int *score, int *id, int _length) {
    // Sort
    for (int i = 0; i < _length; i++) {
        int minI = minIndex(score, _length, i - 1);
        int tmpScore = score[i];
        int tmpId = id[i];
        score[i] = score[minI];
        id[i] = id[minI];
        score[minI] = tmpScore;
        id[minI] = tmpId;
    }
}

/// クイックソート
void quick(int *score, int *id, int _length) {
    if (_length <= 1) {
        return;
    }

    // 1 <= pivotIndex <= length
    int pivotIndex = 0;
    int pivotScore = score[pivotIndex];;
    int pivotId = id[pivotIndex];

    int leftLength = 0, rightLength = 0;
    int leftScore[_length], leftId[_length];
    int rightScore[_length], rightId[_length];

    // pivotScore より小さい値 -> 左側, 大きい値 -> 右側
    for (int i = 1; i < _length; i++) {
        int tmpScore = score[i];
        int tmpId = id[i];

        if (tmpScore <= pivotScore) {
            leftLength++;
            leftScore[leftLength - 1] = tmpScore;
            leftId[leftLength - 1] = tmpId;
        } else {
            rightLength++;
            rightScore[rightLength - 1] = tmpScore;
            rightId[rightLength - 1] = tmpId;
        }
    }

    quick(leftScore, leftId, leftLength);
    quick(rightScore, rightId, rightLength);

    // length 6
    // left 2
    // right 3
    for (int i = 0; i < _length; i++) {
        if (i < leftLength) {
            // 0 1
            score[i] = leftScore[i];
            id[i] = leftId[i];
        } else if (i == leftLength) {
            // 2
            score[i] = pivotScore;
            id[i] = pivotId;
        } else {
            // 3 4
            int index = i - leftLength - 1;
            score[i] = rightScore[index];
            id[i] = rightId[index];
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
    select(score, id, trueLength);
//    bubble(score, id, trueLength);
//    quick(score, id, trueLength);

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