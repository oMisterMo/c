#include <stdio.h> // printf
#include <stdbool.h> // bool

/**
 * boolean values are returned as integers 1 or 0
 * 
 * Therefore, you must use the %d format specifier to print a boolean value
 */
int main() {
    bool myBool = false;

    printf("%d\n", myBool); // 1 or 0
    return 0;
}
