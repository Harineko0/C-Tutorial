#include <stdio.h>
#include <stdlib.h>

void practice2() {
    printf("Please enter a number\n");

    int num;
    scanf("%d", &num);
    printf("The number is: %d\n", num);
}

void practice3() {
    printf("Please enter a string\n");

    char * s;
    s = (char *) malloc(sizeof(s) * 128);
    scanf("%s", s);
    printf("The string is: %s\n", s);
}

int main(void) {
    practice2();
    practice3();
    return 0;
}

