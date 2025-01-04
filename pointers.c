#include <stdio.h>

void array_pointers();
void simple_pointers();

/**
 * Pointers are variables that store the memory address of another variable.
 * 
 * The & operator is used to get the address of a variable.
 * The * operator is used to declare a pointer.
 * 
 * When * is not used in declaration, it act as a dereference operator.
 * 
 */
int main() {

    // simple_pointers();
    array_pointers();

    printf("\n");
    return 0;
}

void simple_pointers() {
    // Note that the type of the pointer has to match the type of the variable you're working with (int in our example).
    int num = 10;   // variable
    int* ptr = &num;    // pointer type of variable

    /*
        Good To Know: There are two ways to declare pointer variables in C:
            int* myNum;
            int *myNum;
    */


    printf("%d\n", num); // the value 10
    printf("%p\n", &num); // a pointer to the address of num


    // Reference: Output the memory address of myAge with the pointer (0x7ffe5367e044)
    printf("%p\n", ptr);

    // Dereference: Output the value of myAge with the pointer (43)
    printf("%d\n", *ptr);
}

/**
 * The memory address of the first element is the same as the name of the array:
 */
void array_pointers() {
    int numbers[5] = {1, 2, 3, 4, 5};

    // GET
    printf("numbers: %p\n", numbers);
    printf("numbers[0]: %p\n", &numbers[0]);

    // Get the first number in the array
    printf("\nGet first number:\n");
    printf("*numbers: %d\n", *numbers); // 1
    printf("*(&numbers[0]): %d\n", *(&numbers[0])); // 1


    // Get the second number in the array
    // Deference the pointer to get the value using the * operator
    printf("\nGet second number:\n");
    printf("*numbers + 1: %d\n", *(numbers + 1)); // 2
    printf("*(&numbers[1]): %d\n", *(&numbers[0] + 1)); // 2
    printf("*(&numbers[1]): %d\n", *(&numbers[1])); // 2

    // MODIFY
    *numbers = 10;
    *(&numbers[1]) = 11;

    printf("\nModify first and second number:\n");
    printf("numbers: %d\n", numbers[0]); // 10
    printf("numbers: %d\n", numbers[1]); // 11
}
