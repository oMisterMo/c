#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

void allocate();
void memoryset();
void memorycopy();

int main() {

    // allocate();
    // memoryset();
    memorycopy();
    printf("\n");

    return 0;
}

void allocate() {
    printf("\n---Allocate---\n");

    // Allocate memory for 4 bytes, store the result in a variable (int pointer)
    int *ptr1 = malloc(4);

    // Create another varaible which points to the same memory location 
    // Cast to char* -> Tell compiler I know this is a character pointer
    // Set actual type to char (1 byte)
    // Rest of the bits get truncated
    // ptr2 = Window to of 1 byte to ptr1
    char *ptr2 = (char*) ptr1;

    // Set the initial value to some binary number
    // ptr1[0] = 1684234849;    // a b c d or 01100001 01100010 01000001 01000010 or 1684234849
    // ptr1[0] = 0b01100001011000100100000101000010; // a b A B or 01100001 01100010 01000001 01000010 or 845300034
    *ptr1 = 0b01100001011000100110001101100100; // a b c d
    //      _ _ _ _ _ _ _ _, _ _ _ _ _ _ _ _, _ _ _ _ _ _ _ _, _ _ _ _ _ _ _ _
    //             [3]              [2]             [1]             [0]

    // Print the value in different formats
    printf("%4s: %08b %08b %08b %08b\n", "bin", ptr2[3], ptr2[2], ptr2[1], ptr2[0]);
    printf("%4s: %8d %8d %8d %8d\n", "dec", ptr2[3], ptr2[2], ptr2[1], ptr2[0]);
    printf("%4s: %8x %8x %8x %8x\n", "hex", ptr2[3], ptr2[2], ptr2[1], ptr2[0]);
    printf("%4s: %8c %8c %8c %8c\n", "char", ptr2[3], ptr2[2], ptr2[1], ptr2[0]);



    printf("\n\n");
    printf("int  ptr1: %032b (4 bytes)\n", *ptr1);
    printf("char ptr2: ");
    for (int i = 3; i >= 0; --i) {
        printf("%08b ", ptr2[i]);
    }
    printf(" (1 byte + padding)\n");


    // You must always remember to free the allocated memory
    free(ptr1);
    // free(ptr2);  // ERROR: double free detected
    // Also remember to set the pointers to NULL so we don't accidentally use them
    ptr1 = NULL;
    ptr2 = NULL;
}

/**
 * Sets N bytes of S to C
 */
void memoryset() {
    printf("\n---Memory Set---\n");
    int arr[10];

    // memset(S,C,N);
    // memset(pointer, init character, bytes to set)
    memset(arr, 0, sizeof(arr) / 2);

    for (int i = 0; i < 10; ++i) {
        printf("%p (%d): %16d\n", arr + i, i, arr[i]);
    }
}

void memorycopy() {
    printf("\n---Memory Copy---\n");
    int arr[] = { 9, 8, 7 };
    int dst[5];
    // dst, src, bytes
    memcpy(dst, arr, sizeof(int) * 3);

    for (int i = 0; i < 5; ++i) {
        printf("dst[%d]: %d\n", i, dst[i]);
    }
}

void memorymove() {

    // memmove();
}
