#include <stdio.h> // printf
#include <stdbool.h> // bool

/**
 * boolean values are returned as integers
 * 
 * Therefore, you must use the %d format specifier to print a boolean value
 */
int main() {
    bool myBool = false;

    printf("%d\n", myBool);
    return 0;
}
