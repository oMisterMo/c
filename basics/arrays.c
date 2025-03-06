#include <stdio.h> // printf
#include <stdlib.h> // malloc, free
#include <stdint.h> // uint32_t
#include <time.h> // time
#include <string.h> // strcpy

const char INPUT_STR[] = "%c ";
const char INPUT_CHAR = '$';

void declare();
void declare_initialize();
void multi_dimensional();
void dynamic_allocation();
void histogram_of_lengths();
void array_overflow();

int main() {

    // declare();
    // declare_initialize();
    // multi_dimensional();
    // dynamic_allocation();
    // histogram_of_lengths();
    array_overflow();


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

    // Initialize all values to 0
    printf("nums: ");
    for(int i = 0; i < 10; ++i) {
        nums[i] = 0;
        printf("%d ", nums[i]);
    }
    printf("\n");
    printf("char[]: %s\n",  list);
}

/**
 * The null terminator (\0) is added to the end automatically
 * but I like to manully add it
 */
void declare_initialize() {
    // Array with 5 elements
    int small_num_arr[] = {1, 2, 3, 4, 5};

    // Initialized with 0
    int large_num_arr[128] = {0};

    // Single quote character array
    char str[6] = {'H', 'e', 'l', 'l', 'o', '\0'};  // 5 characters + 1 null terminator
    // char str[6] = {'H', 'e', 'l', 'l', 'o'};        // 5 characters + 1 null terminator

    // Double quotes
    char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ\0";   // 26 characters + 1 null terminator = 27 bytes = 26 length
    // char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";     // 26 characters + 1 null terminator = 27 bytes = 26 length


    printf("\n");
    for(int i = 0; i < 5; ++i) {
        printf("%d ", small_num_arr[i]);
    }
    printf("\n\n");
    for(int i = 0; i < 6; ++i) {
        printf("%d ", large_num_arr[i]);
    }
    printf("...\n\n");
    printf("%s", str);
    printf("\n\n");
    for(size_t i = 0; i < strlen(alphabet); ++i) {
        printf("%c ", alphabet[i]);
    }
    printf("\n\n");
    printf("%s", alphabet);

}

/**
 * Col x Row
 */
void multi_dimensional() {
    // 2 Col x 3 Rows
    // 2 arrays, with 3 elements each
    // [[],[],[]], [[],[],[]]
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
    const int NO_OF_CHAR = 6;
    char *string = (char*) malloc(NO_OF_CHAR * sizeof(char));

    // Initialize the 1st, 4th and 5th bytes
    string[0] = 'a';
    // string[1] = 'b';
    // string[2] = 'c';
    string[3] = 'd';
    string[4] = 'e';
    string[5] = '\0';   // Needed otherwise heap-buffer-overflow

    // Print the allocated registers
    printf("\n");
    for(int i = 0; i < NO_OF_CHAR; i++) {
        printf("%9p (%c) \n", (string + i), *(string + i));
    }
    printf("string: %s\n", string);

    // Copy over the string "xyz" into the first 4 bytes
    strcpy(string, "xyz\0"); // also usefull for copying strings into structs

    // Print the allocated registers
    printf("\n");
    for(int i = 0; i < NO_OF_CHAR; i++) {
        printf("%9p (%c) \n", (string + i), *(string + i));
    }
    printf("string: %s\n", string);


    free(string);
    string = NULL;
}

/**
 * Exercise 1-13. Write a program to print a histogram of the lengths of words in its input. It is easy to draw the histogram with the bars horizontal; a vertical orientation is more challenging.
 * Exercise 1-14. Write a program to print a histogram of the frequencies of different characters in its input.
 */
void histogram_of_lengths() {
    // Terminal help
    printf("A program to print a histogram of the lengths.\n\n");
    printf("Type 'Ctrl + D' to exit\n");
    printf("Type 'Ctrl + C' to terminate\n");
    printf("\n");
    printf(INPUT_STR, INPUT_CHAR);

    // Program
    int c, i, nwhite, nother;
    int ndigit[10];

    nwhite = nother = 0;
    for(i = 0; i < 10; i++) {
        ndigit[i] = 0;
    }

    while((c = getchar()) != EOF) {
        if(c >= '0' && c <= '9') {
            ++ndigit[c - '0'];
        } else if(c == ' ' || c == '\n' || c == '\t') {
            ++nwhite;
        } else {
            ++nother;
        }
    }

    // Result
    printf("--------------------------------\n");
    printf("digits =");
    for(i = 0; i < 10; i++) {
        printf(" %d", ndigit[i]);
    }
    printf(", white space = %d, other = %d\n", nwhite, nother);
    printf("--------------------------------\n");

    // Histogram
    for(i = 0; i < 10; i++) {
        printf("%6d: ", i);
        for(int j = 0; j < ndigit[i]; j++) {
            printf("%c ", 'x');
        }
        printf("(%d)", ndigit[i]);
        printf("\n");
    }
    printf("%6s: ", "blanks");
    for(int j = 0; j < nwhite; j++) {
        printf("%c ", 'x');
    }
    printf("(%d)", nwhite);
    printf("\n");
    printf("%6s: ", "others");
    for(int j = 0; j < nother; j++) {
        printf("%c ", 'x');
    }
    printf("(%d)", nother);

}

void array_overflow() {
    #define SIZE 8
    char firstname[SIZE];
    char lastname[SIZE];

    // Set first character of lastname so we can track the overflow position
    lastname[0] = 'M';

    // Print the first name + 2 extra bytes
    printf("firstname[%d]\n\n", SIZE);
    for (int i = 0; i < SIZE + 2; ++i) {
        if (i == SIZE ) printf("\n=============\n");
        printf("%p [%d] %c\n", &firstname[i], i, *(firstname + i));
    }
    printf("=============\n");

    // Print the lastname
    printf("\nlastname[%d]\n\n", SIZE);
    for (int i = 0; i < SIZE; ++i) {
        printf("%p [%d] %c\n", &lastname[i], i, *(lastname + i));
    }
    printf("%ld\n", (lastname - &firstname[SIZE - 1]));
}
