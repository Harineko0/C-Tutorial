#include "stdio.h"

int main() {
    char alphabets[] = "abcdefghijklmnopqrstuvwxyz";
    char input;

    printf("input character: ");
    scanf(" %c", &input);

    printf("%s\n", alphabets);

    for (int i = 0; i < sizeof(alphabets) / sizeof(alphabets[0]); i++) {
        if (input == alphabets[i]) {
            printf("^");
        } else {
            printf(" ");
        }
    }
}