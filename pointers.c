#include <stdio.h>
#include <stdint.h>

void array_pointers();
void simple_pointers();
void pointer_arithmetic();

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

    simple_pointers();
    // array_pointers();

    printf("\n");
    return 0;
}

void simple_pointers() {
    // Note that the type of the pointer has to match the type of the variable you're working with (int in our example).
    int num = 10; // variable
    int* ptr = &num; // the address of the variable num (aka pointer)

    /*
        Good To Know: There are two ways to declare pointer variables in C:
            int* myNum;
            int *myNum;
    */


    printf("%d\n", num); // the value 10
    printf("%p\n", &num); // a pointer to the physical address in memory of num


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

void pointer_arithmetic() {
    int64_t nums[10] = { 7, 8, 9, 2};
    int64_t *ptr = nums;   // &nums[0]

    // unsigned long long mo = 3039334; // 8 bytes
    printf("\n\n %ld \n\n", sizeof(int64_t));   // 8 bytes

    // printf("nums: %p\n", nums); // &nums[0] | ptr
    printf("nums: %p\n", ptr);
    printf("nums: %p\n", (ptr + 1));
    printf("nums: %p\n", (ptr + 2));
    printf("nums: %p\n", (ptr + 3));

    printf("\n");
    printf("nums: %p\n", &nums[0]);
    printf("nums: %p\n", &nums[1]);
    printf("nums: %p\n", &nums[2]);
    printf("nums: %p\n", &nums[3]);

    printf("\n");
    printf("nums: %ld\n", *(ptr));
    printf("nums: %ld\n", *(ptr + 1));
    printf("nums: %ld\n", *(ptr + 2));
    printf("nums: %ld\n", *(ptr + 3));
}