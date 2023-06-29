#include "stdio.h"

int main(){
    char c;
    while(1){
        printf("input character:");
        scanf(" %c",&c);
        if(c == '.') break;
        for(int i = 0;i<26;i++){
            printf("%c",97+i);
        }
        printf("\n");
        for(int i = 0;i<c-97;i++){
            printf(" ");
        }
        printf("^\n");
    }
}