#include <stdio.h>

// Declaration
int add(int, int);
int sub(int a, int b);  // Well-chosen names are good documentation however, so we will often use them
int square(int*);

// int can be omitted since its the default return type
int power(int base, int n);

int main() {
    printf("power: %d", power(2, 4));
    printf("\n");
    return 0;
}

// Implementation
int add(int a, int b) {
    return a + b;
}

int sub(int a, int b) {
    return a - b;
}

int square(int *n) {
    return *n * *n;
}

/**
 * Pre ANSI C
 * Return type defaults to int. (compiler gives warning)
 *
 * DO NOT USE!!!
 */
power(base, n)
int base, n;
{
    int i, p;
    p = 1;
    for(i = 1; i < n; ++i) {
        p = p * base;
    }
    return p;
}
