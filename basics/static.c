#include <stdio.h>

 // ================== Globals here ==================
static float pi = 3.14;



// ================== Functions here ==================
static void DoSomething() {
    // The data here persists between function calls
    static int counter = 0;
    // The data here does not
    int anotherCounter = 0;

    counter++;
    anotherCounter++;

    printf("static counter: %d\n", counter);
    printf("int counter: %d\n", anotherCounter);
    printf("============================\n");
}


/**
 * Limits visibility to the current translation unit (file)
 *
 * - When you import a file, you can normally access all its members
 * - Similar to private
 *
 * Can be used on a global / local variable or function
 */
int main(void) {

    // ================== Locals here ==================
    static int i = 0;

    DoSomething();
    DoSomething();
    DoSomething();

    return 0;
}

// ================== Function Definitions here ==================
static void DoSomething();

