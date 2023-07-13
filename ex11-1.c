#include <stdio.h>

int i=4, j=4, k=4;

/// make global.j = 2, global.k = 2
void test(int i) {
    int l=2;
    printf("test1: %d %d %d %d\n", i, j, k, l);
    // will be "test1: 1 1 4 2"
    i=2; j=2; k=2;
    // global.j = 2, global.k = 2
    printf("test2: %d %d %d %d\n", i, j, k, l);
    // will be "test2: 2 2 2 2"
}

int main(){
    // k と l は別変数
    int, k=1, l=1;
    i=1;
    j=1;
    // main.k, main.l = 1.
    // global.i = 1, global.j = 1, global.k = 4

    printf("main1: %d %d %d %d\n", i, j, k, l);
    // will be "main1: 1 1 1 1"
    test(i);
    // global.i, global.j, main.k, main.l
    printf("main2: %d %d %d %d\n", i, j, k, l);
    // will be "main2: 1 2 1 1"
}
