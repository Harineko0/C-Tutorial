#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int scanInt() {
    char * buffer;
    buffer = (char *) malloc(sizeof(buffer) * 128);

    printf("Please enter a number\n");
    scanf("%s", buffer);
    int bufferLength = strlen(buffer);
    if (bufferLength > 10) {
        printf("Please enter under 10 digit number.");
        return -1;
    }

    return atoi(buffer);
}

// 10桁制限
int main() {
    int a = scanInt();
    if (a == -1) {
        return 0;
    }
    int b = scanInt();
    if (b == -1) {
        return 0;
    }

    printf("%d + %d = %d\n", a, b, a + b);
    printf("%d - %d = %d\n", a, b, a - b);
    printf("%d * %d = %d\n", a, b, a * b);
    printf("%d / %d = %f\n", a, b, (float )a / (float) b);
}