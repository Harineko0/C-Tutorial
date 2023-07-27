#include <stdbool.h>
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

int str_chomp(const char text[], char words[MAX_WORDS][CHAR_LENGTH]) {
    int word_count = 0;
    int char_index = 0;
    bool before_empty = true;

    for (int i = 0; text[i] != '\0'; i++) {
        if ((text[i] >= 'a' && text[i] <= 'z') || (text[i] >= 'A' && text[i] <= 'Z')) {
            words[word_count][char_index] = text[i];
            char_index++;
            before_empty = false;

        } else if (!before_empty) {
            words[word_count][char_index] = '\0';
            word_count++;
            char_index = 0;
            before_empty = true;

        }
    }
}

int main() {
    int size = 10;
    char s[size];
    char words[MAX_WORDS][CHAR_LENGTH];

//    fgets(s, size, stdin);
//    printf("%s", s);
    str_chomp("Hello, apple grape orange.", words);
    printf("?");
}