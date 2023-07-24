#include <stdio.h>

void error(char* message) {
    printf("\033[31mError: %s\033[37m\n", message);
}