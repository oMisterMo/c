#include <stdio.h> // printf
#include <stdlib.h> // malloc, free
#include <stdint.h> // uint32_t
#include <time.h> // time
#include <string.h> // strcpy

int main() {
    int arr[] = {1, 2, 3, 4, 5};
    int nums[10];

    char list[4];
    char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; // 26 characters
    char str[6] = {'H', 'e', 'l', 'l', 'o', '\0'};
    char *string = (char*) malloc(6 * sizeof(char));

    int matrix[2][3] = { 
        {1, 4, 2}, 
        {3, 6, 8} 
    };

    string[0] = 'a';
    // string[1] = 'b';
    // string[2] = 'c';
    string[3] = 'd';
    string[4] = 'e';
    // string[5] = '\0';

    printf("\n\n");
    for(int i = 0; i < 6; i++) {
        printf("%9p (%c) \n", (string + i), *(string + i));
    }
    printf("%s\n", string);

    strcpy(string, "xyz\0"); // also usefull for copying strings into structs
    printf("\n");
    for(int i = 0; i < 6; i++) {
        printf("%9p (%c) \n", (string + i), *(string + i));
    }
    printf("%s\n", string);


    list[0] = 'a';
    list[2] = 'a';




    // for(int i = 1; i < 4; i++) {
    //     printf("%c ", *(argv + i));
    // }


    // int i, j;
    // for (i = 0; i < 2; i++) {
    //     for (j = 0; j < 3; j++) {
    //         printf("%d ", matrix[i][j]);
    //     }
    //     printf("\n");
    // }

    free(string);

    printf("\n");
    return 0;
}
