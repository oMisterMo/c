#include <stdio.h>

int main() {

    int nums[] = {1,3,6,8,9};
    int mo[8] = {22};

    printf("size of int : %lu\n", sizeof(int));
    printf("size of int nums[] {1,3,6,8,9} : %lu\n", sizeof(nums));
    printf("size of int mo[8] : %lu\n", sizeof(mo));


    printf("**************\n");
    int len = sizeof(mo) / sizeof(mo[0]);
    for (int i = 0; i < len; i++) {
        printf("%d ", mo[i]);
    }

    printf("\n");

    return 0;
}
