#include <stdio.h>

// Simplify, create new type called 'Callback'
// Callback var_name = function(int)
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

int add(int a, int b) {
    return a + b;
}

/**
 * Function Pointers aka Callbacks
 */
int main(void) {

    float result = calculate(2, cube_callback);
    printf("result: %.2f\n", result);
    // --------------------------------------




    // --------------------------------------
    // More on function pointers

    // step 1 - create regular definition
    // int *ptr;                   // points to an int
    // float *func_ptr (int);      // points to a function

    // step 2 - wrap with parenthesis due to precedence
    int (*ptr);
    float (*func_ptr) (int);
    int (*function_ptr) (int, int);

    // step 3 - you can assign it a value
    int num = 3;
    ptr = &num;
    func_ptr = cube_callback;
    function_ptr = &add;    // add == &add

    printf("%.2f\n",func_ptr(3));
    printf("%d\n", (*function_ptr) (1, 2));

    printf("\n\n");
    printf("%p\n", add);
    printf("%p\n", &add);

    return 0;
}
