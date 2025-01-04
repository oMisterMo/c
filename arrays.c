#include <stdio.h> // printf
#include <stdlib.h> // malloc, free
#include <stdint.h> // uint32_t
#include <time.h> // time

int main() {
    int matrix[2][3] = { 
        {1, 4, 2}, 
        {3, 6, 8} 
    };

    int i, j;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    } 

    printf("\n");
    return 0;
}
