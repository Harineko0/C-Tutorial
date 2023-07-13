#include <stdio.h>

int a[10];
/// make b = a
void test(int b[], int max) {
    int i;
    for(i=0; i<max; i++) {
        b[i] = a[i];
    }
}

void print_array(int c[], int max) {
    int i=0;
    for(i=0; i<max; i++) {
        printf("%d ", c[i]);
    }
    printf("\n");
}

int main() {
    int i;
    int b[10];
    // global.a = [1, 1, ...], main.b = [2, 2, ...]
    for(i=0; i<10; i++){
        a[i]=1; b[i]=2;
    }
    print_array(b, 10);
    // will be "2 2 2 2 2 2 2 2 2 2 "
    test(b, 10);
    print_array(b, 10);
    // will be "1 1 1 1 1 1 1 1 1 1 "
}
