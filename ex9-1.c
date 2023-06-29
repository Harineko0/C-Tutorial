// パスカルの三角形

#include "stdio.h"
#include "string.h"
#define N 10

int digits(int num) {
    int digit = 0;

    while (num > 0) {
        num /= 10;
        digit++;
    }

    return digit;
}

int strLength(int *array, int length) {
    int strLength = 0;

    for (int i = 0; i < length; i++) {
        strLength += digits(array[i]);

        // 半角スペースの分足す
        if (i != 0) {
            strLength++;
        }
    }

    return strLength;
}

int main() {
    // [[1], [1, 1], [1, 2, 1], [1, 3, 3, 1]]
    int array[N][N];

    // 全て0に初期化
    memset(array, 0, sizeof(array));

    // パスカル
    for (int i = 0; i < N; i++) {
        array[i][0] = 1;

        if (i > 0) {
            for (int j = 1; j < i; j++) {
                array[i][j] = array[i - 1][j] + array[i - 1][j - 1];
            }
        }

        array[i][i] = 1;
    }

    int maxLength = strLength(array[N - 1], N);

    for (int i = 0; i < N; i++) {
        int length = strLength(array[i], i + 1);
        int spaceLength = (maxLength - length) / 2;

        // スペース挿入
        for (int j = 0; j < spaceLength; j++) {
            printf(" ");
        }

        // 数字出力
        for (int j = 0; j < i + 1; j++) {
            int value = array[i][j];

            printf("%d ", value);
        }

        printf("\n");
    }
}