#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

void types();
void print_types();
void unary_operators();

/**
 * Type coversion can be implicit or explicit
 * 
 * Implicit = Automatic
 * Explicit = Manual
 */
int main(void) {

    // types();
    print_types();
    
    printf("\n");
    return 0;
}

/**
 * https://en.wikipedia.org/wiki/C_data_types#Main_types
 */
void types() {
    // standard
    // bool boolean; // 1 byte
    // char a; // 1 byte
    // short b; // 2 bytes
    // char *c[10]; // 10 bytes string
    // int d; // 4 bytes
    // float e; // 4 bytes
    // double f; // 8 bytes

    // c99 additions (more ints from stdint.h)
    // int8_t a; // 1 byte (-128 - 127)
    // int16_t b; // 2 bytes
    // int32_t c; // 4 bytes
    // int64_t d; // 8 bytes
    // uint8_t e; // 1 byte (0 - 255)
    // uint16_t f; // 2 bytes
    // uint32_t g; // 4 bytes
    // uint64_t h; // 8 bytes


    // may change size depending on major 32-bit systems or 64-bit systems (use stdint.h)
    // char a; // 1 byte
    // short b; // 2 bytes
    // int c; // 4 bytes
    // long d; // 4 bytes or 8 bytes
    // long long e; // 8 bytes
    // unsigned char f; // 1 byte
    // unsigned short g; // 2 bytes
    // unsigned int h; // 4 bytes
    // unsigned long i; // 4 bytes or 8 bytes
    // unsigned long long j; // 8 bytes
}

/**
 * Use sizeof() to print the type and bytes
 */
void print_types() {
    // const char *BUILD_IN_TYPES[] = {
    //     "char",
    //     "int",
    //     "float",
    //     "double",
    // };

    // const char *STD_INT_TYPES[] = {
    //     "int8_t",
    //     "int16_t",
    //     "int32_t",
    //     "int64_t",
    //     "uint8_t",
    //     "uint16_t",
    //     "uint32_t",
    //     "uint64_t",
    // };
    // const int len = sizeof(BUILD_IN_TYPES) / sizeof(BUILD_IN_TYPES[0]);
    // const int std_len = sizeof(STD_INT_TYPES) / sizeof(STD_INT_TYPES[0]);

    // printf("%8s size\n", "Type");
    // for(int i = 0; i < len; i++) {
    //     printf("%8s %ld\n", BUILD_IN_TYPES[i], sizeof(BUILD_IN_TYPES[i]));
    // }
    // printf("\n");
    // for(int i = 0; i < std_len; i++) {
    //     printf("%8s %ld\n", STD_INT_TYPES[i], sizeof(STD_INT_TYPES[i]));
    // }


    printf("%8s %5s\n", "Type", "Size (Bytes)");
    printf("-------------------------------\n");
    printf("%8s %5ld\n", "bool", sizeof(bool));
    printf("%8s %5ld\n", "char", sizeof(char));
    printf("%8s %5ld\n", "short", sizeof(short));
    printf("%8s %5ld\n", "int", sizeof(int));
    printf("%8s %5ld\n", "float", sizeof(float));
    printf("%8s %5ld\n", "double", sizeof(double));

    printf("\n");

    printf("%8s %5ld\n", "int8_t", sizeof(int8_t));
    printf("%8s %5ld\n", "int16_t", sizeof(int16_t));
    printf("%8s %5ld\n", "int32_t", sizeof(int32_t));
    printf("%8s %5ld\n", "int64_t", sizeof(int64_t));
    printf("%8s %5lu\n", "uint8_t", sizeof(uint8_t));
    printf("%8s %5lu\n", "uint16_t", sizeof(uint16_t));
    printf("%8s %5lu\n", "uint32_t", sizeof(uint32_t));
    printf("%8s %5lu\n", "uint64_t", sizeof(uint64_t));
}

/**
 * Unary operators are the operators that perform operations on a single operand
 * to produce a new value.
 */
void unary_operators() {

    /**
     * Unary operators
     * 
     * 1. Unary minus (-)
     * 2. Increment (++)
     * 3. Decrement (--)
     * 4. Logical negation (!)
     * 5. Bitwise negation (~)
     * 6. Sizeof
     * 7. Address of (&)
     * 8. Indirection (pointer) (*)
     */

    // Indirection = Initialize a pointer to a variable
    // Defereference = Get the value of the variable the pointer is pointing to

}
