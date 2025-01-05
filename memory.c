#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void allocate();

int main() {

    allocate();

    return 0;
}

void allocate() {
    // allocate memory for 4 bytes
    int *ptr1 = malloc(4);
    char *ptr2 = (char*) ptr1;

    ptr1[0] = 1684234849;

    printf("%d is %3d %3d %3d %3d in dec\n", *ptr1, ptr2[0], ptr2[1], ptr2[2], ptr2[3]);
    printf("%d is %3c %3c %3c %3c in bin\n", *ptr1, ptr2[0], ptr2[1], ptr2[2], ptr2[3]);
    printf("%d is %3x %3x %3x %3x in hex\n", *ptr1, ptr2[0], ptr2[1], ptr2[2], ptr2[3]);

}

