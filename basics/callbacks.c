#include <stdio.h>

// Simplify, create new type called 'callback'
typedef float (*Callback)(int);

/**
 * Define a calculate function that takes in
 * an integer and an operation.asm
 * 
 * @param num number to apply operation
 * @param Callback callback function, takes an integer and returns a float, 
 *
 * @return the result of the operation on the integer
 */
float calculate(int num, Callback cb) {
    return cb(num);
}

float square_callback(int i) {
    return i * i;
}

float cube_callback(int i) {
    return i * i * i;
}

/**
 * Function Pointers aka Callbacks
 */
int main(void) {

    // printf("Hello world!\n");
    float result = calculate(2, cube_callback);
    printf("result: %.2f\n", result);

    return 0;
}
