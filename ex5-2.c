#include "stdio.h"

int main() {
    int num = 0, originalNum;

    do {
        printf("Please enter a integer greater than 2 less than 2147483647: ");
        scanf("%d", &num);
    } while (num < 2 || num > 2147483647);

    originalNum = num;

    printf("%d = ", num);

    // 出力で * を出力しないか
    int isNotPrintTimes = 1;

    // 2 ~ numの半分まで
    for (int i = 1; i <= originalNum; i = i + 1) {
        int tmpI = i;

        // 2, 3, 5 で割るために
        if (i == 1) {
            tmpI = 2;
        }

        // iで割り切れるとき
        while (num % tmpI == 0) {
            if (isNotPrintTimes) {
                isNotPrintTimes = 0;
            } else {
                printf(" * ");
            }

            printf("%d", tmpI);
            num = num / tmpI;
        }

        if (num == 1) {
            break;
        }
    }
}