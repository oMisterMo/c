#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

void allocate();

int main() {

    printf("\n");
    allocate();
    printf("\n");

    return 0;
}

void allocate() {
    assert(sizeof(int) == 4);

    // Allocate memory for 4 bytes (int)
    int *ptr1 = malloc(4);

    // Create another pointer to the same memory location (char)
    char *ptr2 = (char*) ptr1;

    // Set the initial value to some binary number
    // ptr1[0] = 1684234849;    // a b c d or 01100001 01100010 01000001 01000010 or 1684234849
    ptr1[0] = 0b01100101011000100100000101000010; // a b A B or 01100101 01100010 01000001 01000010 or 845300034

    // Print the value in different formats
    printf("%u is %7x %7x %7x %7x in hex\n", *ptr1, ptr2[0], ptr2[1], ptr2[2], ptr2[3]);
    printf("%u is %7d %7d %7d %7d in dec <=\n", *ptr1, ptr2[0], ptr2[1], ptr2[2], ptr2[3]);
    printf("%u is %7c %7c %7c %7c in char\n", *ptr1, ptr2[0], ptr2[1], ptr2[2], ptr2[3]);
    printf("%u is %7b %7b %7b %7b in bin\n", *ptr1, ptr2[0], ptr2[1], ptr2[2], ptr2[3]);

    // You must always remember to free the allocated memory
    free(ptr1);
    // free(ptr2);  // ERROR: double free detected in tcache 2

    // Also remember to set the pointers to NULL so we don't accidentally use them
    ptr1 = NULL;
    ptr2 = NULL;
}

