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
    static int test = 1;
    printf("%p: sub() int test\n", &test);
}

int main(void) {

    static int age = 25;
    printf("%p: main\n", main);
    printf("%p: add()\n", add);
    printf("%p: sub()\n", sub);
    printf("%p: static int age\n", &age);
    sub(2, 1);


    return 0;
}
