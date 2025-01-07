#include <stdio.h> // printf
#include <stdlib.h> // malloc, free
#include <stdint.h> // uint32_t
#include <time.h> // time
#include <string.h> // strcpy

void declare();
void declare_initialize();
void multi_dimensional();
void dynamic_allocation();

int main() {

    // declare();
    // declare_initialize();
    // multi_dimensional();
    dynamic_allocation();


    printf("\n");
    return 0;
}

void declare() {
    // Declare an array of 10 integers
    int nums[10];

    // Declare an array of 4 characters
    char list[4];


    // Initialize the first and third values
    list[0] = 'a';
    list[2] = 'c';

    // Print the ints in nums
    for(int i = 0; i < 10; ++i) {
        nums[i] = 0;
    }
}

/**
 * The null terminator (\0) is added to the end automatically
 * but I like to manully add it
 */
void declare_initialize() {
    int arr[] = {1, 2, 3, 4, 5};
    char str[6] = {'H', 'e', 'l', 'l', 'o', '\0'}; // 5 characters + 1 null terminator
    char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ\0"; // 26 characters + 1 null terminator

    printf("\n");
    for(int i = 0; i < 5; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    printf("%s", str);
    printf("\n");
    for(int i = 0; i < strlen(alphabet); ++i) {
        printf("%c ", alphabet[i]);
    }
    printf("\n");
    printf("%s", alphabet);

}

/**
 * Col x Row
 */
void multi_dimensional() {
    // 2 Col x 3 Rows
    int matrix[2][3] = { 
        {1, 4, 2}, 
        {3, 6, 8} 
    };

    // Loop and print the values
    int cols, rows;
    for (cols = 0; cols < 2; cols++) {
        for (rows = 0; rows < 3; rows++) {
            printf("%d ", matrix[cols][rows]);
        }
        printf("\n");
    }
}

void dynamic_allocation() {
    // Start by allocating 6 * 1 byte
    char *string = (char*) malloc(6 * sizeof(char));

    // Initialize the 1st, 4th and 5th bytes
    string[0] = 'a';
    // string[1] = 'b';
    // string[2] = 'c';
    string[3] = 'd';
    string[4] = 'e';
    // string[5] = '\0';

    // Print the allocated registers
    printf("\n");
    for(int i = 0; i < 6; i++) {
        printf("%9p (%c) \n", (string + i), *(string + i));
    }
    printf("string: %s\n", string);

    // Copy over the string "xyz" into the first 4 bytes
    strcpy(string, "xyz\0"); // also usefull for copying strings into structs

    // Print the allocated registers
    printf("\n");
    for(int i = 0; i < 6; i++) {
        printf("%9p (%c) \n", (string + i), *(string + i));
    }
    printf("string: %s\n", string);


    free(string);
}
