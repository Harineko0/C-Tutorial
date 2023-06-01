#include "stdio.h"

int main() {
    int num, originalNum;

    printf("Please enter a number: ");
    scanf("%d", &num);

    originalNum = num;

    printf("%d = ", num);

    int isFirst = 1;

    // 2 ~ numの半分まで
    for (int i = 2; i <= originalNum; i++) {
        // iで割り切れるとき
        while (num % i == 0 || num == i) {
            if (isFirst) {
                isFirst = 0;
            } else {
                printf(" * ");
            }

            printf("%d", i);
            num = num / i;
        }

        if (num == 1) {
            break;
        }
    }
}