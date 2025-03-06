#include <stdio.h>
#include <stdint.h>

void array_pointers();
void simple_pointers();
void pointer_arithmetic();
void function_pointers(int*, int*);
void multi_dimensional_pointers();

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
    // array_pointers();

    // int a = 1, b = 2;
    // function_pointers(&a, &b);

    multi_dimensional_pointers();

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
 * The memory address of the first element is the same as the name of the array
 *
 * Pointer arithmetic: The pointer is incremented by the size of the data type
 * Deference the pointer to get the value using the * operator
 *
 */
void array_pointers() {
    int numbers[5] = {1, 2, 3, 4, 5};

    // Print the original array
    printf("\n");
    printf("numbers[5]: ");
    for(int i = 0; i < 5; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n\n");

    // Print the memory address of the array and the first element
    printf("The name of the array and first value point to the same address.\n");
    printf("-----------------------------\n");
    printf("%11s %14p\n%11s %14p\n", "numbers", numbers, "&numbers[0]", &numbers[0]);
    printf("-----------------------------\n");

    // Get the first number in the array
    printf("\n");
    printf("Get first number:\n");
    printf("-----------------------------\n");
    printf("*numbers: %d\n", *numbers); // 1
    printf("numbers[0]: %d\n", numbers[0]); // 1
    printf("*(&numbers[0]): %d\n", *(&numbers[0])); // 1 (silly)
    printf("*(&(*(&numbers[0]))): %d\n", *(&(*(&numbers[0])))); // 1 (sillier)

    // Get the second, third and fourth numbers in the array
    printf("\nGet other numbers:\n");
    printf("-----------------------------\n");
    printf("*(numbers + 1): %d\n", *(numbers + 1)); // 2
    printf("*(numbers + 2): %d\n", *(numbers + 2)); // 3
    printf("*(numbers + 3): %d\n", *(numbers + 3)); // 4

    // Modify the first and second numbers in the array
    printf("\nModify first and second number:\n");
    printf("-----------------------------\n");
    *numbers = 10;
    numbers[1] = 11; // same as *(&numbers[1])
    printf("*numbers = %d\n", numbers[0]); // 10
    printf("numbers[1] = %d\n", numbers[1]); // 11

    // Print the original array
    printf("\n");
    printf("numbers[5]: ");
    for(int i = 0; i < 5; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n\n");
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

void function_pointers(int* a, int* b) {
    printf("---function_pointers---\n");

    printf("before: (%d  %d)\n", *a, *b);
    (*a) *= 2;
    (*b) *= 2;
    printf("after: (%d  %d)\n", *a, *b);
}

void multi_dimensional_pointers() {
    // 2 Col x 3 Rows
    // 2 arrays, with 3 elements each
    // [[],[],[]], [[],[],[]]
    int matrix[2][3] = {
        {1, 4, 2},
        {3, 6, 8}
    };

    // arr[i][j] = *(arr[i] + j)
    // arr[i][j] = *(*(arr + i) + j)
    // Loop and print the values
    int cols, rows;
    for (cols = 0; cols < 2; cols++) {
        for (rows = 0; rows < 3; rows++) {
            printf("%d ", *(*(matrix + cols) + rows));
        }
        printf("\n");
    }

    printf("\n");

    // A pointer to an array of 3 elements
    int (*p)[3] = matrix;

    printf("%p\n", p);
    printf("%p\n", p + 1);
    printf("%p\n", p + 2);

    int *first_col = *p;
    int *second_col = *(p + 1);
    printf("\n");
    printf("[0][2]: %d\n", first_col[2]);
    printf("[1][2]: %d\n", second_col[2]);

    printf("\n");
    // printf("sizeof(matrix[0]): %ld\n", sizeof(matrix[0]));
    // printf("sizeof(p): %ld\n", sizeof(matrix[0]));
    // printf("%ld\n", (p + 1) - p);
    // printf("%ld\n", (p + 2) - (p + 1));

}
