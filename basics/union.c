#include <stdio.h>
#include <assert.h>


int main(void) {


    // Exactyl like structs but only 1 member can have a value
    // The same memory space is used for all other members
    typedef union People {
        // char a;
        short b;
        int c;
        // long d;
    } person;

    printf("%ld", sizeof(person));
    printf("\n");

    // How do you use a uion type? I don know yet
    // person->b = 2;
    // person->c = 5;


    return 0;
}
