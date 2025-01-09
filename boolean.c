#include <stdio.h> // printf
#include <stdbool.h> // bool

/**
 * boolean values are returned as integers 1 or 0
 * 
 * Therefore, you must use the %d format specifier to print a boolean value
 */
int main() {
    printf("Boolean values are returned as integers 1 or 0\n");
    printf("----------------------------------------------\n");
    printf("myBool = false\n");
    bool myBool = false;
    printf("myBool: %d\n", myBool); // 1 or 0

    printf("\n");

    printf("myBool = true\n");
    myBool = true;
    printf("myBool: %d\n", myBool); // 1 or 0
    return 0;
}
