/**
 * Local Variables          // stack
 * Free Memory              // heap
 *
 * Global Variables         // permanent
 * Program Instructions     // storage
 * Static Variables         // area
 *
 */

#include <stdio.h>

void add(int a, int b) {

}
void sub(int a, int b) {
    // This is stored with the globals
    static int test = 1;
    printf("%p: sub() int test\n", &test);

    // These are on the stack
    // printf("%p: a\n", &a);
    // printf("%p: a\n", &b);
}

void example() {
    // Global, to this function only, not visible outside
    static int count = 0; // Retains its value between calls
    count++;
    printf("%p %d\n", &count, count);
}


int main(void) {

    static int age = 25;
    printf("%p: main\n", main);
    printf("%p: add()\n", add);
    printf("%p: sub()\n", sub);
    printf("%p: static int age\n", &age);
    sub(2, 1);

    printf("\n");
    example();  // 1
    example();  // 2
    example();  // 3


    return 0;
}
