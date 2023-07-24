#include "stdio.h"

#define CHAR_LENGTH 20
#define MAX_WORDS 20

void to_lower(char str[]) {
    int i = 0;

    while (str[i] != '\0') {
        char c = str[i];

        if (c >= 'A' && c <= 'Z') {
            str[i] = c + 32;
        }

        i++;
    }
}

/// 辞書順.
/// @return a > b: 1, a == b: 0, a < b: -1
int char_order(char a, char b) {

}

int str_cmp(const char a[], const char b[]) {
    int depth = 0;

    while (1) {

    }
}

int str_chomp(const char text[], char words[CHAR_LENGTH][MAX_WORDS]) {

}

int main() {
    int size = 5;
    char s[size];

    fgets(s, size, stdin);
    printf("%s", s);
}