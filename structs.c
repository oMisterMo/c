#include <stdio.h>
#include <stdint.h>
#include <string.h>

void basic_structure();
void copy_structure();

/**
 * A struct is a user-defined data type in C/C++.
 * A struct is a collection of variables of different data types.
 * 
 * Syntax:
 * struct [structure tag] {
 *     member definition;
 *     member definition;
 *     ...
 * } [one or more structure variables];
 * 
 * If you want to change/modify a value, you can use the dot syntax (.).
 * And to modify a string value, the strcpy() function
 */
int main(int argc, char *argv[]) {


    basic_structure();
    // copy_structure();

    return 0;
}

void basic_structure() {
    struct Point {
        int x;
        int y;
        // int64_t z;
    };

    struct Point p1 = {1, 2};
    struct Point p2;
    p2.x = 3;
    p2.y = 4;

    printf("p1.x: %d\n", p1.x);
    printf("p1.y: %d\n", p1.y);

    printf("p2.x: %d\n", p2.x);
    printf("p2.y: %d\n", p2.y);

    printf("sizeof Point object: %ld\n", sizeof(p1));
}

void copy_structure() {
    struct Person {
        int myNum;
        char myLetter;
        char myString[30];
    };
    // Create a structure variable and assign values to it
    struct Person s1 = {13, 'B', "Some text"};

    // Create another structure variable
    struct Person s2;

    // Copy s1 values to s2
    s2 = s1;

    // Change s2 values
    s2.myNum = 30;
    s2.myLetter = 'C';
    strcpy(s2.myString, "Something else");

    // Print values
    printf("%d %c %s\n", s1.myNum, s1.myLetter, s1.myString);
    printf("%d %c %s\n", s2.myNum, s2.myLetter, s2.myString); 
}