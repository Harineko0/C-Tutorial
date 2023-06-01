#include "stdio.h"

int main() {
    int numbers[] = {4, 6, 2, 7, 5, 1,12, -1};
    // -> -1, 1, 2, 4, 5, 6, 7, 12
    int length = sizeof(numbers) / sizeof(numbers[0]);
    int sorted = 0;

    for (int i = 0; i < length; i++) {
        int insertTo = length - 1;

        int num = numbers[sorted];

        for (int j = 0; j < sorted; j++) {

        }
    }
}