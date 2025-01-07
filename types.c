#include <stdio.h>
#include <stdint.h>
#include <string.h>

void defaults();
void print_pretty(float, uint8_t);
void print_types();
void unary_operators();

/**
 * Type coversion can be implicit or explicit
 * 
 * Implicit = Automatic
 * Explicit = Manual
 */
int main(int argc, char *argv[]) {

    defaults();
    print_types();
    
    printf("\n");
    return 0;
}

void defaults() {
    // char
    char a; // 1 byte

    // string
    char *e[10]; // 10 bytes string

    // ints
    int b; // 4 bytes

    // floats
    float l; // 4 bytes

    // doubles
    double m; // 8 bytes

    // more ints from stdint.h
    int8_t c; // 1 byte (-128 - 127)
    int16_t d; // 2 bytes
    int32_t f; // 4 bytes
    int64_t g; // 8 bytes
    uint8_t h; // 1 byte (0 - 255)
    uint16_t i; // 2 bytes
    uint32_t j; // 4 bytes
    uint64_t k; // 8 bytes
    // print_pretty(l, h); // pass by value


    // may change size depending on major 32-bit systems or 64-bit systems (use stdint.h)
    // char c; // 1 byte
    // short d; // 2 bytes
    // int f; // 4 bytes
    // long g; // 4 bytes or 8 bytes
    // long long g; // 8 bytes
    // unsigned char h; // 1 byte
    // unsigned short i; // 2 bytes
    // unsigned int j; // 4 bytes
    // unsigned long k; // 4 bytes or 8 bytes
    // unsigned long long k; // 8 bytes

}

void print_pretty(float l, uint8_t h) {
    printf("---Printing pretty---\n");
    l = 3.14159;
    printf("%f\n", l); // 3.141590 (6 decimal places)
    printf("%.2f\n", l); // 3.14 (2 decimal places)

    h = 255; // uint8_t (0 - 255)
    printf("%d \n", h);

    // Arrays
    printf("*** Arrays ***\n");
    int arr[15];
    char list[3];

    arr[0] = 10;
    arr[1] = 2;
    arr[2] = 2;

    list[0] = 'a';
    list[1] = 'b';
    list[2] = 'c';

    printf("arr[0]= %d\n", arr[0]);
    printf("list[1]= %c\n", list[1]);

    // Loops
    printf("\n*** Loops ***\n");
    for(int i = 0; i < 10; i++) {
        printf("%d ", i);
    }

    // Others
    printf("\n---End, Printing pretty---\n");
}

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


    printf("%8s %5s\n", "Type", "Size");
    printf("----------------\n");
    printf("%8s %5ld\n", "char", sizeof(char));
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

}
